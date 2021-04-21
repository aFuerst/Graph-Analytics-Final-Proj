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

GrB_Info SaveAPSPData(int total_samples, double total_time, double *timedata, const char *dataset)
{
  char fname[200];
  sprintf(fname, "./savedata/shortestpath_times-%s.csv", dataset);
  FILE *fp = fopen(fname, "w");
  if (fp == NULL)
  {
    printf("failed to open file");
    exit(1);
  }
  fprintf(fp, "node,value\n");

  // double val;
  fprintf(fp, "%d,%f\n", -1, total_time);
  for (int i = 0; i < total_samples; ++i)
  {
    fprintf(fp, "%d,%f\n", i, timedata[i]);
  }
  fclose(fp);
  return GrB_SUCCESS;
}