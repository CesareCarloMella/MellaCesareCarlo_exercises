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
        double delta;           	//lunghezza passo.
        position s, incr, f;    	//posizione attuale s, incremento proposto, posizione finale
        int lanci, accettati;		//per controllo asso di accettazione
        lanci=0;
        accettati=0;
        double prob1, prob2, prob;
        int segnale=1;			//per decidere se accettare il passo.
        int M, N;			//parametri monte carlo
        double *r, *prog, *prog2;	//statistica a blocchi.
        int L;
	//lettura dei parametri
        ifstream read("Parameters.dat");
        read>>s.x;		//posizione iniziale
        read>>s.y;		//	""	""
        read>>s.z;		//	""	""
        read>>delta;            //lunghezza inizialmente proposta
        read>>M;                //numero di lanci
        read>>N;                //numero di blocchi
        L=M/N;			//lunghezza blocco
        //Ho le dimensioni dei vettori che mi servono, procedo
        r=new double[M];
        prog=new double[N];
        prog2=new double[N];
        read.close();
        //posizione iniziale impostata, verifico (e modifico) passo tramite controllo del tasso di accettazione.
        while(segnale==1) {
                accettati=0;
                lanci=0;
                for(int i=0; i<100; i++) {
                        incr=T_Random(rnd, delta);     	//genera una proposta
                        f.x=s.x+incr.x;			//posizione finale proposta
                        f.y=s.y+incr.y;
                        f.z=s.z+incr.z;
                        prob1=prob100(s);		//valuta le probabilità
                        prob2=prob100(f);		
                        prob=min(1.,prob2/prob1);	//probabilità che entrerà nella funzione di accettazione.
                        A_AccRej(rnd, prob, &s,&incr, &lanci, &accettati);	//Accetto?
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
                read>>s.x;
                read>>s.y;
                read>>s.z;
                read.close();
       }
//La delta e' ok. Procedo con la simulazione
        ofstream printR("risultati100/raggi100.out");
      	ofstream printP("risultati100/posizioni100.out");
        for(int i=0; i<N; i++) {
                prog[i]=0.;
                prog2[i]=0.;
        }
        for(int i=0; i<N; i++) {
                for(int j=L*i; j<L*(i+1); j++) {
                        incr=T_Random(rnd,delta);
                        f.x=s.x+incr.x;
                        f.y=s.y+incr.y;
                        f.z=s.z+incr.z;
                        prob1=prob100(s);
                        prob2=prob100(f);
                        prob=min(1.,prob2/prob1);
                        A_AccRej(rnd, prob, &s,&incr, &lanci, &accettati);
                        r[j]=getRadius(s);
                        printR<<(j+1)<<"   "<<r[j]<<endl;
                        if(j<=10000) {
                        	printP<<(j+1)<<"   "<<s.x<<"   "<<s.y<<"   "<<s.z<<endl;
                        }
                        prog[i]+=r[j];
                }
                prog[i]/=L;              	//media su un blocco
                prog2[i]=prog[i]*prog[i];       //quadrato della media su un blocco 
       }
       printR.close();
       printP.close();
       printR.open("risultati100/progressivi100.out");
       MediaBlocchi(N,prog,prog2,&printR);
       printR.close();
               

return 0;
}


