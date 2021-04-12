#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "GraphBLAS.h"
#include "LAGraph.h"

GrB_Info AllPairsShortestPath(GrB_Matrix matrix, GrB_Matrix* apsp) {
  GrB_Index nrows;
  CUST_OK(GrB_Matrix_nrows(&nrows, matrix)); // n = # of rows of A
  GrB_Vector temp_output;
  CUST_OK(GrB_Vector_new(&temp_output, GrB_FP64, nrows));

  GrB_Matrix result;
  CUST_OK(GrB_Matrix_new(&result, GrB_FP64, nrows, nrows));

  for (GrB_Index i=0; i < nrows; ++i) {
    CUST_OK(LAGraph_BF_basic(&temp_output, matrix, i));
    for (GrB_Index j=0; j < nrows; ++j) { 
      double temp;
      GrB_Vector_extractElement(&temp, temp_output, j);
      CUST_OK(GrB_Matrix_setElement(result, temp, i, j));
    }
  }
  *apsp = result;
  return GrB_SUCCESS;
}