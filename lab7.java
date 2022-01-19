//Ellen Almeida de Souza 
//DRE: 120041556
//somar elementos de um vetor usando concorrência em java
//
//classe com a estrutura compartilhada
class Soma{

	//variável compartilhada 
	private int soma;
	//construtor
	public Soma (){
		this.soma=0;
	}

	//operação de leitura sobre o recurso  compartilhado
	public synchronized int get(){
		return this.soma;
	}
	//operação de adição sobre o recurso compartilhado
	public synchronized void set(int n){
		this.soma+=n;
	}

}


//criar uma classe que estende a classe thread 
class Somadora extends Thread{
       //identificador da thread
       private int id;

	//objeto compartilhado com outras Threads 
	Soma s;
	int [] vetor;
	int N;
	//consrutor 
	public Somadora(int id,Soma s, int [] v, int N){
		this.id=id;
		this.s=s;
		this.vetor=v;
		this.N=N;
	}
	//método principal da thread
	public void run(){
		//percorre o array de forma alternada 
		for(int i=id;i<vetor.length;i+=N){
			s.set(vetor[i]);
			//System.out.printf("thread %d somando....\n", id);
		}
	}
}

//classe da aplicação com a main
class Principal{
	static final int N=4;

	public static void main (String[] args){
		//reserva espaço para um vetor de threads
		Thread[] threads = new Thread[N];

		//cria uma instância do recurso compartilhado 
		Soma s = new Soma();
		//cria e preenche vetor
		int [] array = new int [15];
		// var de verificação
		int aux=0;
		for(int j=0;j<array.length;j++){
			array[j]=j;
			aux+=array[j];
		}
		
		//cria as threads da aplicação
		for(int i=0;i<threads.length;i++){
			threads[i]= new Somadora(i,s,array,N);
			//System.out.printf("thread %d criada \n", i);
		}

		//inicia as threads 
		for (int i=0; i<threads.length;i++){
			threads[i].start();
			//System.out.printf("thread %d iniciou\n", i);
		}


		//espera as threads terminarem
		for(int i=0;i<threads.length;i++){
			try {threads[i].join();}
			catch (InterruptedException e) {return; }
			//System.out.printf("thread %d concluida\n", i);
		}

		System.out.println("Somatório = " + s.get());
		if(aux== s.get()){
			System.out.println("Tudo certo! Soma correta\n");
		}
	}

}
