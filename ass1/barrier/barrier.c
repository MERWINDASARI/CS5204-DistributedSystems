#include <stdio.h>
#include <mpi.h>
#include "mpi_iittp_barrier.h"

int main(int argc, char **argv)
{
        MPI_Init(&argc,&argv);
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        printf("Before the barrier %d\n", rank);
        fflush(stdout);
        mpi_iittp_barrier();
        printf("After the barrier %d\n", rank);
   
        MPI_Finalize();
        
        return 0;
}
