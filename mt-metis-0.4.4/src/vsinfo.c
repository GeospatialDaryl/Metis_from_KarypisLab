/**
 * @file vsinfo.c
 * @brief FUnctions for allocating vertex separator refinement information.
 * @author Dominique LaSalle <lasalle@cs.umn.edu>
 * Copyright 2014, Regents of the University of Minnesota
 * @version 1
 * @date 2014-11-11
 */




#ifndef MTMETIS_VSINFO_C
#define MTMETIS_VSINFO_C




#include "vsinfo.h"




/******************************************************************************
* DOMLIB MACROS ***************************************************************
******************************************************************************/


#define DLMEM_PREFIX vsnbrinfo
#define DLMEM_TYPE_T vsnbrinfo_t
#include <dlmem_funcs.h>
#undef DLMEM_TYPE_T
#undef DLMEM_PREFIX




/******************************************************************************
* PUBLIC FUNCTIONS ************************************************************
******************************************************************************/


void vsinfo_free(
    graph_t * const graph)
{
  tid_t myid;
  vsinfo_t * vsinfo;

  for (myid=0;myid<graph->dist.nthreads;++myid) {
    vsinfo = graph->vsinfo+myid;
    if (vsinfo->bnd) {
      vtx_iset_free(vsinfo->bnd);
    }
    if (vsinfo->nbrinfo) {
      dl_free(vsinfo->nbrinfo);
    }
  }

  dl_free(graph->vsinfo);
  graph->vsinfo = NULL;
}


void par_vsinfo_create(
    ctrl_t * const ctrl,
    graph_t * const graph)
{
  vsinfo_t * vsinfo;

  tid_t const myid = dlthread_get_id(ctrl->comm);
  tid_t const nthreads = dlthread_get_nthreads(ctrl->comm);

  vsinfo = dlthread_get_shmem(sizeof(vsinfo_t)*nthreads,ctrl->comm);

  vsinfo += myid;

  vsinfo->bnd = vtx_iset_create(0,graph->mynvtxs[myid]);
  vsinfo->nbrinfo = vsnbrinfo_alloc(graph->mynvtxs[myid]);
  
  if (myid == 0) {
    graph->vsinfo = vsinfo;
  }

  dlthread_barrier(ctrl->comm);
}


void par_vsinfo_free(
    graph_t * const graph)
{
  vsinfo_t * vsinfo;

  tid_t const myid = dlthread_get_id(graph->comm);

  vsinfo = graph->vsinfo+myid;

  if (vsinfo->bnd) {
    vtx_iset_free(vsinfo->bnd);
  }
  if (vsinfo->nbrinfo) {
    dl_free(vsinfo->nbrinfo);
  }

  dlthread_free_shmem(graph->vsinfo,graph->comm);
  if (myid == 0) {
    graph->vsinfo = NULL;
  }
}







#endif
