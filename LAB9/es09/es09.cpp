#include <iostream>
#include <cmath>
#include <vector>
#include "TSM.h"
#include <fstream>
#include "random.h"
using namespace std;


int main() {

//Generatore random.
	Random rnd;		//crea oggetto Random.
   	int seed[4];
	int p1, p2;
   	ifstream Primes("Primes");
   	if (Primes.is_open()){
   	   Primes >> p1 >> p2 ;
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
	int N, Npop, WhichConfig, NGenerations;
	vector <City> cities(1000);		//conterrà le coordinate delle città
	vector <int> sequence(1000);		//Mi serve per costruire la popolazione
	vector <Path> popolazione;	//Popolazione
	double probMut, probCross;	//Probabilità di mutazione e crossover
	
	ifstream dataIn("Parameters.dat");	
	dataIn>>WhichConfig;		//se (0) su circonferenza, se (1) dentro quadrato
	dataIn>>N;			//Numero città
	Npop=N*N;			//dimensione della popolazione
	dataIn>>NGenerations;		//Numero di generazioni che si vuole creare
	dataIn>>probMut;		//Probabilità
	dataIn>>probCross;		// ""	""
	dataIn.close();
	ofstream L_Mean, L_Best, finalConfig;
	cities.resize(N);
	sequence.resize(N);
	

	//INITIAL CONFIGURATION
	if( WhichConfig==0 ) {							//CIRCONFERENZA
		L_Mean.open("Risultati/circonferenza/lunghezzeMedie.dat");
		L_Best.open("Risultati/circonferenza/lunghezzeBest.dat");
		finalConfig.open("Risultati/circonferenza/sequenza.dat"); 		
		//Costruzione configurazione città
		ofstream CitiesPosition("Risultati/circonferenza/posizioni.dat");
		for(int i=0; i<N; ++i) {
			//double phi=i*2*M_PI/30;
			double phi=rnd.Rannyu(0.,2*M_PI);
			cities.at(i).x=cos(phi);
			cities.at(i).y=sin(phi);
			CitiesPosition<<cos(phi)<<"   "<<sin(phi)<<endl;
			sequence.at(i)=i;		//prima sequenza.
		}
		Path cromosoma(N,sequence, cities);
		popolazione.push_back(cromosoma);
		for(int i=1; i<Npop; ++i) {
			popolazione.push_back(PairPermutation(&rnd, popolazione.at(i-1)));
		}
		//popolazione=SelectionSort(popolazione, cities);
		//for(int i=0; i<Npop; ++i) cout<<popolazione.at(i).GetLength(citta)<<endl;

	}
	
	if( WhichConfig==1 ) {							//QUADRATO
		L_Mean.open("Risultati/quadrato/lunghezzeMedie.dat");
		L_Best.open("Risultati/quadrato/lunghezzeBest.dat");
		finalConfig.open("Risultati/quadrato/sequenza.dat"); 		
		//Costruzione configurazione città
		ofstream CitiesPosition("Risultati/quadrato/posizioni.dat");
		for(int i=0; i<N; ++i) {
			double x=rnd.Rannyu(-0.5,0.5);
			double y=rnd.Rannyu(-0.5,0.5);
			cities.at(i).x=x;
			cities.at(i).y=y;
			CitiesPosition<<x<<"   "<<y<<endl;
			sequence.at(i)=i;		//prima sequenza.
		}
	Path cromosoma(N, sequence, cities);
	popolazione.push_back(cromosoma);
		for(int i=1; i<Npop; ++i) {
			popolazione.push_back(PairPermutation(&rnd, popolazione.at(i-1)));
		}
		//popolazione=SelectionSort(popolazione, cities);
		//for(int i=0; i<Npop; ++i) cout<<popolazione.at(i).GetLength(citta)<<endl;
	}
	//Algoritmo************************
	//Inizio delle mutazioni.
	double Lmean;
	double Lbest;
	int quale1, quale2;		//selezione parents
	double p;			//variabile di confronto per probabilità di agire
	for(int i=0; i<NGenerations; ++i) {
		//NGeneration volte creo una nuova generazione
		popolazione=SelectionSort(popolazione,cities);	//Mette in ordine
		vector <Path> NewPopolazione;
		
		for(int j=0; j<Npop/2; j++) {
			quale1=Selection(N, &rnd);
			quale2=Selection(N, &rnd);
			Path path1=popolazione.at(quale1);
			Path path2=popolazione.at(quale2);
			NewPopolazione.push_back(path1);
			NewPopolazione.push_back(path2);
			p=rnd.Rannyu();
			if(p<probCross) {		//in questo caso esegue il crossover
				NewPopolazione=Crossover(&rnd,NewPopolazione);
			}
			p=rnd.Rannyu();
			if(p<probMut/2.) {			//riduco ulteriormente per lo shift perchè rallenta molto l'esecuzione
				int n=rnd.Rannyu(0,N);
				NewPopolazione.at(j*2)=Shift(n, NewPopolazione.at(j*2));
				NewPopolazione.at(j*2+1)=Shift(n, NewPopolazione.at(j*2+1));
			}
			p=rnd.Rannyu();
			if(p<probMut) {
				int n1=rnd.Rannyu(0,N);
				int n2=rnd.Rannyu(0,N);
				int Max=max(n1,n2);
				int Min=min(n1,n2);
				int LimiteM=min(Max-Min,N-Max+Min);
				int m=rnd.Rannyu(0,LimiteM);
				NewPopolazione.at(j*2)=BlockSwap(n1,n2,m,NewPopolazione.at(j*2));
				NewPopolazione.at(j*2+1)=BlockSwap(n1,n2,m,NewPopolazione.at(j*2+1));
			}
			p=rnd.Rannyu();
			if(p<probMut) {
				int n=rnd.Rannyu(0,N);
				int m=rnd.Rannyu(0,N/2);	//che sia ancora intero...
				NewPopolazione.at(j*2)=Inversion(n,m, NewPopolazione.at(j*2));
				NewPopolazione.at(j*2+1)=Inversion(n,m,NewPopolazione.at(j*2+1));
			}
		}
		
		
		

		//Pronta la nuova generazione
		popolazione=SelectionSort(NewPopolazione,cities);
		//Ora abbiamo la nuova generazione in ordine salvata al posto di quella vecchia
		Lmean=0;
		for(int j=0; j<Npop/2; ++j) {
			Lmean+=popolazione.at(j).GetLength(cities);
		}
		Lmean/=(Npop/2);
		Lbest=popolazione.at(0).GetLength(cities);
		L_Mean<<i<<"   "<<Lmean<<endl;
		L_Best<<i<<"   "<<Lbest<<endl;
		cout<<"Generazione "<<i<<endl;
		cout<<"Lbest=  "<<Lbest<<endl;
		cout<<"Lmean=  "<<Lmean<<endl;
		if(i==NGenerations-1) {
			for(int j=0;j<N; ++j) {
				finalConfig<<popolazione.at(0).GetElement(j)<<endl;
			}
		}	
	}
				
			
	L_Mean.close();
	L_Best.close();
	finalConfig.close();

return 0;
}















