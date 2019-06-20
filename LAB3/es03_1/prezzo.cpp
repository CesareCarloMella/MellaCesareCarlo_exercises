#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "random.h"
#include "prezzo.h"

using namespace std;

Prezzo :: Prezzo() {
	r=0;
	t=0;
	sigma=0;
	prezzo=0;
	K=0;
	rnd=NULL;
	cout<<"Attenzione, puntatore a Random punta NULL"<<endl;
}

Prezzo :: Prezzo(double rate, double sig, double time, double Sprezzo, double Kprice, Random* rand) {
	r=rate;
	t=time;
	prezzo=Sprezzo;
	sigma=sig;
	K=Kprice;
	rnd=rand;
}

Prezzo :: ~Prezzo() {}

void Prezzo :: SetParameters(double rate, double sig , double time, double Sprezzo, double Kprice) {
	r=rate;
	t=time;
	prezzo=Sprezzo;
	sigma=sig;
	K=Kprice;
}

void Prezzo :: SetRandom(Random * rand) {
	rnd=rand;
}

double Prezzo :: Prezza(double tfinal) {
	double esp=exp((r-0.5*sigma*sigma)*(tfinal-t)+sigma*rnd->Gauss(0,pow((tfinal-t),0.5)));
	prezzo=prezzo*esp;		//restituisce il prezzo al tempo tfinal
	t=tfinal;
	return prezzo;
}
	

