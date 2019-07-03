#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "random.h"
#include "Function.h"
#include "MediaBlocchi.h"


using namespace std;


int main() {
//generatore
	Random* rnd=new Random();		
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
	            rnd->SetRandom(seed,p1,p2);
	         }
	      }
	      input.close();
	   } else cerr << "PROBLEM: Unable to open seed.in" << endl;
	   
	   
//**********************************************************************
        double delta, mu, sigma;           	//lunghezza passo.
        double x, x_start, incr, f;    	//posizione attuale s, incremento proposto, posizione finale
        int lanci, accettati;		//per controllo asso di accettazione
        lanci=0;
        accettati=0;
        double prob1, prob2, prob;
        int segnale=1;			//per decidere se accettare il passo.
        int M, N;			//parametri monte carlo
        double *prog, *prog2;	//statistica a blocchi.
        int L;
        double x_min, x_max, bin_size, *occupazione, *posizione;
        int bin_num;
        
	//lettura dei parametri
        ifstream read("Parameters.dat");
        read>>x_start;		//posizione iniziale
        read>>delta;            //lunghezza inizialmente proposta
        read>>M;                //numero di lanci
        read>>N;                //numero di blocchi
        L=M/N;			//lunghezza blocco
        read>>x_min;		//limite sinistro per il riempimento dell'istogramma
        read>>x_max;		//limite destro per il riempimento dell'istogramma
        read>>bin_num;		//numero di bin
        posizione=new double[bin_num];		//posizioni per l'istogramma
        occupazione=new double[bin_num];	//occupazione dei bin
        for(int i=0; i<bin_num; i++) {
        	posizione[i]=0.;
        	occupazione[i]=0.;
        }
        bin_size=(x_max-x_min)/double(bin_num);	//calcolo la dimensione dei bin
        
        //Ho le dimensioni dei vettori che mi servono, procedo
      
        prog=new double[N];
        prog2=new double[N];
        read.close();
        //posizione iniziale impostata, verifico (e modifico) passo tramite controllo del tasso di accettazione.
        mu=0.5;
        sigma=1;
        while(segnale==1) {
                accettati=0;
                lanci=0;
                for(int i=0; i<100; i++) {
                        incr=T_Random(rnd, delta);     	//genera una proposta
                        f=x+incr;			//posizione finale proposta
                        prob1=Double_Gauss(x, mu, sigma);		//valuta le probabilità
                        prob2=Double_Gauss(f, mu, sigma);		
                        prob=min(1.,prob2/prob1);	//probabilità che entrerà nella funzione di accettazione.
                        A_AccRej(rnd, prob, &x,&incr, &lanci, &accettati);	//Accetto?
                }
                double accettazione=accettati*100./lanci;
                cout<<"Con questo passo ho accettato il "<<accettazione<<"% delle proposte. Delta="<<delta<<endl;
                cout<<"Se vuoi cambiare il delta premi 1, se sei soddisfatto premi 2."<<endl;
                cin>>segnale;
                if(segnale==1) {
                        cout<<"Hai scelto di modificare delta. Inserisci la nuova lunghezza in unita' di raggio di Bohr."<<endl;
                        cin>>delta;
                }
                read.open("Parameters.dat");		//In ogni caso comincio da capo.
                read>>x;
                read.close();
       }
       
