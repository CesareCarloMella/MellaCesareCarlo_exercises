#include <stdlib.h>     // srand, rand: to generate random number
#include <iostream>     // cin, cout: Standard Input/Output Streams Library
#include <fstream>      // Stream class to both read and write from/to files.
#include <cmath>        // rint, pow
#include "MolDyn.h"

using namespace std;

int main(){ 
	int iprint, n_part, nstep, nblocks;
	double temp, rho, rcut, delta;
	ifstream ReadInput("input.dat");
	ReadInput>>temp;
	ReadInput>>n_part;
	ReadInput>>rho;
	ReadInput>>rcut;
	ReadInput>>delta;
	ReadInput>>nstep;
	ReadInput>>iprint;
	ReadInput>>nblocks;
	double L=nstep/nblocks;		//lunghezza del blocco.
	
	
	double *val, *T, *U, *K, *E, *P, *ave,*ave2, *stdev;
	ave=new double[5];
	ave2=new double[5];
	val=new double[5];
	stdev=new double[5];
	T=new double[nstep];
	U=new double[nstep];
	K=new double[nstep];
	E=new double[nstep];
	P=new double[nstep];
	MolDyn Sim(temp, n_part, rho, rcut,delta,nstep);	//oggetto che simula tutto.
	Sim.Input();
	int nconf =1;
	for( int istep=1; istep<=nstep; istep++) {
		Sim.Move();	//Verlet algorithm
		if(istep%iprint == 0)  cout <<"Number of time-step: "<<istep <<endl;
		if(istep%10 == 0){
			Sim.Measure();	//Misura le proprietà
			Sim.ConfXYZ(nconf);
			nconf +=1;
		}
		T[istep]=Sim.GetT();
		U[istep]=Sim.GetU();
		K[istep]=Sim.GetK();
		E[istep]=Sim.GetE();
		P[istep]=Sim.GetP();
	}
	ofstream writeT, writeU, writeK, writeE,writeP, wAveT, wAveU,wAveK,wAveE, wAveP;
	writeT.open("blocchi/T.out");
	writeU.open("blocchi/U.out");
	writeK.open("blocchi/K.out");
	writeE.open("blocchi/E.out");
	writeP.open("blocchi/P.out");		//MEDIE DEI SINGOLI BLOCCHI.
	wAveT.open("progressivi/BlockAveT.out");
	wAveU.open("progressivi/BlockAveU.out");
	wAveK.open("progressivi/BlockAveK.out");
	wAveE.open("progressivi/BlockAveE.out");
	wAveP.open("progressivi/BlockAveP.out");//MEDIE PROGRESSIVE ALL'AUMENTARE DEL NUMERO DI BLOCCHI.

	//divisione in blocchi
	for(int i=0; i<nblocks; i++) {	//i indice di blocco.
		for(int j=0; j<5; j++) {
			val[j]=0.;
		}
		
		for(int j=i*L; j<L*(i+1); j++) {
			val[0]+=T[j];
			val[1]+=U[j];
			val[2]+=K[j];
			val[3]+=E[j];
			val[4]+=P[j];
		}
		for(int k=0; k<5; k++) {
			val[k]/=L;		//valore del blocco a cui sono arrivato
			ave[k]=((ave[k]*i)+val[k])/(i+1);	//media tra i primi i+1 blocchi
			ave2[k]+=val[k]*val[k];			
			if( i==0 ) {
				stdev[k]=0.;	
			} else {	
				stdev[k]=sqrt((ave2[k]/(i+1)-pow(ave[k]/(i+1),2))/(i));		//deviazione standard tra i primi i+1 blocchi.
			}
		}	
		writeT<<val[0]<<endl;
		writeU<<val[1]<<endl;
		writeK<<val[2]<<endl;
		writeE<<val[3]<<endl;		//medie di ogni blocco.
		writeP<<val[4]<<endl;
		wAveT<<i+1<<"   "<<ave[0]<<"   "<<stdev[0]<<endl;		//medie progressive
		wAveU<<i+1<<"   "<<ave[1]<<"   "<<stdev[1]<<endl;
		wAveK<<i+1<<"   "<<ave[2]<<"   "<<stdev[2]<<endl;
		wAveE<<i+1<<"   "<<ave[3]<<"   "<<stdev[3]<<endl;
		wAveP<<i+1<<"   "<<ave[4]<<"   "<<stdev[4]<<endl;
	}
	
	
	delete []T; 
	delete []U; 
	delete []K;
	delete []E;
	delete []P;
	delete []ave;
	delete []ave2;
	delete []val;
	writeT.close();
	writeU.close();
	writeK.close();
	writeE.close();
	writeP.close();
	wAveT.close();
	wAveU.close();
	wAveK.close();
	wAveE.close();
	wAveP.close();
	//configurazione finale.
	Sim.ConfFinal();
  return 0;
}
