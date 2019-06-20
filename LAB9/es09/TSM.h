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
Path(int quante_citta, vector <int> path, vector <City> quali_citta);			//costruisce il percorso, passare il numero di città e il percorso da assegnare
~Path();
void MisLength(vector <City> quali_citta);				//Misura la lunghezza, passare le coordinate
double GetLength(vector <City> quali_citta);						//Restituisce la lunghezza del percorso
void PrintSequence();						//Stampa la sequenza di città
void PrintPosition(int wihchCity, vector <City> quale_citta);		//stampa la posizione della città whichCity 
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

//Mutazione: shift di n
Path Shift(int n, Path sequence);

//Mutazione: shift a blocchi
Path BlockShift(int n, int m, int start, Path sequence);	//Passare indice di shift, numero città contigue, indice di partenza, sequenza

//Mutazione: scambio e blocchi
Path BlockSwap(int n1, int n2, int m, Path sequence);		//Passare primo elemento del primo blocco, secondo ... secondo blocco, lunghezza blocco

//Mutazione: inversione
Path Inversion(int n, int m, Path sequence);			//passare primo elemento del blocco da invertire, lunghezza e sequenza


//Crossover

vector <Path> Crossover(Random * rnd,vector <Path> NewGeneration);
//PASSARE generatore random, vettore new generation da aggiornare, genitori scelti.
Path GeneraFiglio(int N,int cut, Path parent1, Path parent2);
//la precendete taglia parent1 all'indice cut. Da cut+1 riordina secondo l'ordine di parent2.

#endif
