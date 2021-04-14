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