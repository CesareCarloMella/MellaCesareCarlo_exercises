
#include "Function.h"


using namespace std;
//*************************************************PER METROPOLIS************************************************************
position T_Random(Random* rnd, double l) {
	position proposta;
	proposta.x=rnd->Rannyu(-l,l);
	proposta.y=rnd->Rannyu(-l,l);
	proposta.z=rnd->Rannyu(-l,l);
	return proposta;
}

void A_AccRej(Random* rnd, double prob, position* s, position* p, int* th, int* acc) { 
        *th+=1;          //il passo e' fatto.
        double b=rnd->Rannyu();
        if(b<prob) {
                *acc+=1;
                s->x+=p->x;
                s->y+=p->y;
                s->z+=p->z;
        }
}      
//*************************************************PER METROPOLIS************************************************************
          
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


