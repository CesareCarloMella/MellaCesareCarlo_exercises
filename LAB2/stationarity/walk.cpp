#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "random.h"
#include "walk.h"

using namespace std;

//costruttori e distruttori
Walk :: Walk(){
	q=0.;
	rnd=NULL;
	a1=1.;
}

Walk :: Walk(double lenght, Random* r, double a) {
	q=0.;
	l=lenght;
	rnd=r;
	a1=a;
}

Walk :: ~Walk(){}

// methods
void Walk ::  RandomLatticeStep(){			//passo random in una delle direzioni cartesiane di lunghezza l
	double r;
	r=rnd->Rannyu();
	//estraggo la direzione 
	if(r>=0. and r<0.5) {
		q=a1*q+1.*l;
	} 
	if(r>=0.5 and r<1.) {
		q=a1*q-1.*l;
	}		
}


//imposta lunghezza
void Walk :: SetLenght(double lenght) {
	l=lenght;
}
//imposta il generatore
void Walk :: SetGenerator(Random * r) {
	if(rnd!=NULL) {
		delete rnd;
		rnd=r;
	} else {
		rnd=r;
	}
}
	

void Walk :: SetA1(double a) {
	a1=a;
}

double Walk :: Distance() {				//restituisce la distanza dall'origine.
return fabs(q);
}

double Walk :: SquareDist() {				//restituisce il modulo quadro.
return q*q;
}

void Walk :: AzzeraCammino() {				//resetta il random walk
	q=0.;
}


double Walk :: GetPosition() {
	return q;
}

	

