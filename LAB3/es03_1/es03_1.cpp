#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include "random.h"
#include "prezzo.h"
#include "MediaBlocchi.h"
using namespace std;

int main() {
	//generatore
	Random* rnd=new Random();		
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
	            rnd->SetRandom(seed,p1,p2);
	         }
	      }
	      input.close();
	   } else cerr << "PROBLEM: Unable to open seed.in" << endl;
//***************************
	double S_0, K, Sigma, T, r, t;	//prezzo iniziale, prezzo concordato, Flutt, Tempo finale, rate, tempo in generale.
	t=0.; 
	double *Cparz, *Pparz, *C2parz, *P2parz;	//mi servono per l'algoritmo.
	int passi, simul, Nblocchi, L;	//numero di passi per caso discreto, numero di simulazioni, n. blocchi, lunghezza blocchi.

	ifstream read("Parameters.dat");
	read>>S_0;		//Prezzo iniziale
	read>>K;		//Prezzo concordato
	read>>Sigma;		//Fluttuazioni
	read>>T;		//Tempo finale
	read>>r;		//Rate
	read>>simul;		//Numero di Simulazioni
	read>>passi;		//Numero di Passi per il caso discreto
	read>>Nblocchi;		//numero di blocchi.
	read.close();
	L=simul/Nblocchi;	//lunghezza blocco
	read.close();
	
	Prezzo S(r,Sigma,t, S_0, K, rnd);	//Ogetto Prezzo
	
	Cparz=new double[Nblocchi];		//dato del singolo blocco CALL
	Pparz=new double[Nblocchi];		//dato del singolo blocco PUT
	C2parz=new double[Nblocchi];		//quadrati
	P2parz=new double[Nblocchi];		//""   ""

//Caso diretto.
	ofstream output1("Risultati/DataDirettoC.dat");
	ofstream output2("Risultati/DataDirettoP.dat");
	
	for(int i=0; i<Nblocchi; i++) {
		S.SetParameters(r,Sigma, t, S_0, K);
		Cparz[i]=0.;	//nel caso non fose 0.
		Pparz[i]=0.;
		for(int j=0.; j<L; j++) {	
			Cparz[i]+=exp(-r*T)*max(0., (S.Prezza(T)-K));
			Pparz[i]+=exp(-r*T)*max(0., (K-S.Prezza(T)));
		}
		Cparz[i]=Cparz[i]/L;
		Pparz[i]=Pparz[i]/L;
		C2parz[i]=pow(Cparz[i],2);
		P2parz[i]=pow(Pparz[i],2);
		
	}
	MediaBlocchi(Nblocchi, Cparz, C2parz, &output1); 
	MediaBlocchi(Nblocchi, Pparz, P2parz, &output2); 	
	
	output1.close();
	output2.close();
//***********caso discreto
	output1.open("Risultati/DataDiscretoC.dat");
	output2.open("Risultati/DataDiscretoP.dat");
	
	double DeltaT=T/passi;	//Incremento temporale
	double val;
	
	for(int i=0; i<Nblocchi; i++) {			
		Cparz[i]=0.;	//nel caso non fose 0.
		Pparz[i]=0.;
		for(int j=0.; j<L; j++) {	
			t=0.;
			S.SetParameters(r,Sigma, t, S_0, K);
			val=S.Prezza(DeltaT);	//primo valore
			for(t=DeltaT; t<T; t=t+DeltaT) {
				S.SetParameters(r,Sigma, t, val, K);
				val=S.Prezza((t+DeltaT));
			}
			Cparz[i]+=exp(-r*T)*max(0., (val-K));
			Pparz[i]+=exp(-r*T)*max(0., (K-val));
		}
		Cparz[i]=Cparz[i]/L;
		Pparz[i]=Pparz[i]/L;
		C2parz[i]=pow(Cparz[i],2);
		P2parz[i]=pow(Pparz[i],2);
	}
	MediaBlocchi(Nblocchi, Cparz, C2parz, &output1); 
	MediaBlocchi(Nblocchi, Pparz, P2parz, &output2); 
	
	output1.close();
	output2.close();

return 0;
}

