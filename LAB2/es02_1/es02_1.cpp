#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "random.h"
#include "MediaBlocchi.h"


using namespace std;


int main() {
	int M, N;		//numero di punti, numero di blocchi
	int L;			//lunghezza del blocco
//****************************
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
//************************************
//parametri del calcolo
	input.open("Parameters.dat");
	input>>M;		//Numero di step monte carlo
	input>>N;		//numero di blocchi
	L=M/N;				//Lunghezza blocchi.
	input.close();
//variabili che mi servono
	double sum, x;				//variabili di appoggio.
	double *val=new double[N];		//contiene le stime dell'integrale per blocco
	double *val2=new double[N];		//quadrati

//Campionamento uniforme.
	for(int i=0; i<N; i++) {
		sum=0.;
		for(int j=0; j<L; j++) {
			x=rnd.Rannyu();		//punto a caso
			sum+=(M_PI/2.)*cos(M_PI/2.*x);	//sommo i valori della funzione
		}
		val[i]=sum/L;		//stima integrale
		val2[i]=pow(val[i],2);	//quadrati
	}
	
	ofstream output("DataU.dat");
	MediaBlocchi(N, val, val2, &output);
	output.close();
	
//seconda parte: Importance sampling

	
	for(int i=0; i<N; i++) {
		sum=0.;
		for(int j=0; j<L; j++) {
			x=1-pow(1-rnd.Rannyu(),0.5);		//punto distribuito come una retta
			sum+=((M_PI/4.)/(1-x))*cos(M_PI/2.*x);	//sommo i valori della funzione
		}
		val[i]=sum/L;	// stima integrale
		val2[i]=pow(val[i],2);	//quadrati
	}
	
	output.open("DataI.dat");
	MediaBlocchi(N, val, val2, &output);
	output.close();
	

return 0;
}
