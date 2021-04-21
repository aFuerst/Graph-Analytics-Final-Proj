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
    if(argc != 5){
        printf("Arguments should be in following order:\n");
        printf("<# Vertices> <# Edges> <Offset txt File> <Save Name>\n");
        return 1;
    }

    size_t n, nnz;
    int i;

    /*Assign Variables*/
    n = atoi(argv[1]);
    nnz = atoi(argv[2]);

    /*Open Files*/
    FILE *offsets, *results;
    offsets = fopen(argv[3], "r");
    
    float destination_offsets[n+1];
    int destination_offsets_h[n+1];
    int degree[n];

    for (i = 0; i < n; i++) {
        /*Assign Indices*/
        fscanf(offsets, "%e", &destination_offsets[i]);
        destination_offsets_h[i] = destination_offsets[i];
    }

    fclose(offsets);

    destination_offsets_h[n] = nnz;

    /*Calculate Degree*/
    for (i = 0; i < n; i++) {
        degree[i] = destination_offsets_h[i+1] - destination_offsets_h[i];
    }

    /*Write the Shortest Path to a file*/
    results = fopen(argv[4], "w+");
    for (int i = 0; i < n; i++){
        fprintf(results, "%d\n", degree[i]);
    }
    fclose(results);
    
    return 0;
}