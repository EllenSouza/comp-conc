#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>


//o programa não trata tamanho ímpar
#define TAM 10000

//estrutura de dados para armazenar os argumentos da thread
typedef struct {
	int inicio,fim;
} t_Args;

int vetor[TAM]; //vetor global 

//funcao executada pelas Threads
void * tarefa (void * arg){
	t_Args  *args = (t_Args *) arg;
	for(int i=args->inicio;i<=args->fim;i++){
		vetor[i]*=vetor[i];
	}	

	free(arg);
	pthread_exit(NULL);
}
//funcao que verifica os valores do vetor
int verifica(int n){
	for(int i=0;i<TAM;i++){
		if(vetor[i]!=n)
			return i; //retorna a posição que não tem o valor esperado
	}
	return -1; //se der tudo certo retorna -1 
}
//funcao principal do programa
int main(void){
	pthread_t tid[2]; //identificador das threads
	
	t_Args *arg; //receberá os argumentos
	
	//inicializando o vetor 
	for(int i=0; i< TAM; i++)  
		vetor[i]=4;
	
	for(int i=1;i<=2;i++){
	//	printf("Aloca e preenche argumentos para a thread %d\n",i);
		arg = malloc (sizeof (t_Args));
		if(arg == NULL){
			printf("--ERRO: malloc()\n");
			exit(-1);
		}
		if(i==1){  //thread 1 ira manipular a primeira metade das posições
			arg->inicio=0;
			arg->fim=(TAM/2)-1 ; //até metade do vetor 
		}
		else{   //thread 2 ira manipular a segunda metade
			arg->inicio = (TAM/2);
			arg->fim= TAM-1; //até o final
		}
		//printf("Cria a thread %d\n",i);
		if(pthread_create(&tid[i], NULL, tarefa, (void *) arg)){
			printf("--ERRO: pthread-create()\n"); 
			exit(-1);
		}
	}
	//espera as threads terminarem 
	for(int i=1;i<=2;i++){
		if(pthread_join(tid[i],NULL))
			printf("--ERRO: pthread_join()\n");
	}

	//verifica os valores do vetor
	int aux= verifica(16);
	if(aux==-1)
		printf("Sucesso! Todas as %d posições estão com valor esperado!\n",TAM);
	else
		printf("Falha, Erro na posição %d do vetor\n",aux);

	//printf("terminou o thread principal\n");
	//desvincula o termino da main do termino do programa
	//pthread_exit(NUll);


	return 0;
}

