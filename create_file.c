#include <stdio.h>
#include <stdlib.h>


int main (int argc, char** argv){
    FILE* fp=fopen("strong_results_little.csv","w");
    fprintf(fp,"NP,N,M,G,Time\n");
    FILE* fp_weak=fopen("weak_results_little.csv","w");
    fprintf(fp_weak,"NP,N,M,G,Time\n");
    fclose(fp);
    fclose(fp_weak);

    fp=fopen("strong_results_medium.csv","w");
    fprintf(fp,"NP,N,M,G,Time\n");
    fp_weak=fopen("weak_results_medium.csv","w");
    fprintf(fp_weak,"NP,N,M,G,Time\n");
    fclose(fp);
    fclose(fp_weak);

    fp=fopen("strong_results_big.csv","w");
    fprintf(fp,"NP,N,M,G,Time\n");
    fp_weak=fopen("weak_results_big.csv","w");
    fprintf(fp_weak,"NP,N,M,G,Time\n");
    fclose(fp);
    fclose(fp_weak);
}