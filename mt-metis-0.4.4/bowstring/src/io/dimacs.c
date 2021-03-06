/**
 * @file dimacs.c
 * @brief Functions for reading and writing graphs in the DIMACS format
 * @author Dominique LaSalle <lasalle@cs.umn.edu>
 * Copyright 2013
 * @version 1
 * @date 2014-02-13
 */




#ifndef BOWSTRING_IO_DIMACS_C
#define BOWSTRING_IO_DIMACS_C




#include "dimacs.h"



/******************************************************************************
* CONSTANTS *******************************************************************
******************************************************************************/


static const char DIMACS_COMMENT = 'c';
static const char DIMACS_PROBLEM = 'p';
static const char DIMACS_ARC = 'a';
static const char DIMACS_NODE_DESCRIPTOR = 'n';
static const char DIMACS_EDGE = 'e';




/******************************************************************************
* PUBLIC FUCNTIONS ************************************************************
******************************************************************************/


int read_dimacs_graph(const char * const filename, vtx_t * const r_nvtxs, 
    adj_t ** const xadj, vtx_t ** const adjncy, wgt_t ** const vwgt, 
    wgt_t ** const adjwgt)
{
  return BOWSTRING_ERROR_UNIMPLEMENTED;
}


int write_dimacs_graph(const char * const filename, const vtx_t nvtxs, 
    const adj_t * const xadj, const vtx_t * const adjncy, 
    const wgt_t * const vwgt, const wgt_t * const adjwgt)
{
  file_t * fout;
  vtx_t i, u,v;
  adj_t j, nedges;
  wgt_t w;
  int rv;

  nedges = (size_t)xadj[nvtxs];

  if ((rv = __open_file(filename,"w",&fout)) != BOWSTRING_SUCCESS) {
    goto ERROR;
  } 

  dl_fprintf(fout,"%c File generated by GraphUtils\n",DIMACS_COMMENT);
  dl_fprintf(fout,"%c sp "PF_VTX_T" "PF_ADJ_T"\n",DIMACS_PROBLEM,nvtxs,nedges);

  for (i=0;i<nvtxs;++i) {
    v = i+1;
    for (j=(size_t)xadj[i];j<(size_t)xadj[i+1];++j) {
      u = ((size_t)adjncy[j])+1;
      if (adjwgt) {
        w = adjwgt[j];
      } else {
        w = 1;
      }
      dl_fprintf(fout,"%c "PF_VTX_T" "PF_VTX_T" "PF_WGT_T"\n",DIMACS_ARC,v,u,
          w);
    }
  }

  dl_close_file(fout);

  return BOWSTRING_SUCCESS;

  ERROR:

  if (fout) {
    dl_close_file(fout);
    fout = NULL;
  }

  return rv;
}




#endif
