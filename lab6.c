//Ellen 
//dre 120041556
//implementando barreira 

#include <stdio.h>
#include <stdlib.h>
# include <pthread.h>

//variáveis globais
int bloqueadas=0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;
int n;
int *vetor; //vetor que será usado pelas threads

//função da barreira
void barreira (int nthreads){
    pthread_mutex_lock(&x_mutex); //inicio seção crítica
    if(bloqueadas== (nthreads-1)){
        //última thread a chegar na barreira
        pthread_cond_broadcast(&x_cond);
        bloqueadas=0;
    }
    else{
        bloqueadas++;
        pthread_cond_wait(&x_cond, &x_mutex);

    }
    pthread_mutex_unlock(&x_mutex);
}

//função que as threads irão executar
void * tarefa (void * arg){
    int id = *((int *) arg);
    int * somatorio;
    somatorio= malloc(sizeof(int));
    if(somatorio==NULL){
        printf("ERRO--malloc()\n");
        exit(1);
    }
    *somatorio=0;  //inicializando 
    for(int j=0;j<n;j++){

        for(int i=0; i< n;i++){
            *somatorio+=*(vetor+i);
        }
        //printf("thread %d encontra a 1 barreira\n", id);
        barreira(n); //aguarda as outras terminarem suas somas
        
        //altera o vetor
        srand(time(NULL));
        pthread_mutex_lock(&x_mutex); //seção crítica
        *(vetor+id)=rand() %10;
        pthread_mutex_unlock(&x_mutex);

        //printf("thread %d encontra a 2 barreira\n", id);
        barreira(n); //aguarda as outras terminarem de escrever
    }
    pthread_exit((void *)somatorio);
}

//fluxo principal
int main (int argc, char * argv[]){
    if(argc <2){
        printf("Insira a quantidade de iteração\n");
        exit(1);
    }
    n=atoi(argv[1]);

    pthread_t threads[n]; //vetor com os id das threads no sistema
    int id[n]; //vetor com os id das threads
    int *retorno; //valor de retorno das threads
    int result[n]; //vetor com o retorno de todas as threads

    //inicializa o mutex e a variável de condição
    pthread_mutex_init (&x_mutex,NULL);
    pthread_cond_init (&x_cond, NULL);


    //inicializando o vetor
    vetor= malloc(sizeof(int)*n);
    if(vetor==NULL){
        printf("ERRO--malloc()\n");
        exit(1);
    }
    srand(time(NULL));
    for(int i=0;i<n;i++){
        *(vetor+i)=rand() %10;
    }

    //criação das threads
    for(int i=0; i<n;i++){
        id[i]=i;
        if (pthread_create(&threads[i], NULL, tarefa, (void *)&id[i])){
            printf("ERRO--pthread_create()\n");
            return 2;
        }
    }

    //espera todas as threads acabarem a execução
    for(int i=0; i<n; i++){
        if (pthread_join(threads[i],(void **) &retorno)){
            printf("ERRO--pthread_join()\n");
            return 2;
        }
        result[i]=*retorno;
        //printf("Retorno thread %d = %d \n",i,*retorno);
        free(retorno);
    }
    //verifica se os retornos estão iguais 
    for(int i=1;i<n; i++){
        if(result[i]!=result[0]){
            printf("ERRO-- resultados inconsistentes\n");
            break;
        }
        if(i==(n-1))
            printf("Tudo certo! os valores retornados estão corretos e são iguais a %d\n",result[0]);
    }
    //destrou o mutex e a variável de condição
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    
    free(vetor);
    return 0;
}
