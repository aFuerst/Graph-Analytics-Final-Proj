#include <nvgraph.h>
#include <stdio.h> 

void check(nvgraphStatus_t status) {
    if (status != NVGRAPH_STATUS_SUCCESS) {
        printf("ERROR : %d\n",status);
        exit(0);
    }
}
int main(int argc, char *argv[]) {
    /*Check Errors*/
    if(argc != 7){
        printf("Arguments should be in following order:\n");
        printf("<# Vertices> <# Edges> <Weights txt File> <Offset txt File> <Indices txt File> <Save Name>\n");
        return 1;
    }

    const size_t vertex_numsets = 1, edge_numsets = 1;
    size_t n, nnz;
    int i;
    float *sssp_1_h;
    void** vertex_dim;

    /*Assign Variables*/
    n = atoi(argv[1]);
    nnz = atoi(argv[2]);

    /*Open Files*/
    FILE *weights, *offsets, *indices, *results;
    weights = fopen(argv[3], "r");
    offsets = fopen(argv[4], "r");
    indices = fopen(argv[5], "r");
    
    float weights_h[nnz];
    float destination_offsets[n+1];
    int destination_offsets_h[n+1];
    float source_indices[nnz];
    int source_indices_h[nnz];
    int are_the_vertex_not_checked[n];
    int connected_components[n];
    
    for (i = 0; i < nnz; i++) {
        /*Assign Weights*/
        fscanf(weights, "%e", &weights_h[i]);
        
        /*Assign Indices*/
        fscanf(indices, "%e", &source_indices[i]);
        source_indices_h[i] = source_indices[i];
    }

    for (i = 0; i < n; i++) {
        /*Assign Indices*/
        fscanf(offsets, "%e", &destination_offsets[i]);
        destination_offsets_h[i] = destination_offsets[i];
    }
    
    fclose(weights);
    fclose(indices);
    fclose(offsets);

    destination_offsets_h[n] = nnz;

    /*Initialize are_the_vertex_not_checked and connected_components*/
    for (i = 0; i < n; i++) {
        are_the_vertex_not_checked[i] = 0;
        connected_components[i] = 0;
    }

    // nvgraph variables
    nvgraphStatus_t status; nvgraphHandle_t handle;
    nvgraphGraphDescr_t graph;
    nvgraphCSCTopology32I_t CSC_input;
    cudaDataType_t edge_dimT = CUDA_R_32F;
    cudaDataType_t* vertex_dimT;

    // Init host data
    sssp_1_h = (float*)malloc(n*sizeof(float));
    vertex_dim = (void**)malloc(vertex_numsets*sizeof(void*));
    vertex_dimT = (cudaDataType_t*)malloc(vertex_numsets*sizeof(cudaDataType_t));
    CSC_input = (nvgraphCSCTopology32I_t) malloc(sizeof(struct nvgraphCSCTopology32I_st));
    vertex_dim[0]= (void*)sssp_1_h; vertex_dimT[0] = CUDA_R_32F;

    check(nvgraphCreate(&handle));
    check(nvgraphCreateGraphDescr (handle, &graph));
    CSC_input->nvertices = n; 
    CSC_input->nedges = nnz;
    CSC_input->destination_offsets = destination_offsets_h;
    CSC_input->source_indices = source_indices_h;

    // Set graph connectivity and properties (tranfers)
    check(nvgraphSetGraphStructure(handle, graph, (void*)CSC_input, NVGRAPH_CSC_32));
    check(nvgraphAllocateVertexData(handle, graph, vertex_numsets, vertex_dimT));
    check(nvgraphAllocateEdgeData (handle, graph, edge_numsets, &edge_dimT));
    check(nvgraphSetEdgeData(handle, graph, (void*)weights_h, 0));

    // Solve
    int source_vert = 0;
    check(nvgraphSssp(handle, graph, 0, &source_vert, 0));

    // Get and print result
    check(nvgraphGetVertexData(handle, graph, (void*)sssp_1_h, 0));

    /*Fill are_the_vertex_not_checked and connected_components for the first time*/
    source_vert = -1;
    int location = 0;
    for (i = 0; i < n; i++) {
        if(sssp_1_h[i] > 3402823466) {
            if(source_vert == -1) source_vert = i;
            are_the_vertex_not_checked[i] = 1;
        }
        else connected_components[location]++;
    }
    
    while(1){
        /*Calculte SSSP*/
        check(nvgraphSssp(handle, graph, 0, &source_vert, 0));
        check(nvgraphGetVertexData(handle, graph, (void*)sssp_1_h, 0));

        location++;
        source_vert = -1;

        /*Calculate Connected Component Count*/
        for (i = 0; i < n; i++) {
            if(sssp_1_h[i] < 3402823466) {
                connected_components[location]++;
                are_the_vertex_not_checked[i] = 0;
            }
        }

        /*Find next source_vert*/
        for (i = 0; i < n; i++) {
            if(are_the_vertex_not_checked[i] == 1) {
                if(source_vert == -1) source_vert = i;
                break;
            }
        }

        if(source_vert == -1) break;
    }

    /*Write the Shortest Path to a file*/
    results = fopen(argv[6], "w+");
    for (int i = 0; i <= location; i++){
        fprintf(results, "%d\n", connected_components[i]);
    }
    fclose(results);

    //Clean 
    free(sssp_1_h); free(vertex_dim);
    free(vertex_dimT); free(CSC_input);
    check(nvgraphDestroyGraphDescr(handle, graph));
    check(nvgraphDestroy(handle));
    return 0;
}