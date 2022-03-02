import java.util.Queue;
import java.util.LinkedList;
import java.util.Random; 


class Impressora {
    static final int tam = 20; 
    int size;//controla o tamanho da lista
    Queue <Integer> fila;
    
    Impressora(){
        this.size=0;
        fila = new LinkedList<>();
    }

    public synchronized void insere(int n){
        try{
            while(this.size==tam){
                //System.out.println("thread que insere vai ficar retida pois fila está cheia");
                wait(); //espera esvaziar
            }
            fila.add(n);
            this.size++;
            notifyAll(); //libera inserção ou remoção
            //System.out.printf("thread inseriu elemento {%d} e liberou \n",n);
        }catch (InterruptedException e) {}
        
    }
    public synchronized int retira (){
        int elemento;
        try{
            while (this.size==0){
                //System.out.println("thread não consegue retirar e imprimir pois fila vazia");
                wait();//bloqueia pois não há elementos
            }
            elemento=fila.remove();
            this.size--;
            notifyAll();//libera inserção ou remoção
            //System.out.printf("thread consegue retirar elemento {%d} e imprimir e liberar %d\n",elemento);
            return elemento;
        }catch (InterruptedException e){return -1;}
        
    }
 
    public synchronized int getSize(){
        return this.size;
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

    public void run(){
        while(true){
            synchronized(this){
                saida=impressora.retira();  
                System.out.println(saida);
            }
            
        }
    }
}

class Principal{
    static final int N_A = 2; //threads  que inserem


    public static void main (String [] args){
        Impressora impressora = new Impressora();
        Thread imprime = new Saida(impressora);
        Thread [] geradora = new Insere[N_A];

        imprime.start();

        //cria as threads que geram requisições de impressão
        for (int i=0;i<N_A;i++){
            geradora[i]=new Insere(impressora);
            geradora[i].start();
        }

    }
}
