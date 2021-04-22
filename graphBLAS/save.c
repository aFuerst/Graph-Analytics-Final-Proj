#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "GraphBLAS.h"
#include "LAGraph.h"

GrB_Info SaveData(GrB_Vector data, char *class, char *dataset)
{
  char fname[200];
  sprintf(fname, "./savedata/%s-%s.csv", class, dataset);
  FILE *fp = fopen(fname, "w");
  if (fp == NULL)
  {
    printf("failed to open file");
    exit(1);
  }
  fprintf(fp, "node,value\n");

  GrB_Index nvals;
  GrB_Vector_nvals(&nvals, data);
  double val;
  for (GrB_Index i = 0; i < nvals; ++i)
  {
    GrB_Vector_extractElement(&val, data, i);
    fprintf(fp, "%lu,%f\n", i, val);
  }
  fclose(fp);
  return GrB_SUCCESS;
}

GrB_Info SaveAPSPData(int total_samples, double total_time, double *timedata, const char *dataset, GrB_Matrix apsp)
{
  char fname[200];
  sprintf(fname, "./savedata/shortestpath_times-%s.csv", dataset);
  FILE *fp = fopen(fname, "w");
  if (fp == NULL) {
    printf("failed to open file");
    exit(1);
  }
  fprintf(fp, "node,value\n");

  // double val;
  fprintf(fp, "%d,%f\n", -1, total_time);
  for (int i = 0; i < total_samples; ++i) {
    fprintf(fp, "%d,%f\n", i, timedata[i]);
  }
  fclose(fp);

  char fname2[200];
  sprintf(fname2, "./savedata/sp_dist-%s.csv", dataset);
  FILE *fp2 = fopen(fname2, "w");
  if (fp2 == NULL) {
    printf("failed to open file");
    exit(1);
  }

  GrB_Index ncols, nrows;
  CUST_OK(GrB_Matrix_ncols(&ncols, apsp));
  CUST_OK(GrB_Matrix_nrows(&nrows, apsp));
  int val;
  char msg[1000];
  int counts[100];
  for (int x = 0; x < 100;++x){
    counts[x] = 0;
  }

    // printf("%lu ncols, %lu nrows\n", ncols, nrows);
  GrB_Index i = 0;
  for (GrB_Index j = 0; j < ncols; ++j) {
    GrB_Info r = GrB_Matrix_extractElement(&val, apsp, i, j);
    if (r == GrB_SUCCESS) {
      // printf("(%lu,%lu) = %f\n", i, j, val);
      counts[val]++;
      // fprintf(fp2, "%d,", val);
    } else if (r == GrB_NO_VALUE) {
      // fprintf(fp2, "%d,", INT_MAX);
    } else {
      sprintf(msg, "apsp save distances, iter:%lu, col %lu\n", i, j);
      MSG_OK(r, msg);
    }
  }
  for (int x = 0; x < 100; ++x)
  {
    fprintf(fp2, "%d\n", counts[x]);
  }
  fclose(fp2);

  return GrB_SUCCESS;
}