#include "MediaBlocchi.h"
using namespace std;

void MediaBlocchi(int N, double* prog, double* prog2, ofstream* output ) {
	double stdev=0.;	//standard deviation
	double sum=0.;		//sono variabili di appoggio
	double sum2=0.;
	for(int i=0; i<N; i++) {		//scorrendo sui blocchi
		if(i==0) {			//se è il primo, non posso calcolare incertezza statistica.
			sum+=prog[i];		//sommo...
			sum2+=prog2[i];		//...i quadrati
			*output<<(i+1)<<"   "<<sum<<"   "<<stdev<<endl;		//stampo su file.
		} else {	//dal secondo blocco
			sum*=(i);	//torno alla somma, dalla media,
			sum+=prog[i];	//aggiungo un termine,
			sum/=(i+1);	//medio.
			sum2*=(i);	//torno alla somma dei quadrati, dalla media,
			sum2+=prog2[i];	//aggiungo un termine,
			sum2/=(i+1);	//medio.
			stdev=sqrt((sum2-sum*sum)/(i));		//deviazione standard.
			*output<<(i+1)<<"   "<<sum<<"   "<<stdev<<endl;		//stampo su file.
		}
	}
}


		
		
