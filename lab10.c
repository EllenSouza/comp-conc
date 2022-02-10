//Ellen Almeida de Souza DRE:120041556
//produtores e consumidores com semaforos

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> //para o sleep
#define N 10

//variáveis globais 
sem_t slotCheio, slotVazio; //condição
sem_t mutexCons; //exclusão mutua
int Buffer[N];

void insere (int * item, int id){
    
    //aguarda slot vazio
    sem_wait(&slotVazio);
    printf("thread produtora %d atuando\n",id);

   
    for(int i=0; i<N; i++){
        Buffer[i]=*(item+i); //preenche o Buffer todo
    }
    printf("Buffer cheio !!\n");


    //terminou de preencher o Buffer 
    printf("thread produtora %d liberando as threads consumidoras...\n\n", id);
    for(int i=0; i<N; i++){
        sem_post(&slotCheio); //libera as consumidoras
    }
    
}

int retira(int id){
    int item;
    static int out=0;
    //aguarda slot cheio
    sem_wait(&slotCheio);

    printf("thread %d consumidora atuando...\n", id);

    //exclusão mútua entre consumidores
    sem_wait(&mutexCons);
    item = Buffer[out];
    printf("Item %d foi consumido pela thread consumidora %d \n",out, id);
    out = (out +1) % N;
    
    
    //sinaliza slot vazio
    if(out == 0){ //buffer vazio 
        sem_post(&slotVazio); //libera as produtoras
        printf("Buffer vazio!!\n\n");
    }
    sem_post(&mutexCons);
    return item;
}

//funções das threads produtoras
void *produtor ( void * arg){
    long int id = (long int ) arg;
    int elemento[N];
    while(1){
        //produz um elemento ...
        for(int i=0;i<N;i++){
            elemento[i]=rand() % 10 ;
        }


        insere(elemento, id);
        sleep(1);
    }
    pthread_exit(NULL);
}

void *consumidor (void * arg){
    long int id = (long int) arg;
    while(1){
        retira(id);
        //consome e elemento ...
        sleep(1);
    }
    pthread_exit(NULL);
}

//fluxo principal
int main (int argc, char * argv[]){

    if(argc<3){
        printf("Entre com %s <threads produtoras> <threads consumidoras>\n", argv[0]);
        return 1;
    }
    int prod=atoi(argv[1]), cons=atoi(argv[2]);
    pthread_t tid[prod+cons];
    long int id=0;

    srand(time(NULL)); //para os números aleatórios

    //inicializa os semáforos
    sem_init(&mutexCons,0,1);
    sem_init(&slotCheio, 0, 0);
    sem_init(&slotVazio,0,1); //binário


    //cria as threads 
    for(int i=0;i<prod;i++){
        if(pthread_create(&tid[i],NULL, produtor, (void *) id)){
            printf("ERRO--pthread_create()\n");
        }       
        id++;
    }
    id=0;
    for(int i = 0; i<cons;i++){
        if(pthread_create(&tid[i],NULL, consumidor, (void *) id)){
            printf("ERRO--pthread_create()\n");
        }
        id++;
    }

    pthread_exit(NULL);
    return 0;
}
