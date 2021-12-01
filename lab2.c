//Multiplicação matriz-matriz (considerando matrizes quadradas)
#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>

#include "timer.h"

float *mat1;
float *mat2;
float *saida;   //matriz de resultado sequencial
float *final;  //matriz de resultado concorrente 
int nthreads;     //número de Threads

typedef struct{
	int id; // identificador do elemento que a thread ira processar [linha, coluna]
	int dim; //dimensão das matrizes de entrada
}tArgs;


//função que as threads executarão

void* tarefa(void *arg){
	tArgs *args =(tArgs*) arg;
	for(int k=args->id;k<args->dim;k+=nthreads){
		for(int i=0;i<args->dim;i++){
			for(int j=0;j<args->dim;j++){
				final[k*args->dim+i]+=mat1[k*args->dim+j]*mat2[j*args->dim+i];
			}
		}
	}

	pthread_exit(NULL);
}

int verifica (int dim ){
	for(int i=0;i<dim; i++){
		for(int j=0;j<dim;j++){
			if(final[i*dim+j]!=saida[i*dim+j])	return 0;
		}
	}
	return 2;
}

//fluxo principal
int main(int argc, char * argv []){
	int dim; //dimensão das matrizes de entrada
	pthread_t *tid; //identificador das Threads no sistema
	tArgs *args;  //identificador locais das threads e dimensão

	double inicio, fim, delta; //para cálculo do tempo
	
	
	//leitura e armazenamento dos parâmetros de entrada 
	if(argc<3){
		printf("digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
		return 1;
	}
	dim= atoi(argv[1]);

	
	nthreads= atoi(argv[2]);
	if(nthreads > dim)	nthreads=dim;
	

	GET_TIME(inicio);

	//alocação de memoria para as estruturas de dados
	mat1= (float *) malloc (sizeof(float)*dim*dim);
	if(mat1== NULL) {printf("ERRO--malloc()\n"); return 2;}
	mat2= (float *) malloc (sizeof(float)*dim*dim);
	if(mat2== NULL) {printf("ERRO--malloc()\n"); return 2;}
	saida= (float *) malloc (sizeof(float)*dim*dim);
	if(saida== NULL) {printf("ERRO--malloc()\n"); return 2;}
	final= (float *) malloc (sizeof(float)*dim*dim);
	if(final== NULL) {printf("ERRO--malloc()\n"); return 2;}

	//inicialização das estruturas de dados de entrada e saída
	srand(time(NULL));
	for(int i=0; i<dim;i++){
		for(int j=0;j<dim;j++){
			mat1[i*dim+j]= rand() % 155;   //equivalente a mat1[i][j]
			mat2[i*dim+j]=rand() %155;     //números aleatórios
			saida[i*dim+j]=0; //inicializar com zero
			final[i*dim+j]=0; //inicializa com zero

		}
	}

	//multiplicação sequencial das matrizes 
	for(int k=0;k<dim;k++){
		for(int i=0;i<dim;i++){
			for(int j=0;j<dim;j++){
				saida[k*dim+i]+=mat1[k*dim+j] * mat2[j*dim+i];
			}
		}
	}
	GET_TIME(fim);
	delta=fim-inicio;
	printf("Tempo sequencial: %lf\n", delta);


	//multiplicação concorrente das matrizes

	GET_TIME(inicio);
	//alocação das estruturas 
	
	tid=(pthread_t*)malloc (sizeof(pthread_t)*nthreads);
	if(tid==NULL){puts("ERRO--malloc()");	return 2;}
	args=(tArgs*)malloc(sizeof(tArgs)*nthreads);
	if(args==NULL){puts("ERRO--malloc()");	return 2;}

	//criação das threads
	for(int i=0;i<nthreads;i++){
		(args+i)->id=i;
		(args+i)->dim= dim;
		if(pthread_create(tid+i,NULL,tarefa, (void*)(args+i))){
			puts("ERRO-- pthread_create()\n");	return 3;
		}		
	}

	//espera pelo termino das threads
	for(int i=0;i<nthreads;i++){
		pthread_join(*(tid+i),NULL);
	}
	GET_TIME(fim);
	delta=fim-inicio;
	printf("Tempo concorrente: %lf\n", delta);
	

	if(verifica(dim)){
		puts("correto!\n");
	}
	else{puts("vish, deu ruim!!!!!\n");}

	//liberação de memoria
	free(mat1);
	free(mat2);
	free(saida);
	free(args);
	free(tid);


	return 0; 
}
