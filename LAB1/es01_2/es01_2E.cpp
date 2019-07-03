#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "random.h"


using namespace std;


int main() {

//Generatore Random.
   Random rnd;
   int seed[4];
   int p1, p2;
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
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;
//******************************

   int* N=new int[4];		//numero di punti per poter costruire la variabile "media"
   N[0]=1;
   N[1]=2;
   N[2]=10;
   N[3]=100;
   int M=10000;				//numero di campionamenti.
   int bin;				//numero di intervalli.
   double range=1.;			//range del grafico.
   double w;				//larghezza dei singoli intervalli.
   double *exp=NULL;
   double sum=0.;
   double lambda=1.;
   ifstream read("Parametri/DataInE.dat");	//file con numero di bin e range.
   double markExp;			//segnaposto per poter mettere i dati
   double ExpAve=1./lambda;		//Valore di aspettazione per l'esponenziale.
   
   ofstream outputE1("Risultati/ExponentialData1.dat");
   ofstream outputE2("Risultati/ExponentialData2.dat");
   ofstream outputE3("Risultati/ExponentialData10.dat");
   ofstream outputE4("Risultati/ExponentialData100.dat");
   
   for(int i=0; i<4; i++) {
   	read>>bin;
   	read>>range;
   	w=range/bin;	
   	if(exp!=NULL) {
   		delete []exp;
   	}
   	exp=new double[bin];
   
   	for(int b=0; b<bin; b++) {
   		exp[b]=0.;
   	}
 	for(int j=0; j<M; j++) {
 		sum=0.;  
 	   	for(int k=0; k<N[i]; k++) {		//qua distingue
 	   	sum+=rnd.Exp(lambda);
   		}
   		sum=sum/N[i];		//media
   		for(int b=0; b<bin; b++) {
   			markExp=(ExpAve-range/2.)+b*w;
   			if(sum>=(markExp-w/2.) and sum<(markExp+w/2.)) {
   				exp[b]++;
   			}
   		}
   	}
   		if(i==0) { 
   			for(int b=0; b<bin; b++) {
   				markExp=(ExpAve-range/2.)+b*w;
   				outputE1<<markExp<<"   "<<exp[b]<<endl;
   			}
   		}	
   		if(i==1) { 
   			for(int b=0; b<bin; b++) {
   				markExp=(ExpAve-range/2.)+b*w;
   				outputE2<<markExp<<"   "<<exp[b]<<endl;
   			}
   		}
   		if(i==2) { 
   			for(int b=0; b<bin; b++) {
   				markExp=(ExpAve-range/2.)+b*w;
   				outputE3<<markExp<<"   "<<exp[b]<<endl;
   			}
   		}	
   		if(i==3) { 
   			for(int b=0; b<bin; b++) {
   				markExp=(ExpAve-range/2.)+b*w;
   				outputE4<<markExp<<"   "<<exp[b]<<endl;
   			}
   		}
	}

   read.close();
   outputE1.close();
   outputE2.close();
   outputE3.close();
   outputE4.close();

return 0;
}

   
   
