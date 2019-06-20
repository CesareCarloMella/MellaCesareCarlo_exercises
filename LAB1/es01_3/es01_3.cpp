#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "random.h"
#include "ago.h"
#include "MediaBlocchi.h"

using namespace std;


int main() {
	
	int M, N, m;	//numero di lanci, numero di successi, numero di blocchi, numero di dati per ogni stima di vett.
	double L,d,w;		//lunghezza ago, distanza righe, lunghezza vettano, angolo e centro, proiezione.
	ifstream input("Parameters.dat");
	input>>L;		//primo dato file: lunghezza ago
	input>>d;		//secondo dato file: distanza righe
	input>>w;		//terzo dato file: lunghezza piano.
	input>>M;		//quarto dato file: numero di lanci ----> scegliere un multiplo di N (sotto)
	input>>N;		//quinto dato file: numero di blocchi
	input>>m;		//sesto dato file: numero di punti da usare per stima pi greco.
	input.close();
	int Lb=M/N;
	Ago needle(L);				//crea oggetto Ago di lunghezza L
	double *pi=new double[N];		//valore medio di pi in ogni blocco.
	double *pi2=new double[N];		//valore medio di pi al quadrato.
	double sum;
	ofstream output("Risultati/PiData.dat");
	
	for(int i=0; i<N; i++) {
		sum=0.;
		for(int j=0; j<Lb; j++) {
			sum+=needle.GetPi(m,d,w);
		}
		pi[i]=sum/(float)Lb;
		pi2[i]=pi[i]*pi[i];
	}
	MediaBlocchi(N, pi, pi2, &output);
	output.close();

return 0;
}
