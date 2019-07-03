#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "random.h"

using namespace std;

int main() {
   
   //Generatore Random.
   Random rnd;
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

//***********************
   int M=100;		//numero di sottointervalli.
   int n=10000;		//numero di lanci.
   int p=100;		//numero di simulazioni.
   double w=1./M;	//larghezza degli intervalli
   double* DataBin=new double[M];	//conterrà il conteggio dei successi.
   double* ChiSquare= new double[p];
   double value=0.;	//variabile di appoggio. Valore della singola estrazione.
   double e=n/M;	//Valore atteso in ogni intervallo.
   double sum=0.;	//Variabile di appoggio per calcolo del chi quadro.
   ofstream output("Risultati/ChiData.dat");

   for(int i=0; i<p; i++) {
	//reset.  	
	sum=0.;
	ChiSquare[i]=0.;
   	//Simulation.
   	for(int j=0; j<n; j++) {
		value=rnd.Rannyu();	//estrae un numero.
		//which bins?
		DataBin[int(value/w)]+=1.;	//lo incasella
	}
	for(int k=0; k<M; k++) {
		sum+=(DataBin[k]-e)*(DataBin[k]-e);
		DataBin[k]=0.;	//resetta per la simulazione successiva
	}
	ChiSquare[i]=sum/e;	
	output<<ChiSquare[i]<<endl;
   }
   output.close();

return 0;
}	




