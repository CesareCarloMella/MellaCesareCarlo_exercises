//DICHIARAZIONE DELLA CLASSE WALK.


#ifndef __Walk__
#define __Walk__

class Walk {

private:
	double q;		//coordinate del moto
	double l;		//lunghezza passo
	Random* rnd;
	double a1;		//parametro del modello
public:
	 // constructors
	Walk();
	Walk(double, Random*, double);			//passare lunghezza del passo e numero di passi e parametro del modello
  	// destructor
  	~Walk();
 	 // methods
  	void RandomLatticeStep();	//passo random in una delle direzioni cartesiane di lunghezza l
  	void RandomDirStep();		//passo random in direzione generica di lunghezza l
  	void SetLenght(double);		//imposta lunghezza
	void SetGenerator(Random * r);	//imposta generatore random
	void SetA1(double);		//imposta il parametro del modello

	double Distance();		//restituisce la distanza dall'origine
	double SquareDist();		//restituisce il modulo quadro della distanza
	void AzzeraCammino();
 	double GetPosition();
};


#endif
