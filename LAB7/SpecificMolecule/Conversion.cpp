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
	ifstream readU, readP, readG;
	double app;
	ofstream  writeU, writeP, writeG;
	double  *U, *P, *r, *g,  *stdevU, *stdevP, *stdevG;
	
	
	readU.open("DataInput/output.epot.0");
	readP.open("DataInput/output.pres.0");
	readG.open("DataInput/output.G.100");
	U=new double[nblocks];
	P=new double[nblocks];
	r=new double[nbins];
	g=new double[nbins];
	stdevU=new double[nblocks];
	stdevP=new double[nblocks];
	stdevG=new double[nbins];
	int indice;
	for(int i=0; i<nblocks; i++) {
		readU>>indice>>app>>U[i]>>stdevU[i];
		readP>>indice>>app>>P[i]>>stdevP[i];
	}
	for(int i=0; i<nbins; i++) {
		readG>>r[i]>>g[i];
		readG>>stdevG[i];
	}
	
	readU.close();
	readP.close();
	readG.close();
	writeU.open("DataOutput/conversion.epot.0");
	writeP.open("DataOutput/conversion.pres.0");
	writeG.open("DataOutput/conversion.G.100");
	for(int i=0; i<nblocks; i++) {
		writeU<<i+1<<"   "<<U[i]*Tcar<<"   "<<stdevU[i]*Tcar<<endl;
		writeP<<i+1<<"   "<<P[i]*Tcar*KB*(pow(10,27)/101325.)/pow(sigma,3)<<"   "<<stdevP[i]*Tcar*KB*(pow(10,27)/101325.)/pow(sigma,3)<<endl;
	}
	for(int i=0; i<nbins; i++) {
		writeG<<r[i]*sigma<<"     "<<g[i]<<"     "<<stdevG[i]<<endl;
	}
	writeU.close();
	writeP.close();
	writeG.close();
	
  return 0;
}
