//HEADER FILE DELLA CLASSE MolDyn	PER LA SIMULAZIONE DI DINAMICA MOLECOLARE

#ifndef _MOLDYN_
#define _MOLDYN_

class MolDyn {

private:
double stima_pot, stima_kin, stima_etot, stima_temp, stima_P;	//osservabili
//configuration
int n_part;
double *x, *y, *z, *xold, *yold, *zold;		//posizioni.
double *vx, *vy, *vz;				//velocità.
//stato termodinamico
double energy,temp,vol,rho,box,rcut;		
// simulation
int nstep, seed;
double delta;
public:
//methods
MolDyn(double, int, double, double, double, int);	//temp, npart, densità, rcut, delta, nstep.
~MolDyn();
void Input(void);	//In particolare, imposta le posizioni.
void Move(void);
double Force(int, int);
void Measure(void);
double GetU(void);
double GetK(void);
double GetE(void);
double GetT(void);
double GetP(void);
void ConfFinal(void);
void ConfXYZ(int);
double Pbc(double);
};
#endif 
