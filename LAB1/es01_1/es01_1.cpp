#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "random.h"
#include "MediaBlocchi.h"

using namespace std;


int main() {

//Generatore Random.
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

//********************+
   int N, L, M;				//Numero blocchi, Lunghezza blocchi, M valori totali.
   ifstream read("Parameters.dat");
   read>>M;
   read>>N;
   L=M/N;				//Lunghezza blocchi.
   read.close();

   double sum=0.;			//Variabile di appoggio.
   double *ave= new double[N];		//Valor medio in singolo blocco.
   double *ave2= new double[N];		//Quadrato del contenuto della precedente.
  
   //Part 1: avarage value.  **************************************************
   ofstream output("Risultati/RandomData.dat");	//stampa qui i risultati.
   //estrazione random e riempimento vettori di dati.
   for(int i=0; i<N; i++) {	
   	sum=0.;
   	for(int j=0; j<L; j++) {
   		sum+=rnd.Rannyu();	//somma variabili random.
   	}
   	ave[i]=sum/L;	//media del blocco.
   	ave2[i]=pow(ave[i],2);	//salva i quadrati.
   }
   
   MediaBlocchi(N, ave, ave2, &output);	//numero di blocchi, medie sui blocchi, quadrati delle medie sui blocchi, aggancio per output.
   output.close();
  
   //Part 2: sigma square. ******************************************************//identica alla prima parte ma con valore da mediare diverso.
   output.open("Risultati/SigmaData.dat");
    for(int i=0; i<N; i++) {
   	sum=0.;
   	for(int j=0; j<L; j++) {
   		sum+=pow((rnd.Rannyu()-0.5),2);
   	}
   	ave[i]=sum/L;		//media blocco.
   	ave2[i]=pow(ave[i],2);	//media blocco al quadrato.
   }
   MediaBlocchi(N, ave, ave2, &output);	//numero di blocchi, medie sui blocchi, quadrati delle medie sui blocchi, aggancio per output.
   output.close();
  

return 0;
}
