#include <stdlib.h>     // srand, rand: to generate random number
#include <iostream>     // cin, cout: Standard Input/Output Streams Library
#include <fstream>      // Stream class to both read and write from/to files.
#include <cmath>        // rint, pow
#include "MolDyn.h"

using namespace std;

int main(){ 
	int iprint, n_part, nstep, nblocks, nbins;
	double temp, rho, rcut, delta, bin_size, vol, box;
	ifstream ReadInput("input.dat");
	//Parametri
	ReadInput>>temp;			//temperatura
	ReadInput>>n_part;			//numero di particelle
	ReadInput>>rho;				//densità
	ReadInput>>rcut;			//cut per il computo della forza
	ReadInput>>delta;			//step temporale
	ReadInput>>nstep;			//numero di step
	ReadInput>>iprint;			//stampa per controllo
	ReadInput>>nblocks;			//numero di blocchi
	ReadInput>>nbins;			//numero di bin per la g(r)
	int L=nstep/nblocks;			//lunghezza del blocco
	vol = (double)n_part/rho;		//volume
 	box = pow(vol,1.0/3.0);			//lato
	bin_size = (box/2.0)/(double)nbins;	//dimensione di un bin
	
	double *val, *T, *U, *K, *E, *P, *ave,*ave2, *stdev, *g, *gAve, *gAve2, r, errorG;
	ave=new double[5];	//sono 5 osservabili
	ave2=new double[5];
	val=new double[5];
	stdev=new double[5];
	for(int i=0; i<5; i++) {
		ave[i]=0.;
		ave2[i]=0.;
	}
	
	g=new double[nbins];	//sono nbins bin
	gAve=new double[nbins];
	gAve2=new double[nbins];
	for(int i=0; i<nbins; i++) {
		g[i]=0.;
		gAve[i]=0.;
		gAve2[i]=0.;
	}
	T=new double[nstep];	
	U=new double[nstep];
	K=new double[nstep];
	E=new double[nstep];
	P=new double[nstep];
	MolDyn Sim(temp, n_part, rho, rcut,delta,nstep, nbins);	//oggetto che simula tutto.
	Sim.Input();		//Inizializza
	int nconf =1;
	
//Simulazione********************************************+
	
	for( int istep=0; istep<nstep; istep++) {
		Sim.Move();	//Verlet algorithm
		if(istep%iprint == 0)  cout <<"Number of time-step: "<<istep <<endl;
		if(istep%10 == 0){
			Sim.Measure();	//Misura le proprietà
			Sim.ConfXYZ(nconf);
			nconf +=1;
		}
		//Misure istantanee
		T[istep]=Sim.GetT();	//A ogni passo chiede tutto
		U[istep]=Sim.GetU();
		K[istep]=Sim.GetK();
		E[istep]=Sim.GetE();
		P[istep]=Sim.GetP();
		
		for(int i=0; i<nbins; i++) g[i]+=Sim.GetG(i);
		if((istep%L)==0) {
			for( int i=0; i<nbins; i++) {
				g[i]/=(double)L;		//media sul blocco
				gAve[i]+=g[i];		//sommo
				gAve2[i]+=g[i]*g[i];
				g[i]=0.;		//reset
			}
		}
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
			val[k]/=(double)(L);			//valore del blocco a cui sono arrivato
			ave[k]+=val[k];				//aggiorno somma
			ave2[k]+=val[k]*val[k];			
			if( i==0 ) {
				stdev[k]=0.;	
			} else {	
				stdev[k]=sqrt((ave2[k]/(double)(i+1)-pow(ave[k]/(double)(i+1),2.))/(double)(i));		//deviazione standard tra i primi i+1 blocchi.
			}
		}	
		writeT<<val[0]<<endl;
		writeU<<val[1]<<endl;
		writeK<<val[2]<<endl;
		writeE<<val[3]<<endl;		//medie di ogni blocco.
		writeP<<val[4]<<endl;
		wAveT<<i+1<<"   "<<ave[0]/double(i+1)<<"   "<<stdev[0]<<endl;		//medie progressive
		wAveU<<i+1<<"   "<<ave[1]/double(i+1)<<"   "<<stdev[1]<<endl;
		wAveK<<i+1<<"   "<<ave[2]/double(i+1)<<"   "<<stdev[2]<<endl;
		wAveE<<i+1<<"   "<<ave[3]/double(i+1)<<"   "<<stdev[3]<<endl;
		wAveP<<i+1<<"   "<<ave[4]/double(i+1)<<"   "<<stdev[4]<<endl;
	}
	
	ofstream writeG("progressivi/output.G.100");
	for(int i=0; i<nbins; i++) {
		gAve[i]/=nblocks;
		gAve2[i]/=nblocks;
		if(i==0) { 
			errorG=0.;
		} else {
			errorG=sqrt((gAve2[i]-gAve[i]*gAve[i])/i);
		}
		r=double(i)*bin_size;
		writeG<<r<<"     "<<gAve[i]<<"     "<<errorG<<endl;
	}
	
	
	writeG.close();
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
