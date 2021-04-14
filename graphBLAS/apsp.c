#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "GraphBLAS.h"
#include "LAGraph.h"

double LAGraph_toc(const double tic [2]);
void LAGraph_tic(const double tic [2]);
void CUST_OK(GrB_Info p);

GrB_Info AllPairsShortestPath(GrB_Matrix matrix, GrB_Matrix* apsp, double* time) {
  double tictok[2]; 
  LAGraph_tic(tictok);

  GrB_Index nrows;
  CUST_OK(GrB_Matrix_nrows(&nrows, matrix)); // n = # of rows of A
  GrB_Vector compute_output;
  CUST_OK(GrB_Vector_new(&compute_output, GrB_FP64, nrows));

  GrB_Matrix result;
  CUST_OK(GrB_Matrix_new(&result, GrB_FP64, nrows, nrows));

  // GrB_Matrix temp;
  // CUST_OK(GrB_Matrix_new(&result, GrB_FP64, 1, nrows));
  // GrB_Index rows[1];
  // GrB_Index columns[1] = { GrB_ALL };


  for (GrB_Index i=0; i < nrows; ++i) {
    MSG_OK(LAGraph_BF_basic(&compute_output, matrix, i), "apsp BF basic");
    MSG_OK(GrB_assign(result, GrB_NULL, GrB_NULL, compute_output, i, GrB_ALL, 1, GrB_NULL), "apsp assign");

    // for (GrB_Index j=0; j < nrows; ++j) { 
    //   double temp;
    //   GrB_Vector_extractElement(&temp, compute_output, j);
    //   CUST_OK(GrB_Matrix_setElement(result, temp, i, j));
    // }
  }
  *apsp = result;
  *time = LAGraph_toc(tictok);
  MSG_OK(GrB_free(&compute_output), "freeing compute_output");
  return GrB_SUCCESS;
}