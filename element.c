#include <stdio.h>
#include <stdlib.h>


#include "lifeworld.h"
#include "mpi.h"

#define Master(rank) rank==0 ? 1:0
#define MAX_PROCESSES 10

void create_world(Pworld* p,int n,int m);
void init_world(Pworld * p,int seed);
void send_matrix(int n,int m);

int main (int argc, char* argv){
    MPI_Init(NULL,NULL);
    send_matrix(5,5);
    MPI_Finalize();
}



void create_world(Pworld* p,int n,int m){
    p=(Pworld *)malloc(sizeof(Pworld));
    p->rows=n;
    p->columns=m;
    p->matrix= (char**) malloc(sizeof(char*)*n);   
    for (int i=0;i<n;i++){
        p->matrix[i]=(char*) malloc(sizeof(char)*m);
    }
}

void init_world(Pworld * p,int seed){
    srand(seed);
	for (int i=0; i<p->rows; i++){
		for (int j=0; j<p->columns; j++) {
			p->matrix[i][j] = rand()%2 +'0';
		}
	}
}

void send_matrix(int n,int m){
    int world_size;
    int rank;
    int n_eachp;
    int sendcounts[MAX_PROCESSES];
    int displs[MAX_PROCESSES];
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int rest=n%world_size;
    int recv_count;
    Pworld * p;

    n_eachp=n/world_size;

    for(int i=0;i<world_size;i++){
        sendcounts[i]=n_eachp;
        if(rest>0){
            sendcounts[i]+=1;
            rest--;
        }
        displs[i]=i*n_eachp;
        if(rank==i){
            recv_count=sendcounts[i];
        }
    }

    if(Master(rank)){
        //consider to switching n to m, in this way we have less array and we can distribute better the portion of the matrices for each process 
        
        create_world(p,n,m);
        printf(p->columns);
        //init_world(p,5);
    }else{
        create_world(p,recv_count,m);
    }

        //MPI_Scatterv(p->matrix,sendcounts,displs,MPI_CHAR,p->matrix,recv_count,MPI_CHAR,0,MPI_COMM_WORLD);


        printf("Rank:%d received all!",rank);
}