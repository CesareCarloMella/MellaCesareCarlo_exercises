#include <stdlib.h>     // srand, rand: to generate random number
#include <iostream>     // cin, cout: Standard Input/Output Streams Library
#include <fstream>      // Stream class to both read and write from/to files.
#include <cmath>        // rint, pow


using namespace std;

int main(){
	ifstream Read("Parameters.dat");	//legge i parametri dei due elementi.	
	double sigma;
	double Tcar;
	double m;
	int nblocks, nbins;
	Read>>sigma;
	Read>>Tcar;
	Read>>m;
	Read>>nblocks;		//Attenzione a usare il giusto numero di blocchi.
	Read>>nbins;
	Read.close();
	
	
	
	
	double KB=1.38064*pow(10,-23);
	ifstream readE, readU, readK, readT, readP, readG;
	ofstream writeE, writeU, writeK, writeT, writeP,writeG;
	double *T, *U, *E, *K, *P, *r, *g, *stdevT, *stdevU, *stdevE, *stdevK, *stdevP, *stdevG;
	
	readE.open("DataInput/BlockAveE.out");
	readK.open("DataInput/BlockAveK.out");
	readU.open("DataInput/BlockAveU.out");
	readT.open("DataInput/BlockAveT.out");
	readP.open("DataInput/BlockAveP.out");
	readG.open("DataInput/output.G.100");
	E=new double[nblocks];
	U=new double[nblocks];
	K=new double[nblocks];
	T=new double[nblocks];
	P=new double[nblocks];
	r=new double[nbins];
	g=new double[nbins];
	stdevE=new double[nblocks];
	stdevU=new double[nblocks];
	stdevK=new double[nblocks];
	stdevT=new double[nblocks];
	stdevP=new double[nblocks];
	stdevG=new double[nbins];
	int indice;
	for(int i=0; i<nblocks; i++) {
		readE>>indice>>E[i]>>stdevE[i];
		readU>>indice>>U[i]>>stdevU[i];
		readK>>indice>>K[i]>>stdevK[i];
		readT>>indice>>T[i]>>stdevT[i];
		readP>>indice>>P[i]>>stdevP[i];
	}
	for(int i=0; i<nbins; i++) {
		readG>>r[i]>>g[i];
		readG>>stdevG[i];
	}
	
	
	readE.close();
	readK.close();
	readU.close();
	readT.close();
	readP.close();
	readG.close();
	writeE.open("DataOutput/BlockAveE.out");
	writeU.open("DataOutput/BlockAveU.out");
	writeK.open("DataOutput/BlockAveK.out");
	writeT.open("DataOutput/BlockAveT.out");	
	writeP.open("DataOutput/BlockAveP.out");
	writeG.open("DataOutput/conversion.G.100");
	for(int i=0; i<nblocks; i++) {
		writeE<<i+1<<"   "<<E[i]*Tcar<<"   "<<stdevE[i]*Tcar<<endl;
		writeU<<i+1<<"   "<<U[i]*Tcar<<"   "<<stdevU[i]*Tcar<<endl;
		writeK<<i+1<<"   "<<K[i]*Tcar<<"   "<<stdevK[i]*Tcar<<endl;
		writeT<<i+1<<"   "<<T[i]*Tcar<<"   "<<stdevT[i]*Tcar<<endl;
		writeP<<i+1<<"   "<<P[i]*Tcar*KB*(pow(10,27)/101325.)/pow(sigma,3)<<"   "<<stdevP[i]*Tcar*KB*(pow(10,27)/101325.)/pow(sigma,3)<<endl;
	}
	for(int i=0; i<nbins; i++) {
		writeG<<r[i]*sigma<<"     "<<g[i]<<"     "<<stdevG[i]<<endl;
	}
	writeE.close();
	writeU.close();
	writeK.close();
	writeT.close();
	writeP.close();
	writeG.close();
  return 0;
}
