#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <assert.h> 
#define MAXVECT 10
#define MAXPROC 5
#define imprimirvector(v,n) printf("Mi vista (%d) del vector\n",myrank); for (i = 0 ; i < n; i++) printf("[%d: %d] ",myrank,v[i]); printf("\n");


//Vector de numeros aleatorios
float *vector_random(int sizeVector){
	float *numero_alea = (float*)malloc(sizeof(float)*sizeVector);
	assert(numero_alea !=NULL);
	int i;
	for(i=0;i<sizeVector;i++){ numero_alea[i]=rand()%100+1;}
	return numero_alea;
}


// Calcula el promedio de un arreglo de flotantes
float compute_avg(float *array, int num_elements) {
  float sum = 0.f;
  int i;
  for (i = 0; i < num_elements; i++) {
    sum += array[i];
  }
  return sum / num_elements;
}



int main(int argc, char** argv)
{
	
   if(argc!=2){
	fprintf(stderr, "Usage: MPI_promedio Size_Element \n");
	exit(1);
}	
    int Size_Elements = atoi(argv[1]);
    int myrank, worldsize;
    int i;
    int root;
    float vector[MAXVECT];
    char hostname[HOST_NAME_MAX + 1];
    int num_elements_each_proce;
    int num,num1;
   int Longitud;
//    float *numero_alea; 

//  int numero_por_cada_proceso = atoi(argv[1]);
//	printf("procesos: %d\n",numero_por_cada_proceso);

    gethostname(hostname, HOST_NAME_MAX + 1);
    srand(time(NULL));
    int choosen = (rand()%MAXPROC); 
    root = 0;
    // Escriba las lineas que permitan:
    //  (1) Inicializar el dominio MPI
    //  (2) Guardar en la variable 'worldsize' el numero de procesos 
    // involucrados en la ejecucion de esta app
    //  (3) Guardar el rango del proceso
    //
    // (1)
    MPI_Init(NULL, NULL);
    // (2)
    MPI_Comm_size(MPI_COMM_WORLD, &worldsize);
    // (3)
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	 
      //Pedir el tamano del arreglo y validar si es par
	//recorrer con un for  pidiendo los numeros del arreglo que se quieren caclcular
	//hasta el limite del tamaño del arreglo esto en el rank 0
	float *numero_alea=NULL;
	if(myrank == 0){
	
	
	numero_alea = vector_random(Size_Elements * worldsize);//vector del tamano seleccionado con numeros random
	
	for(i=0; i < Size_Elements;i++){
	
	printf("numeros alea: %f\n",numero_alea[i]);
	}	


}


   num_elements_each_proce = MAXVECT / worldsize;
   //se crea el buffer por cada procesos el cual recibe el vector de scatter
   // es decir vector que recibe cada procesos
    float *rec_vector = (float*) malloc(sizeof(float) * MAXVECT/worldsize);
  //raiz crea el arreglo
    if (myrank == root) {
     for (i = 0; i < MAXVECT; i++) {
       vector[i] = i;
     }

    

//  MPI_Scatter(vector, MAXVECT/worldsize, MPI_INT, rec_vector, MAXVECT/worldsize, MPI_INT, root, MPI_COMM_WORLD);

    }
	
    
    
   //Scatter manda un sub arragelo a cada procesos
   
   int size_each_proceso = Longitud/worldsize;
//   printf("size each process %d\n",Size_Elements);
    MPI_Scatter(numero_alea, Size_Elements/worldsize, MPI_INT, rec_vector, Size_Elements/worldsize, MPI_INT, root, MPI_COMM_WORLD);
   

//	printf(" rank: %d, vector: %f\n",myrank,rec_vector[0]);
  //      printf(" rank: %d, vector: %f\n",myrank,rec_vector[1]);
    //Metodo que calcula el promedio de los subs arreglos de cada procesos
      float sub_avg = compute_avg(rec_vector,Size_Elements/worldsize);
//	printf("Proceso %d, promedio:%f\n ",myrank,sub_avg);
		
 //el root instancia el vector de promedios que recibe de cada procesos en el
 //gather
   float *vect_gather;
   if(myrank == 0){
        vect_gather = (float*)malloc(sizeof(float) * worldsize);
  }

   //Metodo Gather que mande el promedio calculado de cada procesos de los subarrays recibidos
  //a raiz para calcular a partir de los promedios parciales el promedio total
       MPI_Gather(&sub_avg,1, MPI_FLOAT, vect_gather, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
 	//root calcula los promedio llegados en el buffer del gather
	if(myrank == 0){
	
	float total_pro = compute_avg(vect_gather,worldsize);
	printf("El promedio de todos los elementos es: %f\n",total_pro);
	
	printf("promedio de arreglo sin MPI");
	float prom = compute_avg(numero_alea,Size_Elements);
	printf(":%f\n",prom);




}

//    free(data);
    MPI_Finalize();
    
}

