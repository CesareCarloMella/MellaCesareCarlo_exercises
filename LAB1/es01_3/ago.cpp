//***********************Implementazione della classe ago************************

#include "ago.h"
#include "random.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>


using namespace std;
//costruttore senza argomento.
Ago :: Ago(){
   rnd=new Random();
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd->SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;
   
   L=0.;		
   Theta=0.;
   Mid=0.;

}

//costyruttore con lunghezza ago.
Ago :: Ago(double l){
   rnd=new Random();
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd->SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;
   
   L=l;			//lunghezza dell'ago.	
   Theta=0.;
   Mid=0.;

}
//distruttore.
Ago :: ~Ago(){
	delete rnd;
}

//Methods

void Ago :: SetLenght(double l) {	
	L=l;
}

void Ago ::Throw(double w, double d){ 	//questo è un lancio.
	double x,y;
	x=rnd->Rannyu(-1.,1.);
	y=rnd->Rannyu(-1.,1.);
	if((x*x+y*y)<1.) {
		Theta=acos(x/(pow(x*x+y*y,0.5)));	//estrae solo tra 0 e pi-greco, l'algoritmo funziona lo stesso perchè uso il punto medio dell'ago.
	}	
	Mid=rnd->Rannyu((d/2.),(w-d/2.));		//estrae il punto medio dell'ago. Lo estrae saltando le prime righe perchè verrebbero contate a metà.
}
 
double Ago :: GetMid(void) {				//potrebbero essere utili.
	return Mid;
}

double Ago :: GetTheta(void) {				
	return Theta;
}

double Ago :: GetPi(double m, double d, double w) {		//esegue e restituisce una stima di pi-greco. 
	int Ns=0;		//numero di successi		//passare numero di lanci da usare m, la distanza tra le righe del piano d, la dimensione del piano w.
	double pr;		
	int n;			//numero di passi sul piano.
	n=(int)w/d;		
	for(int k=0; k<m; k++) {
		this->Throw(w,d);		//solo estrazione random.
		Mid=this->GetMid();		//Punto medio dell'ago.
		Theta=this->GetTheta();		//angolo
		pr=(L/2.)*sin(Theta);	//proiezione dell'ago.
		for(int a=0; a<n; a++) {	//verifica le intersezione.
			if(Mid<=(d*(a+1)) and Mid>(d*a)) {
				if((pr)>=(d*(a+1)-Mid) and a!=n) {
				Ns++;
				}
				if((pr)>=(Mid-d*a) and a!=0) {
					Ns++;
				}
			}	
		}	
	}		
return (2*L*m)/(Ns*d);	//pi greco.
}			








