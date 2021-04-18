#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include "GraphBLAS.h"
#include "LAGraph.h"

double LAGraph_toc(const double tic [2]);
void LAGraph_tic(const double tic [2]);
void CUST_OK(GrB_Info p);
void MSG_OK(GrB_Info p, char *msg);
GrB_Info GrB_Row_assign(GrB_Matrix C, const GrB_Vector mask, const GrB_BinaryOp accum, const GrB_Vector u, GrB_Index r_idx, const GrB_Index *col_idcs, GrB_Index ncols, const GrB_Descriptor desc);

GrB_Info AllPairsShortestPath(GrB_Matrix matrix, GrB_Matrix* apsp, double* time) {
  double tictok[2]; 
  LAGraph_tic(tictok);

  GrB_Index nrows;
  CUST_OK(GrB_Matrix_nrows(&nrows, matrix)); // n = # of rows of A
  GrB_Vector compute_output;
  CUST_OK(GrB_Vector_new(&compute_output, GrB_FP64, nrows));

  GrB_Matrix result;
  CUST_OK(GrB_Matrix_new(&result, GrB_FP64, nrows, nrows));

  GrB_Vector degrees;
  CUST_OK(GrB_Vector_new(&degrees, GrB_UINT32, nrows));
  double throw[2];
  Degree(matrix, degrees, throw);

  // GrB_Matrix temp;
  // CUST_OK(GrB_Matrix_new(&result, GrB_FP64, 1, nrows));
  // GrB_Index rows[1];
  // GrB_Index columns[1] = { GrB_ALL };


  char msg[1000];
  uint32_t deg = 0;
  // GrB_Index idx[1];
  for (GrB_Index i=0; i < nrows; ++i) {
    MSG_OK(GrB_Vector_extractElement(&deg, degrees, i), "apsp degree check");
    if (deg == 0) {
      GrB_Index row[1] = {i};
      MSG_OK(GrB_assign(result, GrB_NULL, GrB_NULL, (double)DBL_MAX, row, 1, GrB_ALL, nrows, GrB_NULL),
             "assign float mx in apsp");
    } else {
      MSG_OK(LAGraph_BF_basic(&compute_output, matrix, i), "apsp BF basic");
      sprintf(msg, "apsp assign, iter:%lu, len %lu\n", i, nrows);
      // printf("curr iter %lu\n", i);
      // idx[0] = i;
      GrB_Info r = GrB_Row_assign(result, GrB_NULL, GrB_NULL, compute_output, i, GrB_ALL, (GrB_Index)1, GrB_NULL);

      if (r == GrB_NULL_POINTER) {
        printf("null ptr on idx '%lu'\n", i);
        double temp;
        for (GrB_Index j = 0; j < nrows; ++j) {
          GrB_Vector_extractElement(&temp, compute_output, j);
          printf("val at %lu: %f\n", j, temp);
          // CUST_OK(GrB_Matrix_setElement(result, temp, i, j));
        }

        // GrB_Vector_extractElement(&temp, compute_output, i);
        printf("degree of idx %lu: %u\n", i, deg);
      }
      MSG_OK(r, msg);
    }
  }
  *apsp = result;
  *time = LAGraph_toc(tictok);
  MSG_OK(GrB_free(&compute_output), "freeing compute_output");
  return GrB_SUCCESS;
}