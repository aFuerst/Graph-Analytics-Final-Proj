void create_ok(GrB_Info p) {
  if(p != GrB_SUCCESS) {
    printf("error %d\n", p);
    exit(p);
  }
}

void cust_free(void **p) {  // pointer to object to free, does nothing if NULL
  if (p != NULL && (*p) != NULL) {
      free (*p);
      (*p) = NULL;
  }
}