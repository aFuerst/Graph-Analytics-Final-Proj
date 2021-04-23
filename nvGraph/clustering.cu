#include <nvgraph.h>
#include <stdio.h> 
#include <sys/time.h>
#include <time.h>

void check(nvgraphStatus_t status) {
    if (status != NVGRAPH_STATUS_SUCCESS) {
        printf("ERROR : %d\n",status);
        exit(0);
    }
}

int main(int argc, char **argv) {
    /*Check Errors*/
    if(argc != 5){
        printf("Arguments should be in following order:\n");
        printf("<# Vertices> <# Edges> <Offset txt File> <Indices txt File>\n");
        return 1;
    }

    size_t n, nnz;
    int i;

    /*Declare time based variables*/
    struct timeval tv1, tv2;
    struct timezone tz;
    long int total_time = 0;

    /*Assign Variables*/
    n = atoi(argv[1]);
    nnz = atoi(argv[2]);

    /*Open Files*/
    FILE *offsets, *indices;
    offsets = fopen(argv[3], "r");
    indices = fopen(argv[4], "r");

    float destination_offsets[n+1];
    int destination_offsets_h[n+1];
    float source_indices[nnz];
    int source_indices_h[nnz];
    uint64_t trcount;

    for (i = 0; i < nnz; i++) {        
        /*Assign Indices*/
        fscanf(indices, "%e", &source_indices[i]);
        source_indices_h[i] = source_indices[i];
    }

    for (i = 0; i < n; i++) {
        /*Assign Indices*/
        fscanf(offsets, "%e", &destination_offsets[i]);
        destination_offsets_h[i] = destination_offsets[i];
    }
    
    fclose(indices);
    fclose(offsets);

    destination_offsets_h[n] = nnz;

    // nvgraph variables
    nvgraphHandle_t handle;
    nvgraphGraphDescr_t graph;
    nvgraphCSCTopology32I_t CSC_input;

    // Init host data
    CSC_input = (nvgraphCSCTopology32I_t) malloc(sizeof(struct nvgraphCSCTopology32I_st));

    check(nvgraphCreate(&handle));
    check(nvgraphCreateGraphDescr (handle, &graph));
    CSC_input->nvertices = n; 
    CSC_input->nedges = nnz;
    CSC_input->destination_offsets = destination_offsets_h;
    CSC_input->source_indices = source_indices_h;

    // Set graph connectivity
    check(nvgraphSetGraphStructure(handle, graph, (void*)CSC_input, NVGRAPH_CSC_32));

    // Count Triangles 100 times to grab time
    gettimeofday(&tv1,&tz); // Get starting time
    for (i = 0; i < 100; i++) {
        /*Count Triangles*/
        trcount = 0;
    
        check(nvgraphTriangleCount(handle, graph, &trcount));
    }    
    gettimeofday(&tv2,&tz); // Get ending time
    /*Calculate time taken in microseconds*/
    total_time = (tv2.tv_sec-tv1.tv_sec)*1000000 + (tv2.tv_usec-tv1.tv_usec);
    printf("Average Time: %ld\n", total_time/100);
    printf("Triangle Count: %ld\n", trcount);

    free(CSC_input);
    check(nvgraphDestroyGraphDescr(handle, graph));
    check(nvgraphDestroy(handle));
    return 0;
}