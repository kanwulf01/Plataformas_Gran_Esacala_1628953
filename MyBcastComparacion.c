#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <assert.h>

void My_bcast(void* data, int count, MPI_Datatype tipodato, int root, MPI_Comm communicator){

   int size, rank, dest, source, tag=1, i;
        int* inmsg;
        int* outmsg=data;
        MPI_Status Stat;

        MPI_Comm_size(communicator, &size);
        MPI_Comm_rank(communicator, &rank);

        if (rank == 0) {

       // printf("nodo %d, Mensaje enviado a Todos %d\n",rank,outmsg);
          for(i=0; i < size; i++){

                if(i != 0){
                MPI_Send(data, count, tipodato, i, 0, communicator);
                }
                }



        }else{

          MPI_Recv(data, count, tipodato, 0, 0, communicator, MPI_STATUS_IGNORE);
       //   printf("nodo %d, mensaje recibido de 0, data:%d \n",rank,inmsg);
        }



}//Finaliza funcion bcast manual

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: compare_bcast num_elements num_trials\n");
    exit(1);
  }

  int num_elements = atoi(argv[1]);
  int num_trials = atoi(argv[2]);

  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  double total_my_bcast_time = 0.0;
  double total_mpi_bcast_time = 0.0;
  int i;
  int* data = (int*)malloc(sizeof(int) * num_elements);
  assert(data != NULL);

  for (i = 0; i < num_trials; i++) {
    // Time my_bcast
    // Synchronize before starting timing
    MPI_Barrier(MPI_COMM_WORLD);
    total_my_bcast_time -= MPI_Wtime();
    My_bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
    // Synchronize again before obtaining final time
    MPI_Barrier(MPI_COMM_WORLD);
    total_my_bcast_time += MPI_Wtime();

    // Time MPI_Bcast
    MPI_Barrier(MPI_COMM_WORLD);
    total_mpi_bcast_time -= MPI_Wtime();
    MPI_Bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    total_mpi_bcast_time += MPI_Wtime();
  }

  // Print off timing information
  if (world_rank == 0) {
    printf("Data size = %d, Trials = %d\n", num_elements * (int)sizeof(int),
           num_trials);
    printf("Avg my_bcast time = %lf\n", total_my_bcast_time / num_trials);
    printf("Avg MPI_Bcast time = %lf\n", total_mpi_bcast_time / num_trials);
  }

  free(data);
  MPI_Finalize();
}

