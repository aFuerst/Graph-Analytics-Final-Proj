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

GrB_Info SanitizeForAPSP(GrB_Matrix matrix) {
  GrB_Index nrows;
  MSG_OK(GrB_Matrix_nrows(&nrows, matrix), "sanitize get nrows"); // n = # of rows of A
  for (GrB_Index i = 0; i < nrows; ++i) {
    for (GrB_Index j = 0; j < nrows; ++j) {
      if (i == j) {
        MSG_OK(GrB_Matrix_setElement(matrix, (int)0, i, j), "set i==j to 0");
      } else {
        // nothing
      }
    }
  }
  return GrB_SUCCESS;
}

GrB_Info AllPairsShortestPath(GrB_Matrix matrix, GrB_Matrix *apsp, double *time, int total_samples, double* sample_times) {
  double total[2]; 

  GrB_Index nrows;
  CUST_OK(GrB_Matrix_nrows(&nrows, matrix)); // n = # of rows of A
  // int num_samples = (int)nrows * 0.1f;
  // int num_samples = 100;
  // *total_samples = num_samples;
  // double *sample_timing = malloc(sizeof(double) * num_samples);
  for (int j = 0; j < total_samples; ++j)
  {
    sample_times[j] = 0.0;
  }

  GrB_Vector compute_output;
  CUST_OK(GrB_Vector_new(&compute_output, GrB_FP64, nrows));

  GrB_Matrix result;
  CUST_OK(GrB_Matrix_new(&result, GrB_FP64, nrows, nrows));

  GrB_Vector degrees;
  CUST_OK(GrB_Vector_new(&degrees, GrB_UINT32, nrows));
  CUST_OK(GrB_assign(degrees, GrB_NULL, GrB_NULL, (uint32_t)0, GrB_ALL, nrows, GrB_NULL));
  double throw[2];
  Degree(matrix, degrees, throw);

  // GrB_Matrix temp;
  // CUST_OK(GrB_Matrix_new(&result, GrB_FP64, 1, nrows));
  // GrB_Index rows[1];
  // GrB_Index columns[1] = { GrB_ALL };

  CUST_OK(SanitizeForAPSP(matrix));

  char msg[1000];
  uint32_t deg = 0;
  // GrB_Index idx[1];

  LAGraph_tic(total);

  double iter[2];
  // double iter_time;
  int step = (int)nrows % total_samples;
  for (GrB_Index i = 0; i < total_samples; ++i)
  {
    LAGraph_tic(iter);
    MSG_OK(LAGraph_BF_basic(&compute_output, matrix, i), "apsp BF basic");
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
    sample_times[i] = LAGraph_toc(iter);
    // printf("sample time %f\n", sample_times[i]);
    // sprintf(msg, "apsp assign, iter:%lu, len %lu\n", i, nrows);
    MSG_OK(r, "apsp assign");
    // printf("step %3d time %16.4f sec\n", i, iter_time);
  }

  // for (GrB_Index i=0; i < nrows; ++i) {
  //   MSG_OK(GrB_Vector_extractElement(&deg, degrees, i), "apsp degree check");
  //   if (deg == 0) {
  //     GrB_Index row[1] = {i};
  //     MSG_OK(GrB_assign(result, GrB_NULL, GrB_NULL, (double)DBL_MAX, row, 1, GrB_ALL, nrows, GrB_NULL),
  //            "assign float mx in apsp");
  //   } else {
  //     MSG_OK(LAGraph_BF_basic(&compute_output, matrix, i), "apsp BF basic");
  //     sprintf(msg, "apsp assign, iter:%lu, len %lu\n", i, nrows);
  //     // printf("curr iter %lu\n", i);
  //     // idx[0] = i;
  //     GrB_Info r = GrB_Row_assign(result, GrB_NULL, GrB_NULL, compute_output, i, GrB_ALL, (GrB_Index)1, GrB_NULL);

  //     if (r == GrB_NULL_POINTER) {
  //       printf("null ptr on idx '%lu'\n", i);
  //       double temp;
  //       for (GrB_Index j = 0; j < nrows; ++j) {
  //         GrB_Vector_extractElement(&temp, compute_output, j);
  //         printf("val at %lu: %f\n", j, temp);
  //         // CUST_OK(GrB_Matrix_setElement(result, temp, i, j));
  //       }

  //       // GrB_Vector_extractElement(&temp, compute_output, i);
  //       printf("degree of idx %lu: %u\n", i, deg);
  //     }
  //     MSG_OK(r, msg);
  //   }
  // }
  *apsp = result;
  *time = LAGraph_toc(total);
  // *sample_times = sample_timing;
  MSG_OK(GrB_free(&compute_output), "freeing compute_output");
  return GrB_SUCCESS;
}