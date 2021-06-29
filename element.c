#include <stdio.h>
#include <stdlib.h>

#include "lifeworld.h"
#include "mpi.h"

#define Master(rank) rank==0 ? 1:0
#define MAX_PROCESSES 10
#define N 50
#define M 50

 int sendcounts[MAX_PROCESSES];
 int *displs;

Pworld* create_world(int n,int m);
void init_world(Pworld * p,int seed);
Pworld* scatter_matrix(int n,int m);
void print_world(Pworld * p);
void print_all_matrix(Pworld * p);
void print_row(Pworld * p, int i);
void print_ghost_world(Pworld * p);
Pworld* compute(Pworld * p,int rank,int world_size);

MPI_Request send_up(Pworld * p,int rank,MPI_Datatype *row,int p_size);
MPI_Request send_down(Pworld * p,int rank,MPI_Datatype *row,int p_size);
MPI_Request * receive_down(Pworld * p,int rank,MPI_Datatype *row,int p_size);
MPI_Request * receive_up(Pworld * p,int rank,MPI_Datatype *row,int p_size);

int isAlive(char cell);
int count_internalrow(Pworld * p, int n, int m);
int count_externalrow(Pworld * p,int n,int m);
char alive_conditioner(char cell,int count);

int main (int argc, char* argv){
    int rank;
    int world_size;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    int generations=1;
    int start_time=MPI_Wtime();
    Pworld* p = scatter_matrix(4,5);
    Pworld* updated_p;
    MPI_Datatype row;
    MPI_Type_contiguous(5,MPI_CHAR,&row);
    MPI_Type_commit(&row);
    updated_p=p;
    for(int i=0;i<generations;i++){
        updated_p = compute(updated_p,rank,world_size);
    }
    
    Pworld* final_p;
    char * pointer_matrix_to_start;

    if(Master(rank)){
        final_p=create_world(4,5);
        pointer_matrix_to_start=&final_p->matrix[0];
    } 
    
    MPI_Barrier(MPI_COMM_WORLD);
    if(Master(rank)){
        printf("This is sendcounts:");
        for(int i=0;i<world_size;i++){
            printf("Sendcounts for rank %d is %d",i,sendcounts[i]);
            printf("Displs for rank %d is %d",i,displs[i]);
        }
        fflush(stdout);
    }
    MPI_Gatherv(&(updated_p->matrix[0]),sendcounts[rank],row,pointer_matrix_to_start,sendcounts,displs,row,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    
    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);
    if(Master(rank)){
        printf("Final Solution\n:");
        print_all_matrix(final_p);
        printf("Questa è la riga 1:"); 
        print_row(updated_p,0);
    }
    MPI_Finalize();
}

/**Permit to send the communication matrix from master to its slave processes**/
Pworld* scatter_matrix(int n,int m){
    int world_size;
    int rank;
    int n_eachp;
    MPI_Status* status;
    displs=malloc(n*sizeof(int));
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
        p=create_world(n,m);
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
        p=create_world(recv_count,m);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if(Master(rank)){
        printf("This is sendcounts:");
        for(int i=0;i<world_size;i++){
            printf("SendCounts for rank %d is %d",i,sendcounts[i]);
            printf("Displs for rank %d is %d",i,displs[i]);
        }
        printf("Before scatter -------------");
    }
    MPI_Scatterv(p->matrix,sendcounts,displs,row,p->matrix,recv_count,row,0,MPI_COMM_WORLD);    
    if(Master(rank)){
        printf("After scatter -------------");
        printf("This is sendcounts:");
        for(int i=0;i<world_size;i++){
            printf("SendCounts for rank %d is %d",i,sendcounts[i]);
            printf("Displs for rank %d is %d",i,displs[i]);
        }
        
    }

    MPI_Barrier(MPI_COMM_WORLD);    
    if(Master(rank)){
        p->rows=sendcounts[0];
        p->matrix=realloc(p->matrix,sizeof(char)*p->rows*p->columns);
    }
    //MPI_Type_free(&row);
    return p;
}


