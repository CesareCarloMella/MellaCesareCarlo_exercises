#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "random.h"
#include "walk.h"


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
//***********************+

	int passi, Nsim, N, L;		//numero di passi, numero di simulazioni per ogni passo, Numero di blocchi, Lunghezza blocco.
	double l;			//lunghezza passo
	input.open("Parameters.dat");
	input>>passi;	//numero di passi
	input>>Nsim;	//numero di simulazioni totali!
	input>>l;	//lunghezza passo.
	input>>N;	//numero di blocchi
	input.close();
	L=Nsim/N;
	Walk* RWvett=new Walk[Nsim];	//Nsim oggetti Walk.
	
	
	double a1;
	double *val, *val2, *prog, *prog2;	//conterranno, per ogni passo, valori, quadrati, somme dei valori e somme dei quadrati dei blocchi.
	val=new double[N];
	val2=new double[N];
	prog=new double[N];
	prog2=new double[N];
	double sum=0.;
	
	
	cout<<"Inserisci il parametro del modello: "<<endl;
	cin>>a1;
	
	for(int i=0; i<Nsim; i++) {
		RWvett[i].SetLenght(l);
		RWvett[i].SetGenerator(rnd);
		RWvett[i].SetA1(a1);
	}				//ora ho Nsim oggetti Walk inizializzati nel modo corretto.
	ofstream output("Risultati/LatticeData"+ to_string(a1)+".dat");
	ofstream output1("Risultati/Position"+ to_string(a1) + ".dat");
	double stdev;
	double result;
	
	for(int i=0; i<passi; i++) {		//scorrendo sul numero di passi
		for(int j=0; j<N; j++) {
			sum=0.;
			for(int k=j*L; k<j*L+L; k++) {
				RWvett[k].RandomLatticeStep();	//passo random
				sum+=RWvett[k].SquareDist(); //somma il passo.
			}
			val[j]=sum/(double)L;	//valore medio nel blocco.
			val2[j]=val[j]*val[j];	//valore medio al quadrato nel blocco.
			if(j==0) {
				prog[j]=val[j];
				prog2[j]=val2[j];
			} else {
				prog[j]=prog[j-1]+val[j];	//sommo.
				prog2[j]=prog2[j-1]+val2[j];	//sommo.
			}
		}
		result=pow(prog[N-1]/(double(N)),0.5);	//valore da graficare
		stdev=pow((prog2[N-1]/double(N)-pow(prog[N-1]/double(N),2))/(double)(N-1),0.5);
		stdev=stdev/(2*result);
		if(i<1000) {
			output<<i+1<<"   "<<result<<"   "<<stdev<<endl;
		}
		if(i%5==0) {
		output1<<i<<"   "<<RWvett[0].GetPosition()<<endl;
		}
	}
	
	output.close();
	output1.close();
	
//********************

return 0;
}
