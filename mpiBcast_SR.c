#include <stdio.h>
#include "mpi.h"

int main(int argc,char *argv[]){
        int size, rank, dest, source, count, tag=1, i;
        int inmsg;
        int  outmsg=100;
        MPI_Status Stat;

        MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if (rank == 0) {
           
        printf("nodo %d, Mensaje enviado a Todos %d\n",rank,outmsg);
	  for(i=0; i < size; i++){
		
		if(i != 0){
		MPI_Send(&outmsg, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
		}
		}
          
          
             
        }else{

          MPI_Recv(&inmsg, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &Stat);
	  printf("nodo %d, mensaje recibido de 0, data:%d \n",rank,inmsg);
        }

        MPI_Finalize();
}