//Compute the generation, returning a new updated world
Pworld* compute(Pworld * p,int rank,int world_size){
    MPI_Datatype row;
    MPI_Type_contiguous(5,MPI_CHAR,&row);
    MPI_Type_commit(&row);
    MPI_Request request_sendtop;
    MPI_Request request_sendbottom;
    int received_top;
    int received_bottom;
    MPI_Request * request_receivetop;
    MPI_Request * request_receivebottom;
    MPI_Status * status_receivetop;
    MPI_Status * status_receivebottom;

    Pworld * updated_world;
    request_sendtop=send_up(p,rank,&row,world_size);
    request_sendbottom=send_down(p,rank,&row,world_size);
    
    Pworld * bottom_ghostrow;
    Pworld * top_ghostrow;

    top_ghostrow = create_world(1,p->columns);
    
    bottom_ghostrow = create_world(1,p->columns);
    
    updated_world = create_world(p->rows,p->columns);
    
    //if there are more than two rows in the world, it means that there are rows that doesn't need ghosts rows in order to be computed
    if(p->rows>2){
        for(int i=1;i<p->rows-1;i++){
            for(int j=0;j<p->columns;j++){
                printf("OOOOOH È MAGGIORE DI 2!!!");
                //starting to count the nearest cells
                int count=0;

                //count alive cells in upper row
                count+=count_externalrow(p,i-1,j);
                
                //count alive cells in current row (need to not consider the current cell)
                count+=count_internalrow(p,i,j);

                //count alive cells in lower row
                count+=count_externalrow(p,i+1,j);
                
                //update cell
                updated_world->matrix[(p->columns*i)+j] = alive_conditioner(p->matrix[(p->columns*i)+j],count);
                printf("%d",count);
            }
        }
    }
    if(world_size>2){
        request_receivetop=receive_up(top_ghostrow,rank,&row,world_size);
        request_receivebottom=receive_down(bottom_ghostrow,rank,&row,world_size);
    }else{
        request_receivetop=receive_up(bottom_ghostrow,rank,&row,world_size);
        request_receivebottom=receive_down(top_ghostrow,rank,&row,world_size);
    }

    //trying to receive the two ghost_rows
    if(Master(rank)){
        //MPI_Test(request_receivebottom,&received_bottom,status_receivebottom);
        //printf("Test for master:%d",received_bottom);
        fflush(stdout);
        printf("These are ");
        print_ghost_world(top_ghostrow);
        print_ghost_world(bottom_ghostrow);
    }

    for(int j=0;j<p->columns;j++){
        //upper-border row count

        //starting to count for upper-border row
        int count=0;
        //count alive cells in upper ghost-row
        count+=count_externalrow(top_ghostrow,0,j);
        if(j==0&&rank==2){
            printf("Questo è il count dell'elemento che cerchi dopo la top %d",count);
        }
        //count alive cells in current row (need to not consider the current cell)
        count+=count_internalrow(p,0,j);
         if(j==0&&rank==2){
            printf("Questo è il count dell'elemento che cerchi dopo l'internal %d",count);
        }
        //count alive cells in lower internal row
        if(p->rows>1){
            count+=count_externalrow(p,1,j);
        }else{
            count+=count_externalrow(bottom_ghostrow,0,j);    
        }
        
        if(j==0&&rank==2){
            printf("Questo è il count dell'elemento che cerchi %d",count);
        }
        //update cell
        updated_world->matrix[(p->columns*0)+j] = alive_conditioner(p->matrix[(p->columns*0)+j],count);
        //lower-border row count

        //starting to count for upper-border row
        count=0;

        //count alive cells in upper internal row
        if(p->rows>1){
            count+=count_externalrow(p,p->rows-2,j);
            count+=count_internalrow(p,p->rows-1,j);
            count+=count_externalrow(bottom_ghostrow,0,j);
            updated_world->matrix[(p->columns*(p->rows-1))+j] = alive_conditioner(p->matrix[(p->columns*(p->rows-1))+j],count);
        }
        //count alive cells in current row (need to not consider the current cell)

        //count alive cells in lower ghost-row
        
        //update cell
        if(Master(rank))
            printf("quindi count è %d\n",count);
    }
    //Compute upper-border row and lower-border row
    if(Master(rank)){
        printf("This is updated world for master:");
        print_all_matrix(updated_world);
        printf("ENdWOrld"); 
    }
    return updated_world;
}

char alive_conditioner(char cell,int count){
    if(isAlive(cell)){
        if(count<2||count>3){
            return '0';
        }else{
            return '1';
        }
    }else{
        if(count==3){
            return '1';
        }else{
            return '0';
        }
    }
}

//permit to count the entire border row of a cell
int count_externalrow(Pworld * p,int n,int m){
    int count=0;
    for(int j=m-1;j<m+2;j++){
        if(j<0){
            //printf("\nAnalizzo l'elemento %d per %d,%d %c\n",j,n,m,(p->matrix)[(p->columns*n)+(p->columns-1)]);
            count += isAlive((p->matrix)[(p->columns*n)+p->columns-1]);    
        }else{
            if(j>p->columns-1){
              //  printf("\nAnalizzo l'elemento %d per %d,%d %c\n",j,n,m,(p->matrix)[(p->columns*n)]);
                count += isAlive((p->matrix)[(p->columns*n)]);    
            }else{
                //printf("\nAnalizzo l'elemento %d per %d,%d %c\n",j,n,m,(p->matrix)[(p->columns*n)+j]);
                count += isAlive((p->matrix)[(p->columns*n)+j]);
            }
        }
        //count += isAlive((p->matrix)[(p->columns*n)+j]);
    }
    return count;
}

