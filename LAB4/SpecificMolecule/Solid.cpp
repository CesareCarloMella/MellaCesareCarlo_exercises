#include <stdlib.h>     // srand, rand: to generate random number
#include <iostream>     // cin, cout: Standard Input/Output Streams Library
#include <fstream>      // Stream class to both read and write from/to files.
#include <cmath>        // rint, pow

using namespace std;

int main(){
	ifstream Read("Parameters.dat");	//legge i parametri dei due elementi.	
	double sigma[2];
	double Tcar[2];
	double m[2];
	int nblocks;
	Read>>sigma[0];
	Read>>Tcar[0];
	Read>>m[0];
	Read>>sigma[1];
	Read>>Tcar[1];
	Read>>m[1];
	Read>>nblocks;		//Attenzione a usare il giusto numero di blocchi.
	Read.close();
	
	
	double KB=1.38064*pow(10,-23);
	ifstream readE, readU, readK, readT, readP;
	ofstream writeE, writeU, writeK, writeT, writeP;
	double *T, *U, *E, *K, *P, *stdevT, *stdevU, *stdevE, *stdevK, *stdevP;
	
	readE.open("Solid/Data/BlockAveE.out");
	readK.open("Solid/Data/BlockAveK.out");
	readU.open("Solid/Data/BlockAveU.out");
	readT.open("Solid/Data/BlockAveT.out");
	readP.open("Solid/Data/BlockAveP.out");
	E=new double[nblocks];
	U=new double[nblocks];
	K=new double[nblocks];
	T=new double[nblocks];
	P=new double[nblocks];
	stdevE=new double[nblocks];
	stdevU=new double[nblocks];
	stdevK=new double[nblocks];
	stdevT=new double[nblocks];
	stdevP=new double[nblocks];
	int indice;
	for(int i=0; i<nblocks; i++) {
		readE>>indice>>E[i]>>stdevE[i];
		readU>>indice>>U[i]>>stdevU[i];
		readK>>indice>>K[i]>>stdevK[i];
		readT>>indice>>T[i]>>stdevT[i];
		readP>>indice>>P[i]>>stdevP[i];
	}
	
	readE.close();
	readK.close();
	readU.close();
	readT.close();
	readP.close();
	for(int i=0; i<2; i++) {
		writeE.open("Solid/Elemento" +to_string(i+1) + "/BlockAveE.out");
		writeU.open("Solid/Elemento" +to_string(i+1) + "/BlockAveU.out");
		writeK.open("Solid/Elemento" +to_string(i+1) + "/BlockAveK.out");
		writeT.open("Solid/Elemento" +to_string(i+1) + "/BlockAveT.out");	
		writeP.open("Solid/Elemento" +to_string(i+1) + "/BlockAveP.out");
		for(int j=0; j<nblocks; j++) {
			writeE<<j+1<<"   "<<E[j]*Tcar[i]<<"   "<<stdevE[j]*Tcar[i]<<endl;
			writeU<<j+1<<"   "<<U[j]*Tcar[i]<<"   "<<stdevU[j]*Tcar[i]<<endl;
			writeK<<j+1<<"   "<<K[j]*Tcar[i]<<"   "<<stdevK[j]*Tcar[i]<<endl;
			writeT<<j+1<<"   "<<T[j]*Tcar[i]<<"   "<<stdevT[j]*Tcar[i]<<endl;
			writeP<<j+1<<"   "<<P[j]*Tcar[i]*KB*(pow(10,22)/1.01325)/pow(sigma[i],3)<<"   "<<stdevP[j]*Tcar[i]*KB*(pow(10,22)/1.01325)/pow(sigma[i],3)<<endl;
		}
		writeE.close();
		writeU.close();
		writeK.close();
		writeT.close();
		writeP.close();
	}
  return 0;
}
