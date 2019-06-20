
#ifndef _prezzo_
#define _prezzo_

class Prezzo {
private:
	double r, sigma, t, K;
	Random *rnd;
	double prezzo;
public:
	Prezzo();
	Prezzo(double rate, double sig, double time, double Sprezzo, double Kprice, Random* rand);
  	// destructor
  	~Prezzo();
	// methods
 	void SetParameters(double rate, double sig , double time, double Sprezzo, double Kprice);
	void SetRandom(Random * rand);
  	double Prezza(double tfinal);
};

#endif
