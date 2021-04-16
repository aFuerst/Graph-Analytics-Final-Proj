#include "GraphBLAS.h"
#include "LAGraph.h"

void CUST_OK(GrB_Info p) {
  if(p != GrB_SUCCESS) {
    printf("error %d\n", p);
    exit(p);
  }
}

void MSG_OK(GrB_Info p, char *msg) {
  char *error = "unknown error";
  if (p == GrB_OUT_OF_MEMORY) {
    error = "out of memory";
  }
  if (p == GrB_INSUFFICIENT_SPACE) {
    error = "INSUFFICIENT SPACE";
  }
  if (p == GrB_INVALID_OBJECT) {
    error = "GrB INVALID OBJECT";
  }
  if (p == GrB_INDEX_OUT_OF_BOUNDS) {
    error = "GrB INDEX OUT OF BOUNDS";
  }
  if (p == GrB_PANIC) {
    error = "GrB PANIC";
  }
  if (p == GrB_UNINITIALIZED_OBJECT) {
    error = "GrB UNINITIALIZED OBJECT";
  }
  if (p == GrB_NULL_POINTER) {
    error = "GrB NULL POINTER";
  }
  if (p == GrB_INVALID_VALUE) {
    error = "GrB INVALID VALUE";
  }
  if (p == GrB_INVALID_INDEX) {
    error = "GrB INVALID INDEX";
  }
  if (p == GrB_DOMAIN_MISMATCH) {
    error = "GrB DOMAIN MISMATCH";
  }
  if (p == GrB_DIMENSION_MISMATCH) {
    error = "GrB DIMENSION MISMATCH";
  }
  if (p == GrB_OUTPUT_NOT_EMPTY) {
    error = "GrB OUTPUT NOT EMPTY";
  }
  if (p == GrB_NO_VALUE) {
    error = "GrB NO VALUE";
  }
  if(p != GrB_SUCCESS) {
    printf("error %d %s, %s\n", p, msg, error);
    exit(p);
  }
}

// void cust_free(void **p) {  // pointer to object to free, does nothing if NULL
//   if (p != NULL && (*p) != NULL) {
//       free (*p);
//       (*p) = NULL;
//   }
// }