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
    if(argc != 6){
        printf("Arguments should be in following order:\n");
        printf("<# Vertices> <# Edges> <Offset txt File> <Indices txt File> <Save Name>\n");
        return 1;
    }

    size_t n, nnz;
    int i, j, k;

    /*Declare time based variables*/
    struct timeval tv1, tv2;
    struct timezone tz;
    long int total_time = 0;

    /*Assign Variables*/
    n = atoi(argv[1]);
    nnz = atoi(argv[2]);

    /*Open Files*/
    FILE *offsets, *indices, *results;
    offsets = fopen(argv[3], "r");
    indices = fopen(argv[4], "r");

    float destination_offsets[n+1];
    int destination_offsets_h[n+1];
    float source_indices[nnz];
    int source_indices_h[nnz];
    int *subvertices;
    int flag;
    float *clustering;
    int numvertices;
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
    nvgraphGraphDescr_t graph, subgraph;
    nvgraphCSCTopology32I_t CSC_input;

    // Init host data
    CSC_input = (nvgraphCSCTopology32I_t) malloc(sizeof(struct nvgraphCSCTopology32I_st));
    subvertices = (int*)malloc(n*sizeof(int));
    clustering = (float*)malloc(n*sizeof(float));

    check(nvgraphCreate(&handle));
    check(nvgraphCreateGraphDescr (handle, &graph));
    CSC_input->nvertices = n; 
    CSC_input->nedges = nnz;
    CSC_input->destination_offsets = destination_offsets_h;
    CSC_input->source_indices = source_indices_h;

    // Set graph connectivity
    check(nvgraphSetGraphStructure(handle, graph, (void*)CSC_input, NVGRAPH_CSC_32));

    // Iterate through vertices and calculate clustering coefficient
    for (i = 0; i < 100; i++) {
        numvertices = destination_offsets_h[i+1] - destination_offsets_h[i] + 1;
        /*Fill subvertices*/
        if (numvertices == 1)       clustering[i] = 0;            
        else if (numvertices == 2)  clustering[i] = 0; 
        else{
            /*Fill subvertices*/
            flag = 0;
            k = 0;
            for(j = 0; j < numvertices; j++){
                subvertices[j] = source_indices_h[destination_offsets_h[i]+k];
                k++;
                if(flag == 0) {
                    if(subvertices[j] > i){
                        subvertices[j + 1] = subvertices[j];
                        subvertices[j] = i;
                        j++;
                        flag = 1;
                    }
                }
            }

            if(flag == 0) subvertices[numvertices-1] = i;
printf("Vert: %d\n", numvertices);
printf("i = %d >> %d %d %d || %d %d %d\n", i, subvertices[0], subvertices[1],  subvertices[2], subvertices[numvertices-2], subvertices[numvertices-1], subvertices[numvertices]);
            /*Create Subgraph*/
            check(nvgraphExtractSubgraphByVertex(handle, graph, subgraph, subvertices, numvertices));
            /*Count Trianngles*/
            trcount = 0;
    
            check(nvgraphTriangleCount(handle, subgraph, &trcount));
printf("/_: %ld\n", trcount);
            /*Fill Clustering Coefficient*/
            clustering[i] = (float)trcount / (numvertices - 1);
printf("Clust: %.6f\n", clustering[i]);
        }
    }
    
    /*Write the Shortest Path to a file*/
    results = fopen(argv[5], "w+");
    for (int i = 0; i <= n; i++){
        fprintf(results, "%.6f\n", clustering[i]);
    }
    fclose(results);

    free(CSC_input); free(subvertices); free(clustering);
    check(nvgraphDestroyGraphDescr(handle, graph));
    check(nvgraphDestroyGraphDescr(handle, subgraph));
    check(nvgraphDestroy(handle));
    return 0;
}