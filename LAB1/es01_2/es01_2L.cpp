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
//*************************+   
   int* N=new int[4];			//numero di punti per costruire la variabile "media"
   N[0]=1;
   N[1]=2;
   N[2]=10;
   N[3]=100;
   int M=10000;				//numero di campionamenti.
   int bin;				//numero di intervalli.
   double range=1.;			//range del grafico.
   double w;				//larghezza dei singoli intervalli.
   double *lor=NULL;			//dati della distribuzione.
   double sum=0.;
   double mean, gamma;
   mean=0.;
   gamma=1.;
   ifstream read("Parametri/DataInL.dat");	//file con numero di bin e range.
   double markLor;			//segnaposto per poter mettere i dati
   double LorAve=mean;			//Valore medio della distribuzione.
   
   ofstream outputL1("Risultati/LorenzianData1.dat");
   ofstream outputL2("Risultati/LorenzianData2.dat");
   ofstream outputL3("Risultati/LorenzianData10.dat");
   ofstream outputL4("Risultati/LorenzianData100.dat");


   for(int i=0; i<4; i++) {
   	read>>bin;
   	read>>range;
   	w=range/bin;	
   	if(lor!=NULL) {
   		delete []lor;
   	}
   	lor=new double[bin];
   	for(int b=0; b<bin; b++) {
   		lor[b]=0.;
   	}
 	for(int j=0; j<M; j++) {
 		sum=0.;  
 	   	for(int k=0; k<N[i]; k++) {
 	   	sum+=rnd.Lorentz(mean, gamma);
   		}
   		sum=sum/N[i];		//valore della variabile media.
   		
   		for(int b=0; b<bin; b++) {
   			markLor=(LorAve-range/2.)+b*w;   			
   			if(sum>=(markLor-w/2.) and sum<(markLor+w/2.)) {
   				lor[b]++;
   			}
   		}
   	}
   		if(i==0) { 
   			for(int b=0; b<bin; b++) {
   				markLor=(LorAve-range/2.)+b*w;
   				outputL1<<markLor<<"   "<<lor[b]<<endl;
   			}
   		}	
   		if(i==1) { 
   			for(int b=0; b<bin; b++) {
   				markLor=(LorAve-range/2.)+b*w;
   				outputL2<<markLor<<"   "<<lor[b]<<endl;
   			}
   		}
   		if(i==2) { 
   			for(int b=0; b<bin; b++) {
   				markLor=(LorAve-range/2.)+b*w;
   				outputL3<<markLor<<"   "<<lor[b]<<endl;
   			}
   		}	
   		if(i==3) { 
   			for(int b=0; b<bin; b++) {
   				markLor=(LorAve-range/2.)+b*w,
   				outputL4<<markLor<<"   "<<lor[b]<<endl;
   			}
   		}
	}

   outputL1.close();
   outputL2.close();
   outputL3.close();
   outputL4.close();

return 0;
}

   
   
