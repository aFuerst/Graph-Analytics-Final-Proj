#define  _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "GraphBLAS.h"
#include "LAGraph.h"

/*
* Assumptions:
*   All nodes are labeled 0-|V-1|
*   
*/

// 'includes'
GrB_Info LAGraph_init();
GrB_Info LAGraph_finalize();
double LAGraph_toc(const double tic [2]);
void LAGraph_tic(const double tic [2]);
void CUST_OK(GrB_Info p);
GrB_Info AllPairsShortestPath(GrB_Matrix matrix, GrB_Matrix* apsp, double* time);

const char *graph_names[] = {"gnutella", "wikivote", "hepth", "bitcoin", "enron", "hepph"};
GrB_Info LoadHepTh(GrB_Matrix *matrix);
GrB_Info LoadGnutella(GrB_Matrix *matrix);
GrB_Info LoadWikiVote(GrB_Matrix *matrix);
GrB_Info LoadBitcoinOTC(GrB_Matrix *matrix);
GrB_Info LoadEnron(GrB_Matrix *matrix);
GrB_Info LoadHepPh(GrB_Matrix *matrix);
GrB_Info (*load_funcs[])(GrB_Matrix *matrix) = {LoadGnutella, LoadWikiVote, LoadHepTh, LoadBitcoinOTC, LoadEnron, LoadHepPh};
const int NUM_GRAPHS = 6;
/*
* Given a boolean n x n adjacency matrix A and a source vertex s,performs a BFS traversal
* of the graph and sets v[i] to the level in which vertex i is visited (v[s]==1).
* If i is not reacheable from s, then v[i]=0.(Vector v should be empty on input.)
*/
GrB_Info BFS(GrB_Vector *v , GrB_Matrix A, GrB_Index s)
{
  GrB_Index n;
  GrB_Matrix_nrows(&n ,A ); // n = # of rows of A
  GrB_Vector_new(v , GrB_INT32 , n ); // Vector<i nt 3 2 t > v (n )
  GrB_Vector q; // v e r t i c e s v i s i t e d in each l e v e l
  GrB_Vector_new(&q ,GrB_BOOL, n ); // Vector<bool> q (n )
  GrB_Vector_setElement(q, (bool)true, s) ; // q [ s ] = true , f a l s e everywhere e l s e
  /*
  * BFS t r a v e r s a l and l a b e l th e v e r t i c e s .
  */
  int32_t d = 0; // d = l e v e l in BFS t r a v e r s a l
  bool succ = false; // succ == t ru e when some su c c e s so r found
  do {
    ++d; // next l e v e l ( s t a r t with 1)
    GrB_assign(*v, q, GrB_NULL, d, GrB_ALL, n, GrB_NULL) ; // v [ q ] = d
    GrB_vxm(q, *v, GrB_NULL, GrB_LOR_LAND_SEMIRING_BOOL, q, A, GrB_DESC_RC); // q [ ! v ] = q jj.&& A ; f i n d s a l l th e
    // u n v i s it e d su c c e s s o r s from cu r r ent q
    GrB_reduce(&succ, GrB_NULL,GrB_LOR_MONOID_BOOL, q, GrB_NULL) ; // succ = j j ( q )
  } while (succ); // i f t h e r e i s no su c c e s so r in q , we are done .
  GrB_free(&q); // q v e ct o r no lon g e r needed
  return GrB_SUCCESS;
}

void InitGraph(GrB_Matrix *matrix) {
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
  char* fname = "../graphs/social/soc-sign-bitcoinotc-patch.csv";
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

}

GrB_Info Clustering(const GrB_Matrix matrix, GrB_Vector *output, double *time) {
  double t[2];
  GrB_Info ret = LAGraph_lcc(output, matrix, false, false, t);
  *time = t[1];
  return ret;
}

