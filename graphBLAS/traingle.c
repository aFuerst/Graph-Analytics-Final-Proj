#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include"GraphBLAS.h"
/*
* Given, L, the lower triangular portion of n x n adjacency matrix A (of an
* undirected graph), computes the number of triangles in the graph.
*/
uint64_t trianglecount(GrB_Matrix L, GrB_Vector* tris)  //L:NxN,lower−triangular,bool
{
  GrB_Index n;
  GrB_Matrix_nrows(&n, L); // n = # of vertices

  GrB_Matrix C;
  GrB_Matrix_new(&C, GrB_UINT64, n, n);

  GrB_mxm(C, L, GrB_NULL, GrB_PLUS_TIMES_SEMIRING_UINT64, L, L, GrB_DESC_T1); //C<L>=L+.*L’

  uint64_t count;
  GrB_reduce(&count,GrB_NULL,GrB_PLUS_MONOID_UINT64,C,GrB_NULL); //1−normofC

  GrB_reduce(*tris, GrB_NULL, GrB_NULL, GrB_PLUS_MONOID_UINT64, L, GrB_NULL);

  GrB_free(&C);//C matrix no longer needed

  return count;
}