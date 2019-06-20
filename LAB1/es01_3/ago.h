//*************************HEADER FILE DELLA CLASSE AGO***************************


#ifndef __Ago__
#define __Ago__

#include "random.h"


class Ago {

private:
	double L, Theta, Mid;
	Random* rnd;
	int seed[4];
   	int p1, p2;
	
public:

  // constructors
  Ago();
  Ago(double);	//costruttore con lunghezza.
  // destructor
  ~Ago();
  // methods
  void SetLenght(double);		//fissa la lunghezza
  void Throw(double, double);		//lancia l'ago, passare la lunghezza del piano e distanza tra le righe
  double GetMid(void);
  double GetTheta(void);
  double GetPi(double, double, double);	//esegue una stima di pi greco.
};

#endif