//Tasso di accettazione regolato. Ottimizzazione dei parametri:  
	double E;
	double E_min=0.;
	double sigma_min=0.;
	double mu_min=0.;
	ofstream OttimData("Risultati/Ottimizzazione.dat");
	for(sigma=0.55; sigma<=0.7; sigma+=0.005) {	//griglia in sigma
    		for(mu=0.7; mu<=0.9; mu+=0.005) {	//griglia in mu
    			//stima dell'energia
    			E=0.;
    			x=x_start;			//riparto sempre dall'inizio
    			for(int i=0; i<M; i++) {	
                        	incr=T_Random(rnd,delta);	
                        	f=x+incr;
                        	prob1=Double_Gauss(x,mu,sigma);
                        	prob2=Double_Gauss(f,mu,sigma);
                       		prob=min(1.,prob2/prob1);
                        	A_AccRej(rnd, prob, &x,&incr, &lanci, &accettati);
                      		E+=Integrand(x,mu,sigma);	//aggiorno energia
                      	}
                      	E/=(double)M;	//quanto vale l'energia?
                      	OttimData<<mu<<"   "<<sigma<<"   "<<E<<endl;
                     	//fine stima, energia stampata
                     	//algoritmo per minimizzare
                      	if(sigma==0.5 and mu==0.1) {	//se è il primo tentativo imposto i valori...
                      		E_min=E;
                      		sigma_min=sigma;
                      		mu_min=mu;
                      	} else { 			//... altrimenti confronto
                      		if(E<=E_min) { 
                      			cout<<"Trovato un valore di energia più basso, New_E_Min = "<<E<<endl;
                      			cout<<"Nuovi parametri, sigma = "<<sigma<<" e mu = "<<mu<<endl<<endl;
                      			E_min=E;
                      			sigma_min=sigma;
                      			mu_min=mu;
                      		}
                      	}
                }
       	}
        OttimData.close();
//fine dell'algoritmo per il minimo      
        //Integrazione per la stima dell'energia del ground state
        x=x_start;
        ofstream output1("Risultati/progressivi.out");
	ofstream output3("Risultati/parametriFinali.out");
	
	output3<<x_start<<endl;
	output3<<delta<<endl;
	output3<<M<<endl;
	output3<<N<<endl;
	output3<<x_min<<endl;
	output3<<x_max<<endl;
	output3<<bin_num<<endl;
        output3<<mu_min<<endl;
        output3<<sigma_min<<endl;
        
        output3<<endl<<endl;
        output3<<"	posizione di partenza (x_start)"<<endl<<"	delta"<<endl<<"	numero punti totale"<<endl<<"	numero blocchi"<<endl;
        output3<<"	estremo inferiore per istrogramma"<<endl<<"	estremo superiore"<<endl<<"	numero dei bin"<<endl<<"	parametro mu ottimizzato"<<endl<<"	parametro sigma ottimizzato"<<endl;	
        output3.close();
        	
	

         for(int i=0; i<N; i++) {
                prog[i]=0.;
                prog2[i]=0.;
        }
        
        int which_bin;
        for(int i=0; i<N; i++) {
                for(int j=L*i; j<L*(i+1); j++) {
                        incr=T_Random(rnd,delta);		//genera proposta
                        f=x+incr;				//	""	""
                        prob1=Double_Gauss(x, mu_min, sigma_min);	//calcola le probabilità
                        prob2=Double_Gauss(f, mu_min, sigma_min);	//	""	""
                        prob=min(1.,prob2/prob1);			//probabilità di accettazione
                        A_AccRej(rnd, prob, &x,&incr, &lanci, &accettati);		//accetto??
                        if(x>=x_min and x<=x_max) {
	                        which_bin=int((x-x_min)/bin_size);	//aggiorno l'istogramma
        	                occupazione[which_bin]+=1.;
        	        }
                        prog[i]+=Integrand(x, mu_min, sigma_min);		//valuta energia
                }
                prog[i]/=L;	          	//media su un blocco
                prog2[i]=prog[i]*prog[i];       //quadrato della media su un blocco 
       }

       MediaBlocchi(N,prog,prog2,&output1);
       output1.close();
       
       ofstream output2("Risultati/istogramma.out");         	
       for(int i=0; i<bin_num; i++) {
       		posizione[i]=(double(i)*bin_size)+x_min;
       		output2<<posizione[i]<<"   "<<occupazione[i]<<endl;
       }
       
       output2.close();
       
                      
return 0;
}
