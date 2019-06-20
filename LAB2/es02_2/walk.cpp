#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "random.h"
#include "walk.h"

using namespace std;

//costruttori e distruttori
Walk :: Walk(){
	q=new double[3];
	q[0]=0.;
	q[1]=0.;
	q[2]=0.;
	rnd=NULL;
}

Walk :: Walk(double lenght, Random* r) {
	q=new double[3];
	q[0]=0.;
	q[1]=0.;
	q[2]=0.;
	l=lenght;
	rnd=r;
}

Walk :: ~Walk(){}

// methods
void Walk ::  RandomLatticeStep(){			//passo random in una delle direzioni cartesiane di lunghezza l
	int dir=0;					//direzioni 1,2,3 sono, rispettiavamente, x,y,z
	double r;
	r=rnd->Rannyu();
	//estraggo la direzione 
	if(r>=0. and r<(1./3.)) {
		dir=1;	//cioe' x
	} 
	if(r>=(1./3.) and r<(2./3.)) {
		dir=2;	//cioe' y
	}
	if(r>=(2./3.)) {
		dir=3; //cioe' z
	}
	//estraggo avanti o indietro.
	r=rnd->Rannyu();
	if(r>=(1/2.)) {
		r=1.*l;
	} else {
		r=-1.*l;
	}
	q[dir-1]+=r;		
}


void Walk :: RandomDirStep() {
	double fi,theta;
	fi=rnd->Rannyu(0, 2*M_PI);
	theta=acos(1-2.*rnd->Rannyu(0,1));
	q[0]+=sin(theta)*cos(fi)*l;
	q[1]+=sin(theta)*sin(fi)*l;
	q[2]+=cos(theta)*l;
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
	


double Walk :: Distance() {				//restituisce la distanza dall'origine.
	double d=q[0]*q[0]+q[1]*q[1]+q[2]*q[2];
 return pow(d,0.5);
}

double Walk :: SquareDist() {				//restituisce il modulo quadro.
	double d=q[0]*q[0]+q[1]*q[1]+q[2]*q[2];
return d;
}

void Walk :: AzzeraCammino() {				//resetta il random walk
	q[0]=0.;
	q[1]=0.;
	q[2]=0.;
}


double* Walk :: GetPosition() {
	return q;
}

	

