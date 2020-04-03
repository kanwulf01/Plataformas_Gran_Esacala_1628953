#include <stdio.h>
#include "mpi.h"

int main(int argc,char *argv[]){
        int size, rank, dest, source, count, tag=1, i,p;
        int inmsg[10];
        int  outmsg=100;
        int vector[10];
        int vector2[2];
        int vector3[2];
        int y = 0;
        int x = 0;
        int divProcData;
        MPI_Status Stat;

        MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if (rank != 0) {
        for(i=0; i < 10/size;i++){vector2[i]= (10/size)*rank+i;}
        MPI_Send(&vector2, 2, MPI_INT, 0, tag, MPI_COMM_WORLD);
 	printf("nodo %d, mensaje enviado a 0\n",rank);
        }else{

//          MPI_Recv(&vector2, 2, MPI_INT, 0, tag, MPI_COMM_WORLD, &Stat);
                
	int j = 2;
	int z = 0;
	for(i=0; i< 10/size; i++){vector[i] = (10/size)*rank + i;} //vector con valores {0,1} que tiene nodo 0
	for(i=1; i<size; i++){
	  MPI_Recv(&vector2, 2, MPI_INT, i, tag, MPI_COMM_WORLD, &Stat); //recibe el vector de 2 posicion por cada iteracion
		for(p = 0; p < 10/size; p++){
		printf("nodo %d, vector enviado de %d data:%d\n",rank,i,vector2[p]);  
		vector[j] =  vector2[p];//guardo en el arreglo global del rank 0 los arreglo que traigo por cada iteracion del for de afuera
		j++;
	
		}
                        }
	for(i=0; i < 10; i++){
	printf("Arreglo completo de rank:%d valor: %d\n",rank,vector[i]);		
}
        }

	MPI_Finalize();
}

