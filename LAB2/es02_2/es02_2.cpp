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
	for(int i=0; i<Nsim; i++) {
		RWvett[i].SetLenght(l);
		RWvett[i].SetGenerator(rnd);
	}				//ora ho Nsim oggetti Walk inizializzati nel modo corretto.
	
	double *val, *val2, *prog, *prog2;	//conterranno, per ogni passo, valori, quadrati, somme dei valori e somme dei quadrati dei blocchi.
	val=new double[N];
	val2=new double[N];
	prog=new double[N];
	prog2=new double[N];
	double sum=0.;
	ofstream output("Risultati/LatticeData.dat");
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
		output<<i+1<<"   "<<result<<"   "<<stdev<<endl;
	}
	
	output.close();
	
//********************
	output.open("Risultati/DirData.dat");
	double *q=new double[3];
	ofstream posizioni1("posizioni/Posizioni1.dat");
	ofstream posizioni2("posizioni/Posizioni10.dat");
	ofstream posizioni3("posizioni/Posizioni30.dat");
	ofstream posizioni4("posizioni/Posizioni45.dat");
	
	for(int i=0; i<N; i++) {
		val[i]=0.;
		val2[i]=0.;
		prog[i]=0.;
		prog2[i]=0.;
	}
	for(int i=0; i<Nsim; i++) {
		RWvett[i].AzzeraCammino();
	}
	for(int i=0; i<passi; i++) {		//scorrendo sul numero di passi
		for(int j=0; j<N; j++) {
			sum=0.;
			for(int k=j*L; k<j*L+L; k++) {
				RWvett[k].RandomDirStep();	//passo random
				if(k<500 and i==0) {
					q=RWvett[k].GetPosition();
					posizioni1<<i<<"   "<<q[0]<<"   "<<q[1]<<"   "<<q[2]<<endl;
				}
				if(k<500 and i==9) {
					q=RWvett[k].GetPosition();
					posizioni2<<i<<"   "<<q[0]<<"   "<<q[1]<<"   "<<q[2]<<endl;
				}
				if(k<500 and i==29) {
					q=RWvett[k].GetPosition();
					posizioni3<<i<<"   "<<q[0]<<"   "<<q[1]<<"   "<<q[2]<<endl;
				}
				if(k<500 and i==44) {
					q=RWvett[k].GetPosition();
					posizioni4<<i<<"   "<<q[0]<<"   "<<q[1]<<"   "<<q[2]<<endl;
				}
				sum+=RWvett[k].SquareDist();	//somma il passo.
			}
			val[j]=sum/L;	//valore medio nel blocco.
			val2[j]=pow(val[j],2);	//valore medio al quadrato nel blocco.
			
			if(j==0) {
				prog[j]=val[j];
				prog2[j]=val2[j];
			} else {
				prog[j]=prog[j-1]+val[j];	//sommo.
				prog2[j]=prog2[j-1]+val2[j];	//sommo.
			}
			
		}
		result=pow(prog[N-1]/double(N),0.5);	//valore da graficare
		stdev=pow((prog2[N-1]/double(N)-pow(prog[N-1]/double(N),2))/double(N-1),0.5);
		stdev=stdev/(2.*result);
		output<<i+1<<"   "<<result<<"   "<<stdev<<endl;
	}	
	output.close();
	posizioni1.close();
	posizioni2.close();
	posizioni3.close();
	posizioni4.close();

return 0;
}
