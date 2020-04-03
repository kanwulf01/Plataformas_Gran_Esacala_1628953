#include <stdio.h>
#include "mpi.h"

int main(int argc,char *argv[]){
        int size, rank, dest, source, count, tag=1, i;
        int inmsg[10];
        int  outmsg=100;
	int vector[2];
	int vector2[2];
	int vector3[2];
	int y = 0;
	int x = 0;
	int divProcData;
        MPI_Status Stat;

        MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if (rank == 0) {
	for(i=0; i < 10;i++){vector[i]=i;}
	divProcData = 10/size;
	//printf("vector %d",divProcData);
       // printf("nodo %d, Mensaje enviado a Todos \n",rank);
          for(i=0; i < size; i++){

                if(i != 0){
		// printf("div%d, contador: %d\n",divProcData,y);
		vector2[y] = vector[divProcData];
		y++;
		divProcData++;
		 // printf("div%d, contador: %d\n",divProcData,y);
		vector2[y] = vector[divProcData];
                MPI_Send(&vector2, 2, MPI_INT, i, tag, MPI_COMM_WORLD);
		y=0;
		divProcData++;
                }
                }



        }else{

          MPI_Recv(&vector2, 2, MPI_INT, 0, tag, MPI_COMM_WORLD, &Stat);
	 for(i=0; i<10/size; i++){ 
		vector[i] = i;
		printf("nodo 0, tiene vector:%d, \n",vector[i]);
			
		}
		for(i=0; i<2; i++){
          printf("nodo %d, mensaje recibido de 0, tiene vector:%d, \n",rank,vector2[i]);
			}
        }

        MPI_Finalize();
}

