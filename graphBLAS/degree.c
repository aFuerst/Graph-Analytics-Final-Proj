#include "GraphBLAS.h"
#include "LAGraph.h"

double LAGraph_toc(const double tic [2]);
void LAGraph_tic(const double tic [2]);

/*
* Computes the degree of each vertex, placing these values in 'deg' vector
*/
GrB_Info Degree(GrB_Matrix matrix, GrB_Vector deg, double* time) {
  double tictok[2]; 
  LAGraph_tic(tictok);
  GrB_Info ret = GrB_reduce(deg, GrB_NULL, GrB_NULL, GrB_PLUS_UINT64, matrix, GrB_NULL);
  *time = LAGraph_toc(tictok);
  return ret;
}