//permit to count the center row of a cell, so excluding the current cell
int count_internalrow(Pworld * p, int n, int m){
    int count=0;
  //  print_row(p,n);
    
    //valuto il vicino sinistro
    if(m==0){
        //printf("\nAnalizzo l'elemento sinistro per %d,%d %c\n",n,m,(p->matrix)[(p->columns*n)+(p->columns-1)]);
        count += isAlive((p->matrix)[(p->columns*n)+(p->columns-1)]);
    }else{
        
        //printf("\nAnalizzo l'elemento sinistro per %d,%d %c\n",n,m,(p->matrix)[(p->columns*n)+(m-1)]);
        count += isAlive((p->matrix)[(p->columns*n)+(m-1)]);
    }

    //valuto il vicino destro
    if(p->columns-1==m){
        //printf("\nAnalizzo l'elemento destro per %d,%d %c\n",n,m,(p->matrix)[(p->columns*n)+(p->columns-1)]);
        
        count += isAlive((p->matrix)[(p->columns*n)]);    
    }else{
        //printf("\nAnalizzo l'elemento destro per %d,%d %c\n",n,m,(p->matrix)[(p->columns*n)+(m+1)]);
        count += isAlive((p->matrix)[(p->columns*n)+(m+1)]);
    }
    return count;
}

int isAlive(char cell){
    int result;
    if(cell=='0'){
        result=0;
    }else{
        result=1;
    }
    //printf("%c è uguale a 0? %d\n",cell,result);
    return result;
}

/** Permit to send the communication row to the process above */
MPI_Request send_up(Pworld * p,int rank,MPI_Datatype* row,int p_size){
   int rank_tosend;
   if(Master(rank)){
       rank_tosend=p_size-1;
   }else{
       rank_tosend=rank-1;
   }
   //printf("Rank %d for %d",rank_tosend,rank);
   fflush(stdout);
   MPI_Request r;
   MPI_Isend(&(p->matrix[0]),1,*row,rank_tosend,14,MPI_COMM_WORLD,&r);
   return r;
}

/** Permit to send the communication row to the process below**/
MPI_Request send_down(Pworld * p,int rank,MPI_Datatype* row,int p_size){
    //printf("Send Down, Rank %d for %d e %d",rank+1%p_size,rank,4+1%p_size);
    fflush(stdout);
    MPI_Request r;
    if(Master(rank)){
    //    printf("I'm master and i'm sending row %d,\n",p->rows-1);
    }
    MPI_Isend(&(p->matrix[(p->columns)*(p->rows-1)]),1,*row,(rank+1)%p_size,14,MPI_COMM_WORLD,&r);
    return r;
}

/** Permit to receive the communication row from the process above**/
MPI_Request * receive_up(Pworld * p,int rank,MPI_Datatype* row,int p_size){
    int rank_toreceive;
    if(Master(rank)){
        rank_toreceive=p_size-1;
    }else{
        rank_toreceive=rank-1;
    }
    MPI_Request * r;
    MPI_Status s;
    MPI_Recv(&p->matrix[0],1,*row,rank_toreceive,14,MPI_COMM_WORLD,&s);
    return r;
}


/** Permit to receive the communication row from the process below**/
MPI_Request * receive_down(Pworld * p,int rank,MPI_Datatype* row,int p_size){
    int rank_tosend;
    if(Master(rank)){
        rank_tosend=p_size-1;
    }else{
        rank_tosend=rank-1;
    }
    MPI_Request * r;
    MPI_Status s;
    MPI_Recv(&p->matrix[0],1,*row,(rank+1)%p_size,14,MPI_COMM_WORLD,&s);
    return r;
}


Pworld* create_world(int n,int m){
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

//print_world permit to print a single row of the world on stdout
void print_row(Pworld * p, int i){
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    printf("Stampa la riga %d del rank %d con righe %d:\n",i,rank,p->rows);
    for(int j=0;j<p->columns;j++){
        printf("rank%d:%c,",rank,p->matrix[(i*p->columns)+j]);
    }
}

//print_world permit to print the world on stdout
void print_ghost_world(Pworld * p){
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    printf("Stampa il rank %d con righe %d:\n",rank,p->rows);
    for(int i=0;i<p->rows*p->columns;i++){
        if(i%p->columns==0){
            printf("----");
            printf("\nRiga:%d of rank %d\n",i/p->columns,rank);
        }
        printf("rank_ghost%d:%c,",rank,p->matrix[i]);
    }
}
//print_world permit to print directly all the world with values
void print_all_matrix(Pworld * p){
    for(int i=0;i<p->rows*p->columns;i++){
        if(i%p->columns==0){
            printf("----");
            printf("\n");
        }
        printf("%c,",p->matrix[i]);
    }
    printf("\n");
}