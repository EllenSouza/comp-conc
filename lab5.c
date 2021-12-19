/*lab 5 Ellen Almeida de Souza */
/*thread5 imprime primeiro dps 2,3 e 4 em qualquer ordem e por último thread1 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS  5

/* Variaveis globais */
int x = 0;
pthread_mutex_t var_mutex;
pthread_cond_t var_cond;

/* Thread5 */
void *Thread5 (void *t) {
  int boba1, boba2;

  /* faz alguma coisa pra gastar tempo... */
  boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;

  printf("Seja bem-vindo!\n");

  pthread_mutex_lock(&var_mutex);
  x++;
  pthread_cond_broadcast(&var_cond);
  pthread_mutex_unlock(&var_mutex);

  pthread_exit(NULL);
}

/* Thread2 */
void *Thread2 (void *t) {

  pthread_mutex_lock(&var_mutex);
  while (x == 0) { 
     pthread_cond_wait(&var_cond, &var_mutex);
  }
  x++;

  printf("Fique a vontade.\n");
  pthread_cond_broadcast(&var_cond);

  pthread_mutex_unlock(&var_mutex); 

  pthread_exit(NULL);
}

/* Thread3 */
void *Thread3 (void *t) {

  pthread_mutex_lock(&var_mutex);
  while (x == 0) {
     pthread_cond_wait(&var_cond, &var_mutex);
  }
  x++;

  printf("Sente-se por favor.\n");
  pthread_cond_broadcast(&var_cond);

  pthread_mutex_unlock(&var_mutex);

  pthread_exit(NULL);
}

/* Thread4 */
void *Thread4 (void *t) {

  pthread_mutex_lock(&var_mutex);
  while (x == 0) {
     pthread_cond_wait(&var_cond, &var_mutex);
  }
  x++;

  printf("Aceita um copo d'agua?.\n");
  pthread_cond_broadcast(&var_cond);

  pthread_mutex_unlock(&var_mutex);

  pthread_exit(NULL);
}


/* Thread1 */
void *Thread1 (void *t) {

  pthread_mutex_lock(&var_mutex);
  while (x < NTHREADS-1 ) { 
     pthread_cond_wait(&var_cond, &var_mutex);
  }
  printf("Volte sempre!\n");
  
  pthread_mutex_unlock(&var_mutex);
  pthread_exit(NULL);
}


/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&var_mutex, NULL);
  pthread_cond_init (&var_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[2], NULL, Thread3, NULL);
  pthread_create(&threads[1], NULL, Thread2, NULL);
  pthread_create(&threads[0], NULL, Thread1, NULL);
  pthread_create(&threads[4], NULL, Thread5, NULL);
  pthread_create(&threads[3], NULL, Thread4, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&var_mutex);
  pthread_cond_destroy(&var_cond);

  return 0;
}
