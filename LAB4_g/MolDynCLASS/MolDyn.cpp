//IMPLEMENTAZIONE DELLA CLASSE MolDyn

#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <fstream>
#include "MolDyn.h"

using namespace std;
//costruttore **************
 MolDyn :: MolDyn(double T, int number, double dens, double taglio, double dtime, int npassi, int num_bin) {
	temp=T;				//temperatura
	n_part=number;			//numero di particelle
	x=new double[n_part];		//preparo variabili
	y=new double[n_part];
	z=new double[n_part];
	xold=new double[n_part];	//...vecchie...
	yold=new double[n_part];
	zold=new double[n_part];
	vx=new double[n_part];		//...veloità...
	vy=new double[n_part];
	vz=new double[n_part];	
	rho=dens;			//densità sistema
	rcut=taglio;			//cut di interazione
	delta=dtime;			//incremento temporale
	nstep=npassi;			//numero di step
	//osservabili
	stima_pot=0.;
	stima_kin=0.;
	stima_etot=0.;
	stima_temp=0.;
	//per random
	seed = 1;    //Set seed for random numbers
 	srand(seed); //Initialize random number generator
 	//dimensioni del sistema. Dipendono dai dati forniti
 	vol = (double)n_part/rho;
 	box = pow(vol,1.0/3.0);
 	//Per il calcolo della g(r)
	nbins = num_bin;
	bin_size = (box/2.0)/(double)nbins;
}

//*******************distruttore
MolDyn :: ~MolDyn() {
	
}

//METODO Input *****************************************************************************************************************
void MolDyn :: Input(void) {
	//Messaggi descrittivi
	cout << "Classic Lennard-Jones fluid        " << endl;
 	cout << "Molecular dynamics simulation in NVE ensemble  " << endl << endl;
 	cout << "Interatomic potential v(r) = 4 * [(1/r)^12 - (1/r)^6]" << endl << endl;
  	cout << "The program uses Lennard-Jones units " << endl;
 	cout << "Number of particles = " << n_part << endl;
	cout << "Density of particles = " << rho << endl;
  	cout << "Volume of the simulation box = " << vol << endl;
  	cout << "Edge of the simulation box = " << box << endl;

	cout << "The program integrates Newton equations with the Verlet method " << endl;
	cout << "Time step = " << delta << endl;
	cout << "Number of steps = " << nstep << endl << endl;
//IMPLEMENTAZIONE DELLO STATO POSIZIONI INIZIALI.
	ifstream ReadConf;
	ReadConf.open("config/config.final");	//legge le ultime posizioni...
	if(ReadConf.fail()==true) {		//...ma se non ci sono...
		cout << "Read initial configuration from file config.0 " << endl << endl;
		ReadConf.open("config/config.0");	//il file .fcc contiene le posizioni di un fcc
 		for (int i=0; i<n_part; ++i){
   			ReadConf >> x[i] >> y[i] >> z[i];
    			x[i] = x[i] * box;
    			y[i] = y[i] * box;
    			z[i] = z[i] * box;
  		}
  		ReadConf.close();
  		cout << "Prepare random velocities with center of mass velocity equal to zero " << endl << endl;
   		double sumv[3] = {0.0, 0.0, 0.0};
  		for (int i=0; i<n_part; ++i){
   	  		vx[i] = rand() - 0.5;
     			vy[i] = rand() - 0.5;
     			vz[i] = rand() - 0.5;	//si potrebbe migliorare con Maxwell Boltzmann
     			sumv[0] += vx[i];
     			sumv[1] += vy[i];
     			sumv[2] += vz[i];
  		}

 		for (int idim=0; idim<3; ++idim) sumv[idim] /= (double)n_part;
   		double sumv2 = 0.0, fs;
   		for (int i=0; i<n_part; ++i){
    			vx[i] = vx[i] - sumv[0];
    			vy[i] = vy[i] - sumv[1];
    			vz[i] = vz[i] - sumv[2];

     			sumv2 += vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i];
   		}
   		sumv2 /= (double)n_part;
   		fs = sqrt(3 * temp / sumv2);   // fs = velocity scale factor 
   		for (int i=0; i<n_part; ++i){
     			vx[i] *= fs;
    			vy[i] *= fs;
     			vz[i] *= fs;
		//ora le velocità sono compatibili con la temperatura.
     			xold[i] =Pbc( x[i] - vx[i] * delta);
     			yold[i] =Pbc( y[i] - vy[i] * delta);
     			zold[i] =Pbc( z[i] - vz[i] * delta);
  		}		
  	} else {		//... se invece non è la prima volta che viene eseguito il programma...
  		cout<<"Non è la prima simulazione! Continuo dall'ultimo risultato."<<endl<<endl;
  		for (int i=0; i<n_part; ++i){
   			ReadConf >> x[i] >> y[i] >> z[i];
    			x[i] = x[i] * box;
    			y[i] = y[i] * box;
    			z[i] = z[i] * box;
  		}
  		ReadConf.close();
  		ReadConf.open("config/config.prefinal");		//legge le penultime posizioni
  		for(int i=0; i<n_part; i++) {
  			ReadConf>> xold[i] >> yold[i] >>zold[i];
  			xold[i]= xold[i] * box;
  			yold[i]= yold[i] * box;
  			zold[i]= zold[i] * box;
  		}
  		ReadConf.close();
  		//Stima delle velocità per vedere a che punto è la temperatura. la precisione è inferiore alla stima solita.
  		double sumv2 = 0.0, fs;
  		for(int i=0; i<n_part; i++) {
  			vx[i]=(x[i]-xold[i])/delta;
  			vy[i]=(y[i]-yold[i])/delta;
  			vz[i]=(z[i]-zold[i])/delta;
  			sumv2 += vx[i]*vx[i]+vy[i]*vy[i]+vz[i]*vz[i];
  		}
  		sumv2 /=(double)n_part;
  		double CheckTemp=sumv2/3;
  		if(abs((CheckTemp-temp)/temp*100.)<=5) {
  			cout<<"La temperatura è giusta entro il 5%, non correggo."<<endl;
  		} else {
  			fs = sqrt(3 * temp / sumv2);   // fs = velocity scale factor 
   			for (int i=0; i<n_part; ++i){
     				vx[i] *= fs;
    				vy[i] *= fs;
     				vz[i] *= fs;
			//ora le velocità sono compatibili con la temperatura.
     				xold[i] = Pbc(x[i] - vx[i] * delta);
     				yold[i] = Pbc(y[i] - vy[i] * delta);
     				zold[i] = Pbc(z[i] - vz[i] * delta);
  			}		
  		}
  	}
}	
//**********************************************************************************************************************************	
void MolDyn :: Move(void){ //Move particles with Verlet algorithm
	double xnew, ynew, znew, fx[n_part], fy[n_part], fz[n_part];
	for(int i=0; i<n_part; ++i){ //Force acting on particle i
    		fx[i] = Force(i,0);
    		fy[i] = Force(i,1);
    		fz[i] = Force(i,2);
  	}
  	for(int i=0; i<n_part; ++i){ //Verlet integration scheme
		xnew = Pbc( 2.0 * x[i] - xold[i] + fx[i] * pow(delta,2) );
		ynew = Pbc( 2.0 * y[i] - yold[i] + fy[i] * pow(delta,2) );
		znew = Pbc( 2.0 * z[i] - zold[i] + fz[i] * pow(delta,2) );
		vx[i] = Pbc(xnew - xold[i])/(2.0 * delta);
		vy[i] = Pbc(ynew - yold[i])/(2.0 * delta);
		vz[i] = Pbc(znew - zold[i])/(2.0 * delta);
		xold[i] = x[i];
		yold[i] = y[i];
		zold[i] = z[i];
    		x[i] = xnew;
    		y[i] = ynew;
    		z[i] = znew;
  }
}	


//FORZE.
double MolDyn :: Force(int ip, int idir){ //Compute forces as -Grad_ip V(r)
	double f=0.0;
	double dvec[3], dr;

	for (int i=0; i<n_part; ++i){
		if(i != ip){
			dvec[0] = Pbc( x[ip] - x[i] );  // distance ip-i in pbc
			dvec[1] = Pbc( y[ip] - y[i] );
			dvec[2] = Pbc( z[ip] - z[i] );
			dr = dvec[0]*dvec[0] + dvec[1]*dvec[1] + dvec[2]*dvec[2];
			dr = sqrt(dr);
			if(dr < rcut){
  		      		f += dvec[idir] * (48.0/pow(dr,14) - 24.0/pow(dr,8)); // -Grad_ip V(r)
      			}
    		}
  	}
return f;
}

//MEASURE.*********************************
void MolDyn :: Measure(){ //Properties measurement
	double v, t, vij;
	double dx, dy, dz, dr;
	ofstream Epot, Ekin, Etot, Temp, Press;
	Epot.open("istantanei/output_epot.dat",ios::app);
	Ekin.open("istantanei/output_ekin.dat",ios::app);
	Temp.open("istantanei/output_temp.dat",ios::app);
	Etot.open("istantanei/output_etot.dat",ios::app);
	Press.open("istantanei/output_press.dat",ios::app);
	v = 0.0; //reset observables
	t = 0.0;
	for (int k=0; k<nbins; ++k) walker[k]=0.0;
//cycle over pairs of particles
	for (int i=0; i<n_part-1; ++i){
		for (int j=i+1; j<n_part; ++j){
     			dx = Pbc( x[i] - x[j] );
     			dy = Pbc( y[i] - y[j] );
     			dz = Pbc( z[i] - z[j] );
     			dr = dx*dx + dy*dy + dz*dz;
     			dr = sqrt(dr);
     			//g(r)
     			if(dr<(box/2.0)) {
				bin=int((dr/bin_size));
				walker[bin]+=2;
			}
    			
     			if(dr < rcut){
       				vij = 4.0/pow(dr,12) - 4.0/pow(dr,6);
//Potential energy
       				v += vij;
     			}
    		}          
  	}

//g(r)
		
  	for(int i=0; i<nbins; i++) {
  		double r=double(i)*bin_size;
  		double norm=4.*rho*n_part*M_PI/3.*((pow((r+bin_size),3.))-(pow(r,3.)));
  		walker[i]/=norm;
  	}
//Kinetic energy
	for (int i=0; i<n_part; ++i) t += 0.5 * (vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i]);
   
	stima_pot = v/(double)n_part; //Potential energy
	stima_kin = t/(double)n_part; //Kinetic energy
	stima_temp = (2.0 / 3.0) * t/(double)n_part; //Temperature
	stima_etot = (t+v)/(double)n_part; //Total enery
	Epot << stima_pot  << endl;
	Ekin << stima_kin  << endl;
	Temp << stima_temp << endl;
	Etot << stima_etot << endl;

	Epot.close();
	Ekin.close();
	Temp.close();
	Etot.close();
//Pressure	
	double sum=0.0;
	double dvec[3];
	for (int i=0; i<n_part-1; ++i){
		for( int j=i+1; j<n_part; j++) {
				dvec[0] = Pbc( x[j] - x[i] );  // distance ip-i in pbc
				dvec[1] = Pbc( y[j] - y[i] );
				dvec[2] = Pbc( z[j] - z[i] );
				dr = dvec[0]*dvec[0] + dvec[1]*dvec[1] + dvec[2]*dvec[2];
				dr = sqrt(dr);
				if(dr < rcut){
					sum +=(48.0/pow(dr,12)-24.0/pow(dr,6));
      				}
    		}
    	}
    	stima_P=rho*temp+sum/(3.*vol);
    	Press<<stima_P<<endl;
    	Press.close();
    return;
}
//****************************************
double MolDyn :: GetU(void) {
	return stima_pot;
}
double MolDyn :: GetK(void) {
	return stima_kin;
}
double MolDyn :: GetE(void) {
	return stima_etot;
}
double MolDyn :: GetT(void) {
	return stima_temp;
}
double MolDyn :: GetP(void) {
	return stima_P;
}

double MolDyn :: GetG(int indice) {
	return walker[indice];
}




//STAMPA LA CONFIGURAZIONE FINALE.

void MolDyn :: ConfFinal(void){ //Write final configuration
	ofstream WriteConf;
	cout << "Print final configuration to file config.final " << endl << endl;
	WriteConf.open("config/config.final");
	for (int i=0; i<n_part; ++i){
		WriteConf << x[i]/box << "   " <<  y[i]/box << "   " << z[i]/box << endl;
	}
  	WriteConf.close();
  	WriteConf.open("config/config.prefinal");
  	for (int i=0; i<n_part; ++i){
		WriteConf << xold[i]/box << "   " <<  yold[i]/box << "   " << zold[i]/box << endl;
	}
  	WriteConf.close();
  return;
}

void MolDyn :: ConfXYZ(int nconf){ //Write configuration in .xyz format
	ofstream WriteXYZ;
	WriteXYZ.open("frames/config_" + to_string(nconf) + ".xyz");
	WriteXYZ << n_part << endl;
	WriteXYZ << "This is only a comment!" << endl;
	for (int i=0; i<n_part; ++i){
		WriteXYZ << "LJ  " << Pbc(x[i]) << "   " <<  Pbc(y[i]) << "   " << Pbc(z[i]) << endl;
	}
  	WriteXYZ.close();
}


//***********
double MolDyn :: Pbc(double r){  //Algorithm for periodic boundary conditions with side L=box
    return r - box * rint(r/box);
}

















