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
int main(int argc, char *argv[]) {
    /*Check Errors*/
    if(argc != 4){
        printf("Arguments should be in following order:\n");
        printf("<# Vertices> <# Edges> <Offset txt File>\n");
        return 1;
    }

    size_t n, nnz;
    int i, j;

    /*Declare time based variables*/
    struct timeval tv1, tv2;
    struct timezone tz;
    long int total_time = 0;

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
    int *degree_total = (int*)malloc(n*sizeof(int));
    for (i = 0; i < n; i++) {
        degree_total[i] = 0;
    }

    int max_degree = 0;

    /*Iterate 100 time for averaging*/
    gettimeofday(&tv1,&tz); // Get starting time
    for (j = 0; j < 100; j++) {
        for (i = 0; i < n; i++) {
            degree[i] = destination_offsets_h[i+1] - destination_offsets_h[i];
            if(max_degree < degree[i]) max_degree = degree[i];
            degree_total[degree[i]]++;
        }
    }
    gettimeofday(&tv2,&tz); // Get ending time
    /*Calculate time taken in microseconds*/
    total_time = (tv2.tv_sec-tv1.tv_sec)*1000000 + (tv2.tv_usec-tv1.tv_usec);
    printf("Average Time: %ld\n", total_time/100);
    
    return 0;
}