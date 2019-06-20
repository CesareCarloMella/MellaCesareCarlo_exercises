//IMPLEMENTAZIONE per Traveling SalesMan

#include "TSM.h"
using namespace std;

//costruttore
Path :: Path(int quante_citta, vector <int> path, vector <City> quali_citta) {
	N=quante_citta;		//Numero città
	percorso=path;		//Sequenza. Si parte da 0!!!!
	this->MisLength(quali_citta);
}

Path :: ~Path() {
}
//misura la lunghezza
void Path :: MisLength(vector <City> quali_citta) {
	L=0.;
	for(int i=0; i<N-1; i++) {
		L+=sqrt((quali_citta[percorso[i]].x-quali_citta[percorso[i+1]].x)*(quali_citta[percorso[i]].x-quali_citta[percorso[i+1]].x)+(quali_citta[percorso[i]].y-quali_citta[percorso[i+1]].y)*(quali_citta[percorso[i]].y-quali_citta[percorso[i+1]].y));
	}
	L+=sqrt((quali_citta[percorso[N-1]].x-quali_citta[percorso[0]].x)*(quali_citta[percorso[N-1]].x-quali_citta[percorso[0]].x)+(quali_citta[percorso[N-1]].y-quali_citta[percorso[0]].y)*(quali_citta[percorso[N-1]].y-quali_citta[percorso[0]].y));
	
}

//restituisce la lunghezza
double Path ::	GetLength(vector <City> quali_citta) {
	this->MisLength(quali_citta);
	return L;
}

//Stampa la sequenza delle posizioni
void Path :: PrintSequence() {
	cout<<"Sequenza"<<endl;
	for(int i=0; i<N; i++) cout<<percorso[i]<<endl;
}
//Stampa le coordinate della città scelta
void Path :: PrintPosition(int whichCity, vector <City> quali_citta) {
	if(whichCity>=N) {
		cout<<"Troppo alto, non ho così tante città"<<endl;
		return ;
	}
	cout<<"Coordinate della città "<<whichCity+1<<" della sequenza (partendo da 1)"<<endl;
	cout<<" x ="<<	quali_citta[percorso[whichCity]].x<< " e y ="<<quali_citta[percorso[whichCity]].y<<endl;
}

//Controlla che le città ci siano tutte e una volta sola.
void Path :: Controllo() {
	int indice=0;
	int controllo=0;
	int appoggio;
	for(indice=0; indice<N; ++indice) {
		controllo=0;	//resetta il controllo
		for(int i=0; i<N; ++i) {
			appoggio=percorso[i];	//che indice è?
			if(controllo==1 and appoggio==indice) {
				cout<<"Una città si ripete!!!!!!!!!!"<<endl;
				return ;
			}
			if(appoggio==indice) {
				controllo=1; 	//trovato
			}
		}
		if(controllo==0) {
			cout<<"Non ho trovato una città!!!!!!!!!!"<<endl;
		}
	}
}	



//Set Sequenza
void Path :: SetSequence(vector <int> path) {
	for(int i=0; i<N; i++) {
		percorso[i]=path[i];
	}
}
			
			
//GetN
int Path :: GetN() {
	return N;
}
//GetElement
int Path :: GetElement(int i){						//restituisce l'indice di città al posto i-esimo della sequenza
	return percorso[i];
}

//SetElement
void Path :: SetElement(int i, int sost){						//Imposta diversamente un elemento
	percorso[i]=sost;
}


//*****************************************************************************************************************************
//*****************************************************************************************************************************


//Selection sort
vector <Path> SelectionSort(vector <Path> popol, vector <City> citta) {		//algoritmo di ordinamento. E' costruito per ritornare una sequenza da assegnare
	int flag;								//indice che indicherà il minimo
	double L;								//variabile per lunghezza
	vector <Path> popolCopy=popol;						//Ne costruisce una copia
	for(int i=0; i<popol[0].GetN()*popol[0].GetN(); ++i) {			//prende numero città da uno a caso.
		L=popolCopy[i].GetLength(citta);				//prima lunghezza di riferimento, prende da quello attualmente salvato
		flag=i;								//primo riferimento
		for(int j=i+1; j<popol[0].GetN()*popol[0].GetN(); ++j) {	//cicla sui successivi fino a quand non trova il minimo.
			if(popolCopy[j].GetLength(citta)<L) {
				L=popolCopy[j].GetLength(citta);		//imposta un nuovo riferimento
				flag=j;						//e indice di riferimento
			}
		}
		Path appoggio=popolCopy[i];
		popolCopy[i]=popolCopy[flag];
		popolCopy[flag]=appoggio;
		popolCopy[flag].Controllo();	
	}
	return popolCopy;
}



//Seleziona i migliori
int Selection( int NumCity, Random *rnd) {			//passare il numero di città.
	double p=2.;	//impostare qui il parametro!!!!
	return int(NumCity*NumCity*(pow(rnd->Rannyu(),p)));
}


//Periodic Boundary condition
int PBC(int N, int n) {		//Passare lunghezza sequenza e indice che si vuole correggere
	if(n>=N) {
		return PBC(N,n-N);	//ributta dentro se necessario
	} else if(n<0) {
		return PBC(N,n+N);
	} else {
		return n;	//altrimenti ok
	}
}

//Mutazioni e Incrocio*****************************************************************************************************************************
//*************************************************************************************************************************************************

