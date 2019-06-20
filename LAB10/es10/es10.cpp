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
	int N, WhichConfig;
	vector <City> cities(1000);		//conterrà le coordinate delle città
	vector <int> sequence(1000);		//Conterra' la sequenza.
	double temp;				//Temperatura
	double L, Lp;				//Conterra' le lunghezze
	int Nmosse;

	ifstream dataIn("Parameters.dat");	
	dataIn>>WhichConfig;		//se (0) su circonferenza, se (1) dentro quadrato
	dataIn>>N;			//Numero città
	dataIn>>temp;			//Temperatura iniziale
	dataIn>>Nmosse;			//Numero di mosse per ogni gradino di temperatura

	
	dataIn.close();
	ofstream L_Best, finalConfig;
	cities.resize(N);
	sequence.resize(N);
	Path cromosoma;

	//INITIAL CONFIGURATION
	if( WhichConfig==0 ) {				//CIRCONFERENZA
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
		Path appoggio(N,sequence, cities);
		cromosoma=appoggio;
	}
	
	if( WhichConfig==1 ) {				//QUADRATO
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
		Path appoggio(N,sequence, cities);
		cromosoma=appoggio;
	}

//Algoritmo......***********************//
	int *lanci, *accettati;		//Per tassi di accettazione
	lanci=new int[3];		//sono tre mutazioni
	accettati=new int[3];		//    ""      ""
	int indice=1;
	
	int quale, accetto;	//primo intero per scegliere mutazione, secondo intero conterrà la decisione di accettare o meno
	double prob;		//probabilità di accettazione
	while(temp>=0.01) {
		cout<<endl<<endl;
		cout<<"Temperatura T= "<<temp<<endl;
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
			indice+=1;
			int modulo=indice%9;
		
			if(temp<=5 and i==(Nmosse-1) and modulo==0) {
					cout<<"eccomi"<<endl;
					L_Best<<1./temp<<"  "<<cromosoma.GetLength(cities)<<endl;
				
			}
		}
		cout<<"Tasso di accettazione per PairPermutation: "<<100.*(double)accettati[0]/(double)lanci[0]<<"%"<<endl;
		cout<<"Tasso di accettazione per BlockShift: "<<100.*(double)accettati[1]/(double)lanci[1]<<"%"<<endl;
		cout<<"Tasso di accettazione per BlockSwap: "<<100.*(double)accettati[2]/(double)lanci[2]<<"%"<<endl;
		
		cout<<"Lunghezza attuale L="<<cromosoma.GetLength(cities)<<endl;
		if(temp>10) {
			temp-=1;
		} 
		if(temp<=10) {
			temp-=temp/(temp+700.);
		}
	}
	
	for(int i=0; i<cromosoma.GetN(); i++) {
		finalConfig<<cromosoma.GetElement(i)<<endl;
	}
	finalConfig.close();
	L_Best.close();
	return 0;
}

	















