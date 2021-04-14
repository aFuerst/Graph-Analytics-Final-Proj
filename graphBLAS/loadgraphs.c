#define  _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "GraphBLAS.h"
#include "LAGraph.h"

GrB_Info LAGraph_mmread
(
    GrB_Matrix *A,      // handle of matrix to create
    FILE *f             // file to read from, already open
);
void CUST_OK(GrB_Info p);

GrB_Info LoadAmazon(GrB_Matrix *matrix) {
  // Nodes 	262111
  // Edges 	1234877
  // GrB_Index const NUM_NODES = 262111;
  // int expected_nodes = NUM_NODES;
  // GrB_Info g = GrB_Matrix_new(matrix, GrB_BOOL, NUM_NODES, NUM_NODES);

  char* fname = "../graphs/amazon0302-patched.mtx";
  printf("Reading Matrix Market file: %s\n", fname);
  FILE* f = fopen(fname, "r");
  if (f == NULL) {
      printf("Matrix file not found: [%s]\n", fname);
      exit(1);
  }
  CUST_OK(LAGraph_mmread(matrix, f));
  fclose(f);

  return GrB_SUCCESS;
}

GrB_Info LoadHepTh(GrB_Matrix *matrix) {
  // Nodes 	262111
  // Edges 	1234877
  // GrB_Index const NUM_NODES = 262111;
  // int expected_nodes = NUM_NODES;
  // GrB_Info g = GrB_Matrix_new(matrix, GrB_BOOL, NUM_NODES, NUM_NODES);

  char* fname = "../graphs/cit-HepTh-patched.mtx";
  printf("Reading Matrix Market file: %s\n", fname);
  FILE* f = fopen(fname, "r");
  if (f == NULL) {
      printf("Matrix file not found: [%s]\n", fname);
      exit(1);
  }
  CUST_OK(LAGraph_mmread(matrix, f));
  fclose(f);

  return GrB_SUCCESS;
}

GrB_Info LoadGnutella(GrB_Matrix *matrix) {
  // Nodes 	262111
  // Edges 	1234877
  // GrB_Index const NUM_NODES = 262111;
  // int expected_nodes = NUM_NODES;
  // GrB_Info g = GrB_Matrix_new(matrix, GrB_BOOL, NUM_NODES, NUM_NODES);

  char* fname = "../graphs/p2p-Gnutella31-patched.mtx";
  printf("Reading Matrix Market file: %s\n", fname);
  FILE* f = fopen(fname, "r");
  if (f == NULL) {
      printf("Matrix file not found: [%s]\n", fname);
      exit(1);
  }
  CUST_OK(LAGraph_mmread(matrix, f));
  fclose(f);

  return GrB_SUCCESS;
}

GrB_Info LoadTwitter(GrB_Matrix *matrix) {
  // Nodes 	262111
  // Edges 	1234877
  // GrB_Index const NUM_NODES = 262111;
  // int expected_nodes = NUM_NODES;
  // GrB_Info g = GrB_Matrix_new(matrix, GrB_BOOL, NUM_NODES, NUM_NODES);

  char* fname = "../graphs/twitter_combined-patched.mtx";
  printf("Reading Matrix Market file: %s\n", fname);
  FILE* f = fopen(fname, "r");
  if (f == NULL) {
      printf("Matrix file not found: [%s]\n", fname);
      exit(1);
  }
  CUST_OK(LAGraph_mmread(matrix, f));
  fclose(f);

  return GrB_SUCCESS;
}

GrB_Info LoadWikiVote(GrB_Matrix *matrix) {
  // Nodes 	262111
  // Edges 	1234877
  // GrB_Index const NUM_NODES = 262111;
  // int expected_nodes = NUM_NODES;
  // GrB_Info g = GrB_Matrix_new(matrix, GrB_BOOL, NUM_NODES, NUM_NODES);

  char* fname = "../graphs/wiki-Vote-patched.mtx";
  printf("Reading Matrix Market file: %s\n", fname);
  FILE* f = fopen(fname, "r");
  if (f == NULL) {
      printf("Matrix file not found: [%s]\n", fname);
      exit(1);
  }
  CUST_OK(LAGraph_mmread(matrix, f));
  fclose(f);

  return GrB_SUCCESS;
}

GrB_Info LoadBitcoinOTC(GrB_Matrix *matrix) {
  // Nodes 	5,881
  // Edges 	35,592
  int expected_nodes = 5881;
  GrB_Index const NUM_NODES = 5881;
  GrB_Info g = GrB_Matrix_new(matrix, GrB_BOOL, NUM_NODES, NUM_NODES);

  for (GrB_Index i=0; i < NUM_NODES; ++i) {
    for (GrB_Index j=0; j < NUM_NODES; ++j) {
      GrB_Matrix_setElement(*matrix, (bool)false, i, j);
    }
  }

  if(g != GrB_SUCCESS) {
    printf("new error %d\n", g);
    exit(g);
  }
  char* fname = "../graphs/soc-sign-bitcoinotc-patch.csv";
  FILE* fp = fopen(fname,"r");
  if (fp == NULL) {
      printf("failed to open file");
      exit(1);
  }

  int cont = 0;
  size_t len = 0;
  long src;
  long dest;
  char comma = ',';
  char * ptr;

  int edges = 0;
  int bad_edges = 0;
  while (true) {
    char* line = NULL;
    // SOURCE, TARGET, RATING, TIME
    cont = getline(&line, &len, fp);
    // printf("%s\n", line);
    if (cont == -1) {
      break;
    }
    ptr = strchr(line, comma);
    size_t len = ptr - line;
    src = strtol(line, &ptr, 10);
    // printf("%lu, %ld \n", len, src);
    ptr = strchr(line+len, comma);
    dest = strtol(line+len+1, &ptr, 10);
    // printf("%lu, %ld \n", len, dest);

    if (src >= expected_nodes || dest >= expected_nodes) {
      printf("weird src or dest src:%ld, dest:%ld\n", src, dest);
      bad_edges += 1;
      continue;
    }


    GrB_Info set = GrB_Matrix_setElement(*matrix, (bool)true, src, dest);
    edges += 1;
    if(set != GrB_SUCCESS) {
      printf("set error '%d', src:%ld, dest:%ld\n", set, src, dest);
      exit(set);
    }
  }
  printf("edges: %d; bad edges: %d\n", edges, bad_edges);

  double x=0;
  bool d = false;
  for (GrB_Index i=0; i < NUM_NODES; ++i) {
    GrB_Matrix_extractElement(&d, *matrix, i, 1000);
    x += d;
  }
  printf("orig degree: %f\n", x);

  return GrB_SUCCESS;
}
