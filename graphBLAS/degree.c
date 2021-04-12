#include "GraphBLAS.h"
#include "LAGraph.h"

/*
* Computes the degree of each vertex, placing these values in 'deg' vector
*/
GrB_Info Degree(GrB_Matrix matrix, GrB_Vector deg) {
  return GrB_reduce(deg, GrB_NULL, GrB_NULL, GrB_PLUS_UINT64, matrix, GrB_NULL);
}
