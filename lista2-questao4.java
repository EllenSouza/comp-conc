import java.util.concurrent.Semaphore;
import java.util.Queue;
import java.util.LinkedList;
import java.util.Random; 

class Impressora{
    
    Queue <Integer> fila;
    Semaphore semI, semR, mutex;

    Impressora(Semaphore semI, Semaphore semR, Semaphore mutex){
        this.semI=semI;
        this.semR=semR;
        this.mutex=mutex;
        fila = new LinkedList<>();
    }

    public void insere (int n){
        try{
            //System.out.println("Thread produtora esperando permissão para inserir na fila");
            semI.acquire(); //wait do semáforo de inserção
            mutex.acquire();//excluao mútua para inserir
            fila.add(n);
            //System.out.printf("Thread produtora consegue inserir na fila o elemento {%d}\n",n);
        
            mutex.release();//libera exclusão mútua

            semR.release();//libera uma thread que remove

        }catch(InterruptedException e){}
        
    }
    public int retira(){
        int elemento;
        try{
            //System.out.println("Thread consumidora esperando requisições na fila");
            semR.acquire(); //wait do semáforo de retirada
            mutex.acquire(); //exclusão mútua
            elemento=fila.remove();
            //System.out.printf("Thread consumidora consumiu elemento {%d}\n",elemento);
            mutex.release();//libera exclusão múttua
            semI.release();
            return elemento;
        }catch (InterruptedException e ){ return -1;}
    }

}
class Insere extends Thread{
    Impressora impressora;
    Random aleatorio = new Random();

    Insere(Impressora m){
        this.impressora=m;
    }

    public void run(){
        while(true){
            impressora.insere(aleatorio.nextInt(100));
        }
    }
}
class Saida extends Thread{
    //thread que imprime
    int saida;
    Impressora impressora;
    Saida(Impressora m){
        this.impressora=m;
    }

    public void run (){
        while (true){
            synchronized(this){
                saida=impressora.retira();
                System.out.println(saida);
            }
        }
    }
}
class Principal {
    static final int N_A =2; //threads que inserem
    static final int tam = 2;//tamanho maximo da fila

    public static void main (String [] args){
        Semaphore semI = new Semaphore(tam);
        Semaphore semR = new Semaphore(0);
        Semaphore mutex = new Semaphore(1) ; //binário

        Impressora impressora = new Impressora(semI, semR, mutex);
        Thread imprime = new Saida(impressora);
        Thread [] geradora = new Insere[N_A];

        imprime.start();

        //cria as threads que geram as requisições
        for(int i=0;i<N_A; i++){
            geradora[i]= new Insere(impressora);
            geradora[i].start();
        }
    }
}
