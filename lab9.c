//Ellen Almeida de Souza  DRE:120041556

//uso de semáforos para sincronização
//t5 imprime primeiro e t1 imprime por último

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h> 
#define NTHREADS 5

//variáveis globais 
int cont=0;
//semáforos para sincronizar a ordem de execução das thraeds
sem_t condMeio, condFim ;
//semáforo para exclusão mútua
sem_t mutex;

//thread 1 
void * t1 (void * arg){
    sem_wait(&condFim);
    printf("Volte sempre!\n");
    pthread_exit(NULL);
    pthread_exit(NULL);
}
//thread 2
void * t2 (void * arg){
    sem_wait(&condMeio);
    printf("Fique a vontade.\n");
    sem_wait(&mutex); //entrada na seção crírica
    cont++;
    sem_post(&mutex); //saída da seção crítica
    if(cont==(NTHREADS-1))     sem_post(&condFim);
    pthread_exit(NULL);
}
//thread 3
void * t3 (void * arg){
    sem_wait(&condMeio);
    printf("Sente-se por favor.\n");
    sem_wait(&mutex); //entrada na seção crírica
    cont++;
    sem_post(&mutex); //saída da seção crítica
    if(cont==(NTHREADS-1))     sem_post(&condFim);
    pthread_exit(NULL);
}
//thread 4
void * t4 (void * arg){
    sem_wait(&condMeio);
    printf("Aceita um copo d'água?\n");
    sem_wait(&mutex); //entrada na seção crírica
    cont++;
    sem_post(&mutex); //saída da seção crítica
    if(cont==(NTHREADS-1))     sem_post(&condFim);
    pthread_exit(NULL);
}
//thraed 5
void * t5 (void * arg){
    printf("Seja bem-vindo!\n");
    //permite as threads do meio
    for(int i=0;i<(NTHREADS-2);i++)
        sem_post(&condMeio);
    sem_wait(&mutex); //entrada na seção crírica
    cont++;
    sem_post(&mutex); //saída da seção crítica 
    pthread_exit(NULL);
}

int main (void){
    //identificador das threads
    pthread_t tid[NTHREADS];

    //inicializa os semáforos
    sem_init(&condMeio, 0, 0);
    sem_init(&condFim, 0, 0);
    sem_init(&mutex,0,1); //semáforo binário 
    
    //cria as threads
    pthread_create(&tid[2], NULL, t3, NULL);
    pthread_create(&tid[1], NULL, t2, NULL);
    pthread_create(&tid[0], NULL, t1, NULL);
    pthread_create(&tid[4], NULL, t5, NULL);
    pthread_create(&tid[3], NULL, t4, NULL);

    //espera as threads terminarem
    for(int i=0;i<NTHREADS;i++)
        pthread_join(tid[i],NULL);
    return 0;
}
