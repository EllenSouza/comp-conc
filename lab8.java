//Ellen Almeida de Souza DRE: 120041556
//leitores e escritores em java 

//monitor que implementa a lógica do padrão
//leitores/escritores
class LE{
	private int leit, escr;
	private int dado;

	//construtor
	LE(){
		this.leit=0;  //leitor (0 ou mais) 
		this.escr=0;  //escritor (0 ou 1)
		this.dado=0;
	}
	public synchronized int get(){
		return dado;
	}
	public synchronized void set(int n){
		this.dado=n;
	}

	//entrada para leitores
	public synchronized void EntraLeitor(int id){
		try{
			while (this.escr>0){
				System.out.println("le.leitorBloqueado ("+id+")");
				wait(); //bloqueia pela condição lógica 
			}
			this.leit++; //registra que há mais um leitor
			System.out.println("le.leitorLendo ("+id+")");
		}catch (InterruptedException e) {}
	}
	//saida para leitores
	public synchronized void SaiLeitor (int id){
		this.leit--;  //registra que um leitor saiu
		if (this.leit ==0)
			this.notify(); //libera escritor (caso exista um bloqueado)
		System.out.println("le.leitorSaindo ("+id+")");

	}

	//entrada para escritores
	public synchronized void EntraEscritor(int id){
		try {
			while ((this.leit>0)|| (this.escr>0)){
				System.out.println("le.escritorBloqueado ("+id+")");
				wait(); //bloqueia pela lógica da aplicação
			}
			this.escr++; //registra que há um escritor escrevendo
			System.out.println("le.escritorEscrevendo ("+id+")");
		}catch (InterruptedException e) {}
	}

	//saida para escritores
	public synchronized void SaiEscritor(int id){
		this.escr--;  //registra que escritor saiu
		notifyAll(); //libera leitores e escritores
		System.out.println("le.escritorSaindo ("+id+")");

	}

}

//aplicação

//leitor
class Leitor extends Thread {
	int id; //idenitifcador da thread
	int delay; //atrado bobo
	LE monitor; //objeto monitor para coordenar a lógica de execução das threads
	int aux;

	//contrutor
	Leitor(int id, int delayTime, LE m){
		this.id=id;
		this.delay=delayTime;
		this.monitor=m;
	}

	//método executado pela thread
	public void run (){
		double j=777777777.7, i;
		try{
			for(;;){
				this.monitor.EntraLeitor(this.id);
				for(i=0;i<100000000; i++) {j=j/2;}
			       //loop bobo para simbolizar o tempo de leitura
				
				//leitura do dado
				aux=monitor.get();
				if(aux%2==0)
					System.out.printf("%d ----> par l(%d)\n", aux,this.id);
				else
					System.out.printf("%d----> impar l(%d)\n",aux,this.id);

				this.monitor.SaiLeitor(this.id);
				sleep(this.delay);	
			}
		}catch(InterruptedException e) {return ;}
	}

}//fim classe leitora

//escritor
class Escritor extends Thread{
	int id; //identificador da thread
	int delay; //atraso bobo..
	LE monitor;// objeto monitor para coordenar a lógica de execução
		
	//construtor
	Escritor(int id, int delayTime, LE m){
		this.id=id;
		this.delay=delayTime;
		this.monitor=m;
	}

	//método executado pela thread
	public void run (){
		double j=777777777.7, i;

		try{
			for(;;){
				this.monitor.EntraEscritor(this.id);
				for(i=0; i<100000000; i++) {j=j/2;} 
					//loop bobo 
				
				//escreve
				this.monitor.set(this.id);
				this.monitor.SaiEscritor(this.id);
				sleep(this.delay); //atraso bobo 
			}
		}catch (InterruptedException e) {return ;}
	}
}//fim classe escritora

//classe leitora e escritora
class LeitorEscritor extends Thread{
	int id;  //id da thread
	int delay; //atraso bobo
	LE monitor; //monitor
	int aux;  //auxiliar para verificar e printar

	//construtor
	LeitorEscritor(int id, int delayTime, LE m){
		this.id=id;
		this.delay=delayTime;
		this.monitor=m;
	
	}

	//método executado pela thread
	public void run (){
		double j=777777777.7, i;
		try{
			this.monitor.EntraLeitor(this.id);
			for (i=0; i<100000000; i++) {j=j/2;} //loop bobo
			
			//leitura e impressão
			aux=monitor.get();
			if(aux%2==0)	
				System.out.printf("%d ----> par el(%d)\n", aux,this.id);
			else
				System.out.printf("%d ----> impar el(%d)\n", aux,this.id);			
			this.monitor.SaiLeitor(this.id);

			this.monitor.EntraEscritor(this.id);
			this.monitor.set(this.id);
			this.monitor.SaiEscritor(this.id);

			sleep(this.delay);
		}catch (InterruptedException e) {return;}

	}
}//fim da classe que le e escreve


//classe principal
class Principal {
	static final int L=2;
	static final int E=4;
	static final int LE=1;

	public static void main (String[] args){
		int i;
		//monitor (objeto compartilhado entre leitores e escritores
		LE monitor = new LE();
		Leitor[] l = new Leitor[L]; //threads Leitoras
		Escritor[] e = new Escritor[E]; //threads escritoras 
		LeitorEscritor[] le = new LeitorEscritor[LE]; //thread leitora/escritora
		
		//inicia o log de saida 
		System.out.println("import verificaLE");
		System.out.println("le = verificaLE.LE()");

		for(i=0; i<L; i++){
			l[i]= new Leitor(i+1, (i+1)*500, monitor);
			l[i].start();
		}

		for(i=0; i<E; i++){
			e[i] = new Escritor(i+1, (i+1)*500, monitor);
			e[i].start();
		}
		for(i=0; i< LE; i++){
			le[i] =  new LeitorEscritor(i+1, (i+1)*500, monitor);
			le[i].start();
		}
	}

}


