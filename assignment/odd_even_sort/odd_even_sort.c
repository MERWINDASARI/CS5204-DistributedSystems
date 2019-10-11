#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int **createdata(int rank,int n,int world_size,int r,int **p){
        if(rank>n/2) p=NULL;
        else{
         
                for (int i=0; i<r; i++){
                       p[i] = (int *)malloc(2 * sizeof(int));
                }
             
             srand(rank+1);
             for (int i = 0; i <r; i++) 
                   for (int j = 0; j < 2; j++) 
                        p[i][j] = rand()%100;;
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
                        if(rank+1<world_size)
                        MPI_Send(&p[i][1], 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
                        if(rank!=0)
                        MPI_Recv(&temp, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        if(rank>0){
                                if(temp<p[i][0])
                                MPI_Send(&temp, 1, MPI_INT, rank-1, 1, MPI_COMM_WORLD);
                                else
                                MPI_Send(&p[i][0], 1, MPI_INT, rank-1, 1, MPI_COMM_WORLD);
                        }
                    
                }        
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
        int temp=rank;
        while(temp<n){
                       temp+=2*world_size;
                       r++;
                }
        int *p[r];
        createdata(rank,n,world_size,r,p);
        for(int i = 0; i < n; i++) {
                sort_phase(1, p,r,rank,world_size);
                //print_array(&p, "Array elements after odd phase:");
                MPI_Barrier(MPI_COMM_WORLD);
                sort_phase(0, p,r,rank,world_size);
                //print_array(&p, "Array elements after even phase:");
                MPI_Barrier(MPI_COMM_WORLD);
        }
        MPI_Finalize();
        
        return 0;
}
