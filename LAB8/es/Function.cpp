
#include "Function.h"


using namespace std;
//*************************************************PER METROPOLIS************************************************************
double T_Random(Random* rnd, double l) {
	double proposta;
	proposta=rnd->Rannyu(-l,l);
	return proposta;
}

double T_Gauss(Random * rnd, double l) {
	double proposta;
	proposta=rnd->Gauss(0,l);
	return proposta;
}

void A_AccRej(Random* rnd, double prob, double* s, double* p, int* th, int* acc) { 
        *th+=1;          //il passo e' fatto.
        double b=rnd->Rannyu();
        if(b<prob) {
                *acc+=1;
                *s+=*p;
        }
}      
//*************************************************PER METROPOLIS************************************************************
/*          
//DENSITÀ PROBABILITÀ AUTOFUNZIONI IDROGENO
//1S         
double prob100(position p) {
	double pr=exp(-2.*sqrt(p.x*p.x+p.y*p.y+p.z*p.z));
	pr=pr/M_PI;
	return pr;
}
//2P
double prob210(position p) {
        double pr=p.z*p.z*exp(-sqrt(p.x*p.x+p.y*p.y+p.z*p.z));
        pr=pr/(32.*M_PI);
        return pr;
}


double getRadius(position s){
        return sqrt(s.x*s.x+s.y*s.y+s.z*s.z);
}
*/
double Double_Gauss(double x, double mu, double sigma) {
	double psi=exp(-(pow((x-mu),2.))/(2*sigma*sigma))+exp(-(pow((x+mu),2.))/(2*sigma*sigma));		//funzione d'onda.
	psi*=psi;		//Modulo quadro
	return psi;
}



double Integrand(double x, double mu, double sigma) {	//integranda
	double var=sigma*sigma;
	double val=(x*x*x*x-2.5*x*x)-0.5*((x*x+mu*mu)/(var*var)-1/(var))+(mu*x)/(var*var)*tanh(mu*x/var);
	return val;
}

		
	
	
	
	
	
	


