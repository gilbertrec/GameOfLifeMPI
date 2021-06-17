#include <stdio.h>
#include <stdlib.h>


#include "lifeworld.h"
#include "mpi.h"

#define Master(rank) rank==0 ? 1:0
#define MAX_PROCESSES 10
#define N 50
#define M 50

Pworld* create_worldv2(int n,int m);
void init_world(Pworld * p,int seed);
void send_matrix(int n,int m);

int main (int argc, char* argv){
    MPI_Init(NULL,NULL);
    send_matrix(4,5);
    MPI_Finalize();
}


Pworld* create_worldv2(int n,int m){
    Pworld * p=(Pworld *)malloc(sizeof(Pworld));
    p->rows=n;
    p->columns=m;
    p->matrix= (char*) malloc(sizeof(char)*n*m);   
    return p;
}


//init_world permit to initialize a new world with a random seed
void init_world(Pworld * p,int seed){
    srand(seed);
    int c=0;
	for (int i=0; i<p->rows*p->columns; i++){
			p->matrix[i] = rand()%2 +'0';
            c++;
	}
}

//print_world permit to print the world on stdout
void print_world(Pworld * p){
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    printf("Stampa il rank %d con righe %d:\n",rank,p->rows);
    for(int i=0;i<p->rows*p->columns;i++){
        if(i%p->columns==0){
            printf("----");
            printf("\nRiga:%d of rank %d\n",i/p->columns,rank);
        }
        printf("rank%d:%c,",rank,p->matrix[i]);
    }
} 

/**Permit to send the communication matrix from master to its slave processes**/
void send_matrix(int n,int m){
    int world_size;
    int rank;
    int n_eachp;
    MPI_Status* status;
    int sendcounts[MAX_PROCESSES];
    int *displs=malloc(n*sizeof(int));
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int rest=n%world_size;
    int recv_count;
    Pworld * p;
    MPI_Datatype row;
    n_eachp=n/world_size;
    MPI_Type_contiguous(m,MPI_CHAR,&row);
    MPI_Type_commit(&row);
    if(Master(rank)){
        //consider to switching n to m, in this way we have less array and we can distribute better the portion of the matrices for each process 
        p=create_worldv2(n,m);
        init_world(p,0);
    }
    for(int i=0;i<world_size;i++){
            sendcounts[i]=n_eachp;
            if(rest>0){
                sendcounts[i]+=1;
                rest--;
            }
            if(rank==i &&rank!=0){ //if rank==i but not master
                recv_count=sendcounts[i];
            }
            if(i>0)
                displs[i]=sendcounts[i-1]+displs[i-1];
            else{
                displs[i]=0;
            }
    }
    if(!Master(rank)){
        p=create_worldv2(recv_count,m);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatterv(p->matrix,sendcounts,displs,row,p->matrix,recv_count,row,0,MPI_COMM_WORLD);    
    printf("Processore %d",rank);
    print_world(p);
    MPI_Type_free(&row);
}



//this is for putting also the border lines of communications
void send_matrixv2(int n,int m){
    int world_size;
    int rank;
    int n_eachp;
    MPI_Status* status;
    int sendcounts[MAX_PROCESSES];
    int *displs=malloc(n*sizeof(int));
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int rest=n%world_size;
    int recv_count;
    Pworld * p;
    MPI_Datatype row;
    n_eachp=n/world_size;
    MPI_Type_contiguous(m,MPI_CHAR,&row);
    MPI_Type_commit(&row);
    if(Master(rank)){
        //consider to switching n to m, in this way we have less array and we can distribute better the portion of the matrices for each process 
        p=create_worldv2(n,m);
        init_world(p,0);
    }
    for(int i=0;i<world_size;i++){
        sendcounts[i]=n_eachp;
        if(rest>0){
            sendcounts[i]+=1;
            rest--;
        }
        //now just add the border rows,
        sendcounts[i]+=2;
        if(rank==i &&rank!=0){ //if rank==i but not master
            recv_count=sendcounts[i];
        }
        if(i>0)
            displs[i]=sendcounts[i-1]+displs[i-1]-1;
        else{
            displs[i]=0;
        }
    }
    if(!Master(rank)){
        p=create_worldv2(recv_count,m);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatterv(p->matrix,sendcounts,displs,row,p->matrix,recv_count,row,0,MPI_COMM_WORLD);    
    printf("Processore %d",rank);
    print_world(p);
    MPI_Type_free(&row);
}



/** Permit to send the communication row to the process above */
MPI_Request send_up(Pworld * p,int rank,MPI_Datatype row){
    MPI_Request r;
    MPI_Isend(p->matrix[0],1,row,rank-1,14,MPI_COMM_WORLD,r);
    return r;
}

/** Permit to send the communication row to the process below**/
MPI_Request send_down(Pworld * p,int rank,MPI_Datatype row){
    MPI_Request r;
    MPI_Isend(p->matrix[p->rows],1,row,rank+1,14,MPI_COMM_WORLD,r);
    return r;
}

/** Permit to receive the communication row from the process below**/
MPI_Request receive_down(Pworld * p,int rank,MPI_Datatype row){
    MPI_Request r;
    MPI_Irecv(p->matrix[p->rows],1,row,rank+1,14,MPI_COMM_WORLD,r);
    return r;
}

/** Permit to receive the communication row from the process above**/
MPI_Request receive_up(Pworld * p,int rank,MPI_Datatype row){
    MPI_Request r;
    MPI_Irecv(p->matrix[0],1,row,rank-1,14,MPI_COMM_WORLD,r);
    return r;
}