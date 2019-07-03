/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include <fstream>
#include <ostream>
#include <cmath>
#include <iomanip>
#include "Monte_Carlo_ISING_1D.h"



using namespace std;

int main()
{ 
    Input(); //Inizialization	
    //EQUILIBRAZIONE.
    ofstream equilibration("Equilibration/InternalEnergy.dat");
    for(int i=1; i <=10; ++i) {	//blocchi per l'quilibrazione.
	Reset(i);
	for(int j=1; j<100; j++) {	//blocchi da jmax move.
		Move(metro);
		Measure();
		Accumulate();
		equilibration<<(i-1)*100+j<<"   "<<walker[iu]/double(nspin)<<endl;	
	}
	Averages(i);
    }
    equilibration.close();
    
    cout<<endl<<"Finished the equibration run"<<endl<<endl<<endl;
    //SIMULAZIONE.
    for(temp=0.5; temp<=2; temp+=0.05) {
    	for(int iblk=1; iblk <= nblk; ++iblk) {//Simulation
    		Reset(iblk);   //Reset block averages
    		for(int istep=1; istep <= nstep; istep++)
    		{
      			Move(metro);
      			Measure();
      			Accumulate(); //Update block averages
    		}
    		Averages(iblk);   //Print results for current block
    		DataOut(iblk);
    	}
    	ConfFinal(); //Write final configuration
   }
  return 0;
}



/***********************************************************************************************************************/
void Input(void) {

	ifstream ReadInput;

	cout << "Classic 1D Ising model             " << endl;
	cout << "Monte Carlo simulation             " << endl << endl;
	cout << "Nearest neighbour interaction      " << endl << endl;
	cout << "Boltzmann weight exp(- beta * H ), beta = 1/T " << endl << endl;
	cout << "The program uses k_B=1 and mu_B=1 units " << endl;

//Read seed for random numbers
	int p1, p2;
	ifstream Primes("Primes");
	Primes >> p1 >> p2 ;
	Primes.close();

	ifstream input("seed.in");
	input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
	rnd.SetRandom(seed,p1,p2);
	input.close();
  
//Read input informations
	ReadInput.open("input.dat");

	ReadInput >> temp;
	cout << "Temperature = " << temp << endl;
	
	ReadInput >> nspin;
	cout << "Number of spins = " << nspin << endl;

	ReadInput >> J;
	cout << "Exchange interaction = " << J << endl;

	ReadInput >> h;
	cout << "External field = " << h << endl << endl;
    
	ReadInput >> metro; // if=1 Metropolis else Gibbs

	ReadInput >> nblk;  //Nblocchi

	ReadInput >> nstep; //Nstep per ogni blocco

	if(metro==1) cout << "The program perform Metropolis moves" << endl;
	else cout << "The program perform Gibbs moves" << endl;
	cout << "Number of blocks = " << nblk << endl;
	cout << "Number of steps in one block = " << nstep << endl << endl;
	ReadInput.close();


//Prepare arrays for measurements
	iu = 0; //Energy
	ic = 1; //Heat capacity
	im = 2; //Magnetization
	ix = 3; //Magnetic susceptibility
 
	n_props = 4; //Number of observables

//initial configuration
	ifstream Config("risultati/config.final");
 	if(Config.fail()) {
 		cout<<"Inizializzo gli spin a una configurazione causale"<<endl<<endl;
		for (int i=0; i<nspin; ++i){
			if(rnd.Rannyu() >= 0.5) s[i] = 1;		//mette a casissimo gli spin.
    			else s[i] = -1;
  		}
	} else {
  		cout<<"Leggo l'ultima configurazione"<<endl<<endl;
  		for (int i=0; i<nspin; ++i) {
  			Config>>s[i];
  		}
  	}
  		
//Evaluate energy etc. of the initial configuration
  	Measure();

//Print initial values for the potential energy and virial
  	cout << "Initial energy = " << walker[iu]/(double)nspin << endl;
}


//**********************************************************************+
void Move(int metro) {
	int o;
	double p, energy_old, energy_new, sm;
	double energy_up, energy_down;

	for(int i=0; i<nspin; ++i) {
		attempted++;
  //Select randomly a particle (for C++ syntax, 0 <= o <= nspin-1)
		o = (int)(rnd.Rannyu()*nspin);
		sm=s[o];
		if(metro==1) //Metropolis
    		{
    			energy_old=Boltzmann(sm,o);		//energia vecchia con il vecchio valore dello spin.
    			energy_new=Boltzmann(sm*-1.,o);		//energia nuova con il valore dello spin flipato.
		//probabilità di accettare.
			p=min(1.,exp(-(energy_new-energy_old)/temp));		//probabilità accettazione.
			if(rnd.Rannyu()<p) {
				s[o]=sm*-1.;
				accepted++;
			}
    		}
   		else //Gibbs sampling
    		{
			energy_up=Boltzmann(+1,o);
			energy_down=Boltzmann(-1,o);
			p=exp(-energy_up/temp)/(exp(-energy_up/temp)+exp(-energy_down/temp)); //probabilità con cui voglio lasciarlo up
			if(rnd.Rannyu()<p) {
				s[o]=+1;
				accepted++;
			} else {
				s[o]=-1;
			}
    		}
  	}
}

//******************************************************
double Boltzmann(int sm, int ip) {
	double ene = -J * sm * ( s[Pbc(ip-1)] + s[Pbc(ip+1)] ) - h * sm;			//Pbc mi costruisce la pbc del mio array di spin
	return ene;
}
//******************************************************
void Measure() {
  //int bin;
	double u = 0.0, m = 0.0;

//cycle over spins
	for (int i=0; i<nspin; ++i) {
     		u += -J * s[i] * s[Pbc(i+1)] - 0.5 * h * (s[i] + s[Pbc(i+1)]);			//calcola l'energia interna   
		m += s[i];									//calcola la magnetizzazione
	}
	walker[iu] = u;									//salva l'energia interna nel vettore walker
	walker[im] = m;									//salva la magnetizzazione nel vettore walker
	walker[ic] = u*u;								//per il calolore specifico
	walker[ix] = m*m;								//per la suscettività
}

//******************************************************
void Reset(int iblk) {			//Reset block averages
   
	if(iblk == 1) {			//se è il primo step resetta tutto.
		for(int i=0; i<n_props; ++i) {
           		glob_av[i] = 0;
           		glob_av2[i] = 0;
       		}
   	}

   	for(int i=0; i<n_props; ++i) {
     		blk_av[i] = 0;
   	}
   	blk_norm = 0;
   	attempted = 0;
   	accepted = 0;
}

//******************************************************
void Accumulate(void) {		 //Update block averages

	for(int i=0; i<n_props; ++i) {
		blk_av[i] = blk_av[i] + walker[i];		//Somma l'ultimo risultato
	}
	blk_norm = blk_norm + 1.0;			//Aggiorna la lunghezza del blocco
}

//******************************************************
void Averages(int iblk) { //Print results for current block
    
	cout << "Block number " << iblk << endl;
	cout << "Acceptance rate " << accepted/attempted << endl << endl;
    		
	stima_u = blk_av[iu]/blk_norm/(double)nspin; //Energy per spin
	glob_av[iu]  += stima_u;
	glob_av2[iu] += stima_u*stima_u;
	err_u=Error(glob_av[iu],glob_av2[iu],iblk);
   
   	stima_c=(-stima_u*stima_u*(double)nspin*nspin+blk_av[ic]/blk_norm)/(double)nspin/temp/temp;
	glob_av[ic]+=stima_c;
	glob_av2[ic]+=stima_c*stima_c;
	err_c=Error(glob_av[ic],glob_av2[ic],iblk);

	stima_m=blk_av[im]/blk_norm/(double)nspin;	//magnetization
	glob_av[im] +=stima_m;
	glob_av2[im] +=stima_m*stima_m;
	err_m=Error(glob_av[im],glob_av2[im],iblk);
	
	stima_x=(/*-stima_m*stima_m*(double)nspin*(double)nspin+*/blk_av[ix]/blk_norm)/(double)nspin/temp;
   	glob_av[ix]+=stima_x;
   	glob_av2[ix]+=stima_x*stima_x;
   	err_x=Error(glob_av[ic],glob_av2[ix],iblk);

	cout <<"Energia E="<<(glob_av[iu]/(double)iblk) << endl;
	cout <<"Calore specifico C="<<glob_av[ic]/(double)iblk<<endl;
	cout <<"Magnetizzazione M="<<glob_av[im]/(double)iblk<<endl;
	cout <<"Suscettività chi="<<glob_av[ix]/(double)iblk<<endl<<endl;
}

void DataOut(int iblk) {
	ofstream Ene, Heat, Mag, Chi;
	ofstream FinEne, FinHeat, FinMag, FinChi;
	const int wd=12;
	
	string str=to_string(temp);
    	str=str.substr(0,4);
    	
	if(h==0) {
		Ene.open("risultati/output.ene." + str,ios::app);
		Ene << setw(wd) << iblk <<  setw(wd) << stima_u << setw(wd) << glob_av[iu]/(double)iblk << setw(wd) << err_u << endl;
		Ene.close();
		Heat.open("risultati/output.heat." + str, ios::app);
		Heat<<setw(wd)<<iblk<<setw(wd)<<stima_c<<setw(wd)<<glob_av[ic]/(double)iblk<<setw(wd)<<err_c<<endl;
		Heat.close();
		Chi.open("risultati/output.chi." + str, ios::app);
   		Chi<<setw(wd)<<iblk<<setw(wd)<<stima_x<<setw(wd)<<glob_av[ix]/(double)iblk<<setw(wd)<<err_x<<endl;
   		Chi.close();
   	
	}
	if(h!=0) {
		Mag.open("risultati/output.mag." + str, ios::app);
		Mag<<setw(wd)<<iblk<<setw(wd)<<stima_m<<setw(wd)<<glob_av[im]/(double)iblk<<setw(wd)<<err_m<<endl;
		Mag.close();
	}
	
   	if(iblk==nblk) {
   		if(h==0) {
   			FinEne.open("risultati/output.ene", ios::app);
   			FinEne<<temp<<"   "<<glob_av[iu]/(double)iblk<<"   "<<err_u<<endl;
   			FinHeat.open("risultati/output.heat", ios::app);
   			FinHeat<<temp<<"   "<<glob_av[ic]/(double)iblk<<"   "<<err_c<<endl;
   			FinChi.open("risultati/output.chi", ios::app);
	  		FinChi<<temp<<"   "<<glob_av[ix]/(double)iblk<<"   "<<err_x<<endl;
   		}
   		if(h!=0) {
   			FinMag.open("risultati/output.mag", ios::app);
   			FinMag<<temp<<"   "<<glob_av[im]/(double)iblk<<"   "<<err_m<<endl;
   		}
  	}
}
//********************************************************
void ConfFinal(void) {
	ofstream WriteConf;
	cout << "Print final configuration to file config.final " << endl << endl;
	WriteConf.open("risultati/config.final");
	for (int i=0; i<nspin; ++i) {
		WriteConf << s[i] << endl;
	}
	
	WriteConf.close();
  	rnd.SaveSeed();
}
//*********************************************************
int Pbc(int i) { //Algorithm for periodic boundary conditions
	if(i >= nspin) i = i - nspin;
	else if(i < 0) i = i + nspin;
	return i;
}
//********************************************************
double Error(double sum, double sum2, int iblk) {
	return sqrt((sum2/(double)iblk - pow(sum/(double)iblk,2))/(double)iblk);
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
