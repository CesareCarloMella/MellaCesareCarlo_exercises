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

//*********************************+

   int* N=new int[4];			//Numero di punti per calcolare la variabile "media"
   N[0]=1;	
   N[1]=2;
   N[2]=10;
   N[3]=100;	
   int M=10000;				//numero di campionamenti.
   int bin;				//numero di intervalli.
   double range;			//range del grafico.
   double w;				//larghezza dei singoli intervalli.
   double *uniform=NULL;		//dati della distribuzione.
   double sum=0.;
   ifstream read("Parametri/DataInU.dat");	//file con numero di bin e range.
   double markUnif;				//segnaposto per poter mettere i dati.
   double UnifAve=0.5;				//valore medio della distribuzione uniforme.

   //apro i file per i risultati da graficare.
   ofstream outputU1("Risultati/UniformData1.dat");
   ofstream outputU2("Risultati/UniformData2.dat");
   ofstream outputU3("Risultati/UniformData10.dat");
   ofstream outputU4("Risultati/UniformData100.dat");
   
   for(int i=0; i<4; i++) {
   	read>>bin;		//sono stati scelti per migliorare l'incasellamento...eseguendo per un po' e tentando
   	read>>range;		//idem
   	w=range/bin;	
   	if(uniform!=NULL) {
   		delete []uniform;
   	}
   	uniform=new double[bin];	//crea un vettore che conterrà il numero di punti in ciascun bin
   	
   	for(int b=0; b<bin; b++) {
   		uniform[b]=0.;
   	}
 	for(int j=0; j<M; j++) {
 		sum=0.;
 	   	for(int k=0; k<N[i]; k++) {
 	   	sum+=rnd.Rannyu();
   		}
   		sum=sum/N[i];	//valore della variabile "media"
   		for(int b=0; b<bin; b++) {			//trova dove è finito il punto
   			markUnif=(UnifAve-range/2.)+b*w;		//centra il punto medio dei bin
   			if(sum>=(markUnif-w/2.) and sum<(markUnif+w/2.)) {
   				uniform[b]++;
   			}
   		}
   	}	//da qui in poi è solo per stampare il risultato. 
   		if(i==0) { 
   			for(int b=0; b<bin; b++) {
   				markUnif=(UnifAve-range/2.)+b*w;
   				outputU1<<markUnif<<"   "<<uniform[b]<<endl;
   			}
   		}	
   		if(i==1) { 
   			for(int b=0; b<bin; b++) {
   				markUnif=(UnifAve-range/2.)+b*w;
   				outputU2<<markUnif<<"   "<<uniform[b]<<endl;
   			}
   		}
   		if(i==2) { 
   			for(int b=0; b<bin; b++) {
   				markUnif=(UnifAve-range/2.)+b*w;
   				outputU3<<markUnif<<"   "<<uniform[b]<<endl;
   			}
   		}	
   		if(i==3) { 
   			for(int b=0; b<bin; b++) {
   				markUnif=(UnifAve-range/2.)+b*w;
   				outputU4<<markUnif<<"   "<<uniform[b]<<endl;
   			}
   		}
	}
   read.close();		
   outputU1.close();
   outputU2.close();
   outputU3.close();
   outputU4.close();


return 0;
}

   
   
