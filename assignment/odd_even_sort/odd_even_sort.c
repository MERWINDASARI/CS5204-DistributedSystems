#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int **createdata(int rank,int n,int world_size,int r,int **p){
        if(rank>n/2) p=NULL;
        else{
         
                for (int i=0; i<r; i++){
                       p[i] = (int *)malloc(2 * sizeof(int));
                }
             
             
             for (int i = 0; i <r; i++){
                srand(rank+i);
                   for (int j = 0; j < 2; j++) 
                        p[i][j] = rand()%100;
             }
          }
}
void sort_phase(int phase,int **p,int r,int rank,int world_size){
        if(phase==1){
                for (int i = 0; i <  r; i++){
                        if(p[i][0]>p[i][1]){
                                int temp = p[i][0];
                                p[i][0]=p[i][1];
                                p[i][1]=temp;
                        }
                }
        }
        else{
                for(int i=0;i<r;i++){
                        int temp;
                        if(rank+1<world_size){
                                MPI_Send(&p[i][1], 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
                                printf("Sent %d from %d to %d\n", p[i][1], rank, rank+1);
                                MPI_Recv(&p[i][1], 1, MPI_INT, rank+1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                                printf("Recv %d from %d by %d\n", p[i][1], rank+1, rank);
                        }
                        if(rank!=0){
                                MPI_Recv(&temp, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                                printf("Recv %d from %d by %d\n", temp, rank-1, rank);
                        
                                if(temp<p[i][0]) {
                                
                                MPI_Send(&temp, 1, MPI_INT, rank-1, 1, MPI_COMM_WORLD);
                                printf("Sent %d from %d to %d\n", temp, rank, rank-1);
                                        }
                                else {
                                int t=p[i][0];
                                MPI_Send(&t, 1, MPI_INT, rank-1, 1, MPI_COMM_WORLD);
                                printf("Sent %d from %d to %d\n", p[i][0], rank-1, rank-1);
                                p[i][0]=temp;
                                }
                        }
                        
                        
                    
                }        
        }
}
void print_array(int **p,int r,int rank){
        for (int i = 0; i <r; i++) {
                        printf("rank :%d - %d %d \n ",rank,p[i][0],p[i][1]);
                 printf("\n");
             }
}
int main(int argc, char **argv)
{
        int n = argc == 2 ? atoi(argv[1]) : 10;
        MPI_Init(&argc,&argv);
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        int r=0;
        int temp=2*rank;
        while(temp<n){
                       r++;
                       temp+=2*world_size;
                }
        int *p[r];
        createdata(rank,n,world_size,r,p);
        //printf("rank : %d\n",rank);
        for (int i = 0; i <r; i++) {
                   for (int j = 0; j < 2; j++) 
                        printf("rank :%d - %d\n ",rank,p[i][j]);
                 printf("\n");
             }
        for(int i = 0; i < n; i++) {
                sort_phase(1, p,r,rank,world_size);
                //print_array(p,r,rank);
                MPI_Barrier(MPI_COMM_WORLD);
                sort_phase(0, p,r,rank,world_size);
                MPI_Barrier(MPI_COMM_WORLD);
        }
        print_array(p,r,rank);
        MPI_Finalize();
        
        return 0;
}
