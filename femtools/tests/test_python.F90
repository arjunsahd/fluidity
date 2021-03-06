!    Copyright (C) 2006-2007 Imperial College London and others.
!    
!    Please see the AUTHORS file in the main source directory for a full list
!    of copyright holders.
!
!    Prof. C Pain
!    Applied Modelling and Computation Group
!    Department of Earth Science and Engineering
!    Imperial College London
!
!    amcgsoftware@imperial.ac.uk
!    
!    This library is free software; you can redistribute it and/or
!    modify it under the terms of the GNU Lesser General Public
!    License as published by the Free Software Foundation,
!    version 2.1 of the License.
!
!    This library is distributed in the hope that it will be useful,
!    but WITHOUT ANY WARRANTY; without even the implied warranty of
!    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
!    Lesser General Public License for more details.
!
!    You should have received a copy of the GNU Lesser General Public
!    License along with this library; if not, write to the Free Software
!    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
!    USA

#include "fdebug.h"

subroutine test_python
  !!< Test that we can set a field using python.
  use fields
  use mesh_files
  use unittest_tools
  use futils
  implicit none

  type(vector_field) :: X
  type(scalar_field) :: T
  logical :: fail

  logical :: file_exists
  integer :: unit
  character(len=10000) :: func, buffer

#ifdef HAVE_PYTHON
  X=read_mesh_files("data/interval", quad_degree=4, format="gmsh")

  call allocate(T, X%mesh, "tracer")

  call set_from_python_function(T, &
       "def val(X,t): import math; return math.cos(X[0])", X, 0.0)

  fail=any(abs(T%val-cos(X%val(1,:)))>1e-14)
  call report_test("[test_python 1D function fields]", fail, .false., &
       "python and fortran should produce the same answer.")

  call set_from_python_function(T%val, &
       "def val(X,t): import math; return math.cos(X[0])", X%val(1,:),&
       & time=0.0) 
  
  fail=any(abs(T%val-cos(X%val(1,:)))>1e-14)
  call report_test("[test_python 1D function values]", fail, .false., &
       "python and fortran should produce the same answer.")

  
  inquire(file="stflux.py",exist=file_exists)
  
  if (.not.file_exists) return
  
  unit=free_unit()
  open(unit, file="stflux.py", action="read",&
       & status="old")
  read(unit, '(a)', end=42) func
  
  ! Read all the lines of the file and put in newlines between them.
  do
     read(unit, '(a)', end=42) buffer
     
     func=trim(func)//achar(10)//trim(buffer)
     
  end do
  
42 func=trim(func)//achar(10)
  close(unit)
  
  call set_from_python_function(T%val, &
       trim(func), X%val(1,:),&
       & time=0.0) 
  

#endif

end subroutine test_python