//Mutazione: Pair Permutation
Path PairPermutation(Random * rnd, Path sequence) {	//restituisce una sequenza con una permutazione casuale di due indici
	Path seqC=sequence;					//ne costruire una copia
	int quale1=int(rnd->Rannyu(0,sequence.GetN()));		//sceglie il primo indice
	int quale2=int(rnd->Rannyu(0,sequence.GetN()));		//sceglie il secondo indice
	int app=seqC.GetElement(quale1);			//permuta
	seqC.SetElement(quale1, seqC.GetElement(quale2));	//...
	seqC.SetElement(quale2, app);				//...
	seqC.Controllo();					//controlla che sia tutto ok
	return seqC;						//ritorna la sequenza permutata
}




//Mutazione:shift
Path Shift(int n, Path sequence) {					//passare il parametro di shift e il vettore da shiftare
	int N=sequence.GetN();
	Path appoggio=sequence;						//Vettore di appoggio
	for(int i=0; i<N; ++i) {					//Shifta
		sequence.SetElement(PBC(N,i+n), appoggio.GetElement(PBC(N,i)));
	}
	sequence.Controllo();						//Controlla che sia tutto ok
	return sequence;
}
		


//Mutazione: shift a blocchi
Path BlockShift(int n, int m, int start, Path sequence) {		//Passare indice di shift, numero città contigue, indice di partenza, sequenza
	int N=sequence.GetN();
	if(n>=N or m>=N) {//Controllo
		cout<<"Non sono capace di shiftare così. Scegli un numero più piccolo"<<endl;
		return sequence;
	}
	Path appoggio=sequence;						//Vettore di appoggio
	for(int i=start+m-1; i>=start; --i) {
		for(int j=0; j<n; j++) {
			sequence.SetElement(PBC(N,i+j), appoggio.GetElement(PBC(N,i+j+1)));
			sequence.SetElement(PBC(N,i+j+1), appoggio.GetElement(PBC(N,i+j)));
			appoggio=sequence;
		}
	}	
	sequence.Controllo();						//Controlla che sia tutto ok
	return sequence;
}
		
	
	
		 	
//Mutazione: scambio e blocchi
Path BlockSwap(int n1, int n2, int m, Path sequence) {	//Passare primo elemento del primo blocco, primo ... secondo blocco, lunghezza blocco
	int N=sequence.GetN();
	int Max=max(n1,n2);
	int Min=min(n1,n2);
	int distBoundary=min(Max-Min,(N-Max)+Min);			//la più piccola distanza
	if(distBoundary<m) {
		cout<<"Non sono capace di scambiare così. Scegli un numero più piccolo"<<endl;
		return sequence;
	}
	Path appoggio=sequence;
	for(int i=0; i<m; ++i) {
		sequence.SetElement(PBC(N,n1+i), appoggio.GetElement(PBC(N,n2+i)));
		sequence.SetElement(PBC(N,n2+i), appoggio.GetElement(PBC(N,n1+i)));
	}
	
	sequence.Controllo();
	return sequence;
}
	


//Mutazione: inversione
Path Inversion(int n, int m, Path sequence) {			//passare primo elemento del blocco da invertire, lunghezza e sequenza
	int N=sequence.GetN();
	if(m>=N) {
		cout<<"Non sono capace di invertire così. Scegli un numero più piccolo"<<endl;
		return sequence;
	}
	Path appoggio=sequence;
	for(int i=0; i<m; ++i) {
		sequence.SetElement(PBC(N,n+i), appoggio.GetElement(PBC(N,n+m-i-1)));
	}
	
	sequence.Controllo();
	return sequence;	
}



//Crossover

vector <Path> Crossover(Random * rnd,vector <Path> NewGeneration) {
	int quanti=NewGeneration.size();			//Sono gli ultimi due che vanno modificati
	Path parent1=NewGeneration.at(quanti-2);		//Li copia
	Path parent2=NewGeneration.at(quanti-1);
	int N=parent1.GetN();			
	int cut=rnd->Rannyu(0,N);		//dove taglio i due genitori.
	Path figlio=GeneraFiglio(N, cut, parent1, parent2);	//Genera il primo figlio
	figlio.Controllo();					//Check
	NewGeneration.at(quanti-2)=figlio;				//Lo corregge
	figlio=GeneraFiglio( N, cut, parent2, parent1);		//Genera il secondo figlio
	figlio.Controllo();					//Check
	NewGeneration.at(quanti-1)=figlio;				//aggiunge
	return NewGeneration;					//ritorna
}



Path GeneraFiglio(int N,int cut, Path parent1, Path parent2) {
	vector <int> aggiustare;
	for(int i=cut+1; i<N; ++i) {
		aggiustare.push_back(parent1.GetElement(i));
	}
	vector <int> aggiustato;
	for(int i=0; i<N; ++i) {
		for(unsigned int j=0; j<aggiustare.size(); ++j) {
			if(parent2.GetElement(i)==aggiustare.at(j)) {
				aggiustato.push_back(parent2.GetElement(i));
			}
		}
	}
	//Piccolo controllo
	int dim1=aggiustare.size();
	int dim2=aggiustato.size();
	if(dim1!=dim2){ 
		cout<<"Dimensioni diverse, c'è qualcosa che non va ..."<<endl;
		return parent1;
	}
	//Aggiusta.
	for(int i=cut+1; i<N; ++i) {
		parent1.SetElement(i, aggiustato.at(i-cut-1));
	}
	parent1.Controllo();
	return parent1;
}		
	

	
	
	
			
















