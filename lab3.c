// verifica quantos elementos existem de um dado intervalo
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

int nthreads;    //numero de threads
long long int N; //numero de elementos
float *vetor; 
float inferior=-0; //valor do limiar inferior
float superior=0; //valor do limiar superior


//fluxo das threads
void * tarefa (void * arg){
	long int id = (long int) arg; //identificador da thread
	int *quantLocal;  //variavel local p/ quantidade de elementos no intervalo 
	quantLocal= (int *) malloc(sizeof(int));
	if(quantLocal==NULL){ exit(1);}
	long int tamBloco = N/nthreads; //tamanho do bloco de cada thread
	long int ini = id *tamBloco; //elemento inicial do bloco da thread
	long int fim; //elemento final do bloco da thread
	if(id == nthreads-1) //trata o resto se houver 
		fim= N;
	else
		fim = ini+ tamBloco;
//verifica a quantidade de elementos no intervalo
	for(long int i =ini; i<fim; i++){
		if(vetor[i]>inferior && vetor[i] <superior)
			*quantLocal+=1;
	}
//retorna a quantidade encontrada
	pthread_exit((void *) quantLocal);

}

//fluxo principal
int main (int argc, char *argv[]){
	int quantSeq=0; //quantos elementos sequecial
	int quantConc=0; //quantos elementos concorrente
	double ini,fim; //tomada de tempo
	pthread_t *tid; //identificadores das threads no sistema;
	int *retorno; //valor de retorno das threads

//recebe e valida os parametros de entrada (dimensão, n de threads e intervalo
	if(argc<5){
		fprintf(stderr, "Digite: %s <dimensão do vetor> <numero de threads> <limiar inferior> <limiar superior>\n", argv[0]);
		return 1;
	}

	N=atoll(argv[1]);
	nthreads= atoi(argv[2]);
	inferior=atof(argv[3]);
	superior=atof(argv[4]);

//aloca o vetor de entrada
	vetor= (float*) malloc(sizeof(float)*N);
	if(vetor==NULL){
		fprintf(stderr, "ERRO--malloc()\n");
		return 2;
	}
//preenche o vetor 
	srand(time(NULL));
	for(long int i=0; i<N;i++){
		//numeros aleatorios
		vetor[i]=rand() %200;
	}
//verifica a quantidade sequencialmente 
	GET_TIME(ini);
	for(long int i=0;i<N;i++){
		if(vetor[i]>inferior && vetor[i] <superior){
			quantSeq++;
		}
	}	
	GET_TIME(fim);
	printf("Tempo sequencial: %lf\n", fim-ini);

//verifica a quantidade concorrentemente
	GET_TIME(ini);
	tid = (pthread_t *) malloc (sizeof (pthread_t)*N);
	if(tid == NULL){
		fprintf(stderr, "ERRO--malloc()\n");
		return 2;
	}
//criar as threads 
	for(long int i=0;i<nthreads;i++){
		if(pthread_create(tid+i,NULL, tarefa, (void*) i)){
			fprintf(stderr, "ERRO--pthread_create()\n");
			return 3;
		}
	}	

//aguarda o termino das threads
	for(long int i=0; i<nthreads;i++){
		if(pthread_join(*(tid+i), (void **) &retorno)){
			fprintf(stderr, "ERRO--pthread_join()\n");
			return 3;
		}
		//soma global
		quantConc+=*retorno;
		free(retorno);
	}
	GET_TIME(fim);
	printf("Tempo concorrente: %lf\n", fim -ini);

//exibir os resultados
	if(quantSeq==quantConc)
		printf("Tudo certo. Quantidade: %d\n",quantConc);

//libera as areas de memoria alocadas
	free(vetor);
	free(tid);


	return 0;
}
