//HEADER FILE per Traveling SalesMan
#ifndef  TravelingSalesman
#define TravelingSalesman

#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include "random.h"

using namespace std;

//INCAPSULATORE PER COORDINATE CITTA'
struct City {
	double x;
	double y;
};


//CLASSE CROMOSOMA
class Path {
private:
int N;			//numero di città
vector <int> percorso;	//vettore per il percorso
double L;		//lunghezza del percorso
		
public:
Path();
Path(int quante_citta, vector <int> path, vector <City> quali_citta);			//costruisce il percorso, passare il numero di città e il percorso da assegnare
~Path();
void MisLength(vector <City> quali_citta);				//Misura la lunghezza, passare le coordinate
double GetLength(vector <City> quali_citta);				//Restituisce la lunghezza del percorso
void PrintSequence();						//Stampa la sequenza di città
void PrintPosition(int wihchCity, vector <City> quale_citta);	//stampa la posizione della città whichCity 
void Controllo();						//Controlla che ci sia tutto e che non ci siano ripetizioni
void SetSequence(vector <int> path);				//Imposta una sequenza
int GetN();							//Restituisce il numero di città
int GetElement(int i);						//restituisce l'indice di città al posto i-esimo della sequenza
void SetElement(int i, int sost);				//imposta elemento

};


//********************************funzioni
//Ordinamento
vector <Path> SelectionSort( vector <Path> popol, vector <City> citta);

//Pair Permutation
Path PairPermutation(Random * rnd, Path percorso);

//Seleziona i migliori
int Selection( int NumCity, Random * rnd);

//Periodic Boundary condition
int PBC(int N, int n);		//Passare lunghezza sequenza e indice che si vuole correggeres


//Mutazione: shift a blocchi
Path BlockShift(int n, int m, int start, Path sequence);	//Passare indice di shift, numero città contigue, indice di partenza, sequenza

//Mutazione: scambio e blocchi
Path BlockSwap(int n1, int n2, int m, Path sequence);		//Passare primo elemento del primo blocco, secondo ... secondo blocco, lunghezza blocco




//Metropolis
Path T_Random(Random* rnd,Path percorsoInput, int WhichMutation);		//passare generatore random e quale mutazione effettuare 
//Accept Reject
int A_AccRej(Random* rnd, double prob, int* th, int* acc, int quale);		//passare generatore random, probabilita di accettare, posizione e incremento proposto, lanci, numero di punti accettati.





#endif
