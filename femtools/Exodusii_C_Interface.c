#include <stdio.h>
#include <stdlib.h>
#include "netcdf.h"
#include "exodusII.h"


/* Open ExodusII File for reading */
int c_read_ex_open(const char *path, int mode, int *comp_ws, int *io_ws, float *version)
{
   int exoid;
   exoid = ex_open(path, EX_READ, comp_ws, io_ws, version);
   return (exoid);
}


/* read database parameters */
int c_ex_get_init(int *exoid, char *title, int *num_dim, int *num_nodes,
                  int *num_elem, int *num_elem_blk, int *num_node_sets,
                  int *num_side_sets)
{
   int error;
   error = ex_get_init(*exoid, title, num_dim, num_nodes, num_elem, num_elem_blk, num_node_sets, num_side_sets);
   return (error);
}

/* read node coordinates */
int c_ex_get_coord(int *exoid, float *x,float *y,float *z)
{
   int error;
   error = ex_get_coord(*exoid, x, y, z);
   return(error);
}


/* read node number map */
int c_ex_get_node_num_map(int *exoid, int *node_map)
{
   int error;
   error = ex_get_node_num_map(*exoid, node_map);
   return (error);
}

/* read element number map */
int c_ex_get_elem_num_map(int *exoid, int *elem_num_map)
{
   int error;
   error = ex_get_elem_num_map(*exoid, elem_num_map);
   return (error);
}

/* read element order map */
int c_ex_get_elem_order_map(int *exoid, int *elem_order_map)
{
   int error;
   error = ex_get_map(*exoid, elem_order_map);
   return (error);
}

/* get element block ids only */
int c_ex_get_elem_blk_ids(int *exoid, int *block_ids)
{
   int error;
   error = ex_get_elem_blk_ids(*exoid, block_ids);
   return (error);
}

/* get element type of blocks, number of elements in blocks,
   and number of nodes per element */
int c_ex_get_elem_block(int *exoid, int *block_id, const char *elem_type, int *num_elem_in_block, int *num_nodes_per_elem, int *num_attr)
{
   int error;
   error = ex_get_elem_block(*exoid,
                             *block_id,
                             elem_type,
                             num_elem_in_block,
                             num_nodes_per_elem,
                             num_attr);
   return (error);
}

/* read element connectivity of block with id 'block_id' */
int c_ex_get_elem_connectivity(int *exoid, int *block_id, int *elem_connectivity)
{
   int error;
   error = ex_get_elem_conn(*exoid, *block_id, elem_connectivity);
   return (error);
}

/* read node list for node set of id 'node_set_id' */
int c_ex_get_node_set_node_list(int *exoid, int *num_node_sets, int *node_set_id, int *node_set_node_list)
{
   int error;
   error = ex_get_node_set(*exoid, *node_set_id, node_set_node_list);
   return (error);
}

/* Close ExodusII File */
int c_ex_close(int *exoid)
{
   int error;
   error = ex_close(*exoid);
   return (error);
}

/* read individual node sets */
int c_ex_get_node_set_param(int *exoid, int *num_node_sets, int *node_set_ids, int *num_nodes_in_set)
{
   int error;
   int num_df_in_set;
   int i;
   // Get node set IDs:
   error = ex_get_node_set_ids(*exoid, node_set_ids);
   // Assemble array with number of nodes per node set 'num_nodes_in_set':
   for (i=0; i<*num_node_sets; i++)
   {
      error = ex_get_node_set_param(*exoid, node_set_ids[i], &num_nodes_in_set[i], &num_df_in_set);
   }
   return (error);
}

int c_ex_get_side_set_ids(int *exoid, int *side_set_ids)
{
   int error;
   error = ex_get_side_set_ids (*exoid, side_set_ids);
   return (error);
}

/* read individual side set parameters */
int c_ex_get_side_set_param(int *exoid, int *side_set_id, int *num_sides_in_set, int *num_df_in_set)
{
   int error;
   // Get side set parameters:
   //printf("side_set_id: %d\n", *side_set_id);
   error = ex_get_side_set_param(*exoid, *side_set_id, num_sides_in_set, num_df_in_set);
   return (error);
}


/* Following some additional interfaces that differ from the official interfaces of the exodusii library */

/* Nicer function for open ExodusII File for reading */
int c_read_ex_open_short(const char *path, float *version)
{
   int exoid;
   int comp_ws=0, io_ws=0;
   exoid = ex_open(path, EX_READ, &comp_ws, &io_ws, version);
   return (exoid);
}

/* get element block ids and block parameters */
int c_ex_get_elem_block_parameters(int *exoid, int *num_elem_blk, int *block_ids, int *num_elem_in_block, int *num_nodes_per_elem)
{
   int error;
   char elem_type[MAX_STR_LENGTH+1];
   int *num_attr;
   int i;
   // Get element block ids:
   error = ex_get_elem_blk_ids(*exoid, block_ids);
   // Get element type of blocks, number of elements in blocks, and
   // number of nodes per element:
   num_attr = (int *) calloc(*num_elem_blk, sizeof(int));
   
   for (i=0; i<*num_elem_blk; i++)
   {
      error = ex_get_elem_block(*exoid,
                                block_ids[i],
                                elem_type,
                                &(num_elem_in_block[i]),
                                &(num_nodes_per_elem[i]),
                                &(num_attr[i]));
   }
   free(num_attr);
   return (error);
}