GrB_Info ConnectedComponents(const GrB_Matrix matrix, GrB_Vector *output, double *time) {
  double tictok[2]; 
  LAGraph_tic(tictok);

  // compute connected components
  MSG_OK(LAGraph_cc_fastsv5b(output, &matrix, false), "LAGraph_cc_fastsv5b");

  // int d;
  // for (GrB_Index i=0; i < NUM_NODES; ++i) {
  //   // output is pointers to parents (I think)
  //   GrB_Vector_extractElement(&d, v, i);
  //   printf("%d,", d);
  // }
  *time = LAGraph_toc(tictok);
  return GrB_SUCCESS;
}

/*
* Runs on preloaded matrix
*/
void RunTimes(GrB_Matrix matrix, const char *fname, GrB_Info (*load_func)(GrB_Matrix *matrix)) {
  GrB_Index nrows;
  CUST_OK(GrB_Matrix_nrows(&nrows, matrix));
  GrB_Vector vec_output;
  CUST_OK(GrB_Vector_new(&vec_output, GrB_UINT32, nrows));

  GrB_Matrix mat_output;
  CUST_OK(GrB_Matrix_new(&mat_output, GrB_UINT32, nrows, nrows));

  // time all pairs shortest path
  GrB_Matrix apsp;
  double time=0;

  // time computing node degree
  time = 0;
  CUST_OK(Degree(matrix, vec_output, &time));
  printf("Degree time: %f\n", time);

  // double x=0;
  // bool d = false;
  // for (GrB_Index i=0; i < NUM_NODES; ++i) {
  //   GrB_Matrix_extractElement(&d, matrix, i, 1000);
  //   if (!d) {
  //     // printf("%d,%d\n", d, i);
  //   }
  //   x += d;
  // }
  // printf("manual degree: %f\n", x);

  // int q;
  // GrB_Vector_extractElement(&q, v, 1000);
  // printf("gblas degree: %d\n", q);

  // time CC
  time = 0;
  CUST_OK(ConnectedComponents(matrix, &vec_output, &time));
  printf("CC time: %f\n", time);
  CUST_OK(GrB_free(&vec_output));
  CUST_OK(GrB_free(&matrix));
  
  // time local clustering
  time = 0;
  CUST_OK(GrB_Vector_new(&vec_output, GrB_FP64, nrows));
  CUST_OK(load_func(&matrix));
  MSG_OK(Clustering(matrix, &vec_output, &time), "clustering");
  printf("Clustering time: %f\n", time);

  time = 0;
  AllPairsShortestPath(matrix, &apsp, &time);
  printf("APSP time: %f\n", time);

  CUST_OK(GrB_free(&vec_output));
  CUST_OK(GrB_free(&mat_output));
  CUST_OK(GrB_free(&apsp));
}

int main(int argc, char** argv) {
  GrB_Info init = LAGraph_init();
  int SET_THREADS = 46;
  int threads = LAGraph_set_nthreads(SET_THREADS); // 2 less than on machine
  if (threads != SET_THREADS) {
    printf("Thread count not set! %d vs %d\n", threads, SET_THREADS);
    exit(1);
  }
  printf("Running with %d threads\n", threads);
  if(init != GrB_SUCCESS) {
    printf("init error %d\n", init);
    exit(init);
  }

  GrB_Matrix matrix;

  for (int i=0; i < NUM_GRAPHS; ++i) {
    printf("Running %s\n", graph_names[i]);
    CUST_OK(load_funcs[i](&matrix));
    RunTimes(matrix, graph_names[i], load_funcs[i]);
    CUST_OK(GrB_free(&matrix));
  }

  // return 0;

  // InitGraph(&matrix);



  // uint64_t tricount = trianglecount(matrix, &v);
  // long y = 0;
  // long z = 0;
  // for (GrB_Index i=0; i < NUM_NODES; ++i) {
  //   GrB_Vector_extractElement(&z, v, i);
  //   y += z;
  // }
  // printf("%ld vs %ld\n", y, tricount);


  // GrB_Info finalize = GrB_finalize();
  GrB_Info finalize = LAGraph_finalize();
  printf("%d\n", finalize);

  printf("%d\n", true);
  return 0;
}