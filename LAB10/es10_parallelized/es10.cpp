#include <iostream>
#include <cmath>
#include <vector>
#include "TSM.h"
#include <fstream>
#include "random.h"
#include "mpi.h"
using namespace std;


int main(int argc, char* argv[]) {

	MPI::Init(argc,argv);		//start the parallelized work
	
	int size = MPI::COMM_WORLD.Get_size();	//numero di processi
	int rank = MPI::COMM_WORLD.Get_rank();	//assegna il rank
//Generatore random.
	Random rnd;		//crea oggetto Random.
   	int seed[4];
	int p1, p2;
   	ifstream Primes("Primes");
   	if (Primes.is_open()){
   	   for(int i=0; i<rank+1; i++) {
   	   	Primes >> p1 >> p2 ;
   	   }
   	} else cerr << "PROBLEM: Unable to open Primes" << endl;
   	Primes.close();

	   ifstream input("seed.in");
	   string property;
	   if (input.is_open()){
	      while ( !input.eof() ){
	         input >> property;
	         if( property == "RANDOMSEED" ){
	            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
	            rnd.SetRandom(seed,p1,p2);
	         }
	      }
	      input.close();
	   } else cerr << "PROBLEM: Unable to open seed.in" << endl;
	   
//*****************************
	int N, WhichConfig;
	double temp;				//Temperatura
	double L, Lp;				//Conterra' le lunghezze
	int Nmosse;
	double phi, x, y;

	ifstream dataIn("Parameters.dat");	
	dataIn>>WhichConfig;		//se (0) su circonferenza, se (1) dentro quadrato
	dataIn>>N;			//Numero città
	vector <City> cities(N);	//Vettore di città
	vector <int> sequence(N);	//sequenza.
	dataIn>>temp;			//Temperatura iniziale
	dataIn>>Nmosse;			//Numero di mosse per ogni gradino di temperatura
	dataIn.close();
	
	ofstream L_Best, finalConfig, CitiesPosition;
	
	
	Path cromosoma;	//classe cromosoma. Contiene metodi di misura ecc...

	if(rank==0) {	//solo il primo, poi diffondo.
	//INITIAL CONFIGURATION
		if( WhichConfig==0 ) {				//CIRCONFERENZA
			L_Best.open("Risultati/circonferenza/lunghezzeBest.dat");
			finalConfig.open("Risultati/circonferenza/sequenza.dat"); 		
		//Costruzione configurazione città
			CitiesPosition.open("Risultati/circonferenza/posizioni.dat");
			for(int i=0; i<N; ++i) {
				//double phi=i*2*M_PI/30;
				phi=rnd.Rannyu(0.,2*M_PI);
				cities.at(i).x=cos(phi);
				cities.at(i).y=sin(phi);
				CitiesPosition<<cos(phi)<<"   "<<sin(phi)<<endl;
				sequence.at(i)=i;		//prima sequenza.
			}
			CitiesPosition.close();
		}
	
		if( WhichConfig==1 ) {				//QUADRATO
			L_Best.open("Risultati/quadrato/lunghezzeBest.dat");
			finalConfig.open("Risultati/quadrato/sequenza.dat"); 		
			//Costruzione configurazione città
			CitiesPosition.open("Risultati/quadrato/posizioni.dat");
			for(int i=0; i<N; ++i) {
				x=rnd.Rannyu(-0.5,0.5);
				y=rnd.Rannyu(-0.5,0.5);
				cities.at(i).x=x;
				cities.at(i).y=y;
				CitiesPosition<<x<<"   "<<y<<endl;
				sequence.at(i)=i;		//prima sequenza.
			}
			CitiesPosition.close();
		}	
	}
	//diffondo l'informazione. 
	for(int i=0; i<N; i++) {
		MPI_Bcast(&(cities.at(i).x),1, MPI_DOUBLE, 0, MPI::COMM_WORLD);
		MPI_Bcast(&(cities.at(i).y),1,MPI_DOUBLE,0,MPI::COMM_WORLD);
		MPI_Bcast(&(sequence.at(i)),1, MPI_DOUBLE, 0, MPI::COMM_WORLD);
	}
	//invece tutti i nodi eseguono...
	Path appoggio(N,sequence, cities);
	cromosoma=appoggio;

//Algoritmo......***********************//
	int *lanci, *accettati;		//Per tassi di accettazione
	lanci=new int[3];		//sono tre mutazioni
	accettati=new int[3];		//    ""      ""
	int flag=1;
	
	int quale, accetto;	//primo intero per scegliere mutazione, secondo intero conterrà la decisione di accettare o meno
	double prob;		//probabilità di accettazione
	while(temp>=0.01) {
		for(int i=0; i<3; i++) {	//resetta i tassi di accettazione
			accettati[i]=0;
			lanci[i]=0;
		}
		for(int i=0; i<Nmosse; i++) {
			L=cromosoma.GetLength(cities);		//lunghezza attuale
			quale=(i)%3;				//a turno sceglie la mutazione
			Path proposta=T_Random(&rnd, cromosoma, quale);	//genera una proposta
			Lp=proposta.GetLength(cities);		//misura la lunghezza della proposta
			prob=min(1., exp((L-Lp)/temp));		//calcola la probabilità
			accetto=A_AccRej(&rnd, prob, lanci, accettati, quale);	//decide di accettare con probabilità prob
			if(accetto==1) {
				cromosoma=proposta;
			} else if(accetto==0) {
			}
			/*if(rank==0) {
				flag+=1;
				int modulo=flag%9;
		
				if(temp<=5 and i==(Nmosse-1) and modulo==0) {
					L_Best<<1./temp<<"  "<<cromosoma.GetLength(cities)<<endl;
				
				}
			}*/
		}
		/*if(rank==0) {
			cout<<"Tasso di accettazione per PairPermutation: "<<100.*(double)accettati[0]/(double)lanci[0]<<"%"<<endl;
			cout<<"Tasso di accettazione per BlockShift: "<<100.*(double)accettati[1]/(double)lanci[1]<<"%"<<endl;
			cout<<"Tasso di accettazione per BlockSwap: "<<100.*(double)accettati[2]/(double)lanci[2]<<"%"<<endl;
			cout<<"Lunghezza attuale L="<<cromosoma.GetLength(cities)<<endl;
		}*/
		if(temp>10) {
			temp-=1;
		} 
		if(temp<=10) {
			temp-=temp/(temp+800.);
		}
	}
	

	
	
	//Ora comincia la parte finale: Raccolgo le informazioni e trovo il percorso migliore
	L=cromosoma.GetLength(cities);	//salvo le lunghezze a cui sono arrivato
	cout<<L<<"     from the process "<<rank<<endl;	//check
	

	Path percorso=cromosoma;	//per fare un confronto
	
	int indice;			//cicla sui rank 
	int ricevitore;			//variabile di appoggio per ricevere i percorsi.
	int invio;			//variabile di appoggio per inviare
	indice=1;			
	//trova il migliore.
	
	
	while(indice<size) {
		for(int i=0; i<N; ++i) {
			if(rank==indice ) {
				invio=cromosoma.GetElement(i);
				MPI::COMM_WORLD.Send(&invio,1, MPI_INT, 0, 1);
			}
			if(rank==0) {
				MPI::COMM_WORLD.Recv(&ricevitore,1, MPI_INT, indice, 1);
				percorso.SetElement(i, ricevitore);
			}
		}
		indice+=1;
		if(rank==0) {
			if(percorso.GetLength(cities)<L) {
				cromosoma=percorso;
				L=cromosoma.GetLength(cities);
			}
		}
	}
	//Ora che sul processo 0 ho solo il migliore...output!
	
	int i;
	if(rank==0) {
		cout<<cromosoma.GetLength(cities)<<endl;
		for(i=0; i<cromosoma.GetN(); i++) {
			finalConfig<<cromosoma.GetElement(i)<<endl;	
		}
		finalConfig.close();
		L_Best<<L<<endl;
		L_Best.close();
	}
	
	MPI::Finalize();
	
	return 0;
}

	















