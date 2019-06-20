
#ifndef __Function__
#define __Function__

#include "random.h"
#include <cmath>
#include <iostream>

//*************************************************PER METROPOLIS************************************************************
struct position
{
  double x, y, z;
};

//Genera proposta
position T_Random(Random*, double);		//passare generatore random, struttura posizione e lunghezza del passo.

//Accept Reject
void A_AccRej(Random*,double, position*, position*, int*, int*);		//passare generatore random, probabilita di accettare, posizione e incremento proposto, lanci, numero di punti accettati.
//*************************************************PER METROPOLIS************************************************************

//Densita' probabilita' 1s
double prob100(position);		//calcola la densità di probabilità nella posizione passata
//Densita' probabilita' 2p
double prob210(position);		// ""		""		""		""
//Ottieni raggio
double getRadius(position);		//restituisce il raggio.

#endif
