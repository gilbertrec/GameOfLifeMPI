#include <stdio.h>
#include <stdlib.h>


int main (int argc, char** argv){
    FILE* fp=fopen("strong_results.csv","w");
    fprintf(fp,"NP,N,M,G,Time\n");
    FILE* fp_weak=fopen("weak_results.csv","w");
    fprintf(fp_weak,"NP,N,M,G,Time\n");
}