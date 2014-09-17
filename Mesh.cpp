#include <iostream>
#include <stdlib.h>
#include <omp.h>
<<<<<<< HEAD
#include <unistd.h>
=======
>>>>>>> FETCH_HEAD
#include "Occupant.cpp"
using namespace std;

#define STEPS	100
#define SIZE 	100

bool isEmpty(Occupant o) {
	if (o.probabilityOfMovement == 0.0) {
		return true;
		} 
		return false;
}

#if defined(_OPENMP)
void lock(int i, bool *locks) {
	for (bool locked = false; locked == false; /*NOP*/) {
		#pragma omp critical (LockRegion)
		{
			locked = !locks[i-1] && !locks[i] && !locks[i+1];
			if (locked) {
				locks[i-1] = true; locks[i] = true; locks[i+1] = true;
			}
		}
	}
}
void unlock(int i, bool *locks) {
	#pragma omp critical (LockRegion)
	{
		locks[i-1] = false; locks[i] = false; locks[i+1] = false;
	}
}
#endif

Occupant **CreateMesh(int I, int J) {
	Occupant **Mesh = new Occupant *[I];
	for (int i = 0; i < I; i++) {
		Mesh[i] = new Occupant[J];
		for (int j = 0; j < J; j++) {
		  	
			//Mesh[i][j] = NULL;	
		}
	}
	return(Mesh);
}

/* 
 * Boundary condition check using combination of No Flux & Periodic Approach 
 */

 
Occupant **checkBoundary(Occupant **Mesh) {
	Occupant temp;
	for (int i = 1; i <= SIZE; i++) {
		if(!isEmpty(Mesh[i][0])) {
			if (isEmpty(Mesh[i][1])) {
				Mesh[i][1] = Mesh[i][0];
			} else {
				Mesh[i][SIZE] = Mesh[i][0];
			}
			Mesh[i][0] = temp;
		}
		
		if(!isEmpty(Mesh[i][SIZE + 1])) {
			if (isEmpty(Mesh[i][SIZE])) {
				Mesh[i][SIZE] = Mesh[i][SIZE + 1];
			} else {
				Mesh[i][1] = Mesh[i][SIZE + 1];
			}
			Mesh[i][SIZE + 1] = temp;
		}
		
		if(!isEmpty(Mesh[0][i])) {
			if (isEmpty(Mesh[1][i])) {
				Mesh[1][i] = Mesh[0][i];
			} else {
				Mesh[SIZE][i] = Mesh[0][i];
			}
			Mesh[0][i] = temp;
		}

		if(!isEmpty(Mesh[SIZE + 1][i])) {
			if (isEmpty(Mesh[SIZE][i])) {
				Mesh[SIZE][i] = Mesh[SIZE + 1][i];
			} else {
				Mesh[1][i] = Mesh[SIZE + 1][i];
			}
			Mesh[SIZE + 1][i] = temp;
		} 
	}
	return(Mesh);
}
<<<<<<< HEAD
		
=======


>>>>>>> FETCH_HEAD
void print(Occupant **Mesh, int t) {
	int humanPop = 0;
	int zombiePop = 0;
         for (int i = 1; i <= SIZE; i++) {
                        for (int j = 1; j <= SIZE; j++) {
				if (Mesh[i][j].probabilityOfMovement == 0.1 ) {
					humanPop++;	
				}
				else if (Mesh[i][j].probabilityOfMovement == 0.075 ){
					zombiePop++;
				}
			}
	}
	cout<<t<<"\t"<<humanPop<<"\t"<<zombiePop<<std::endl;
}

int main(int argc, char **argv) {
	srand48(getpid());
	bool *locks = new bool[SIZE + 2];
	for (int i = 0; i < SIZE + 2; i++) locks[i] = false;
	Occupant **MeshA = CreateMesh(SIZE + 2, SIZE + 2);
	Occupant **MeshB = CreateMesh (SIZE + 2, SIZE + 2);

	for (int i = 1; i <= SIZE; i++) {
		for (int j = 1; j <= SIZE; j++) {
<<<<<<< HEAD
			//#zombies should be reduced
=======
>>>>>>> FETCH_HEAD
			if (drand48() < 0.01) {
				// Modification required to incorporate Male, Female & Zombie differentiation
				Zombie z;
				MeshA[i][j] = z;
			}
			else if(drand48()<0.10){
				Human h;
				MeshA[i][j] = h;
			}
		}
	}

        std::cout<<"Time"<<"\t"<<"humanPop"<<"\t"<<"zombiePop"<<std::endl;	
	print(MeshA, 0);

	for (int n = 0; n < STEPS; n++) {
		#if defined(_OPENMP)
		#pragma omp parallel for default(none) shared(MeshA, MeshB, locks, n)
		#endif
	
		for (int i = 1; i <= SIZE; i++) {
			lock(i, locks);
<<<<<<< HEAD
			for (int j = 1; j <= SIZE; j++) {
				if (!isEmpty(MeshA[i][j])) { 
					double move = drand48();
					if (move < 1.0*MeshA[i][j].probabilityOfMovement && isEmpty(MeshB[i-1][j]) && isEmpty(MeshA[i-1][j])) {
						MeshB[i-1][j] = MeshA[i][j];
					} else if (move < 2.0*MeshA[i][j].probabilityOfMovement && isEmpty(MeshB[i+1][j]) && isEmpty(MeshA[i+1][j])) {
							MeshB[i+1][j] = MeshA[i][j];
					} else if (move < 3.0*MeshA[i][j].probabilityOfMovement && isEmpty(MeshB[i][j-1]) && isEmpty(MeshA[i][j-1])) {
							MeshB[i][j-1] = MeshA[i][j];
					} else if (move < 4.0*MeshA[i][j].probabilityOfMovement && isEmpty(MeshB[i][j+1]) && isEmpty(MeshA[i][j+1])) {
							MeshB[i][j+1] = MeshA[i][j];
					} else {
							MeshB[i][j] = MeshA[i][j];
					}
				
					Occupant temp;
					MeshA[i][j] = temp;	
=======
			for (int j = 1; j <= SIZE; j++) if (MeshA[i][j] == true) { 
				// Modification required to differentiate movement of human & zombie
				MeshA[i][j] = false;
				double move = drand48();
				if (move < 1.0*MOVE && MeshB[i-1][j].probabilityOfMovemnt == 0.0 && MeshA[i-1][j].probabilityMovement == 0.0) {
					//Must be examined
					MeshB[i-1][j] = true;
				} else if (move < 2.0*MOVE && MeshB[i+1][j].probabilityOfMovement == 0.0 && MeshA[i+1][j].probabilityOfMovement == 0.0) {
					//Must be examined
					MeshB[i+1][j] = true;
				} else if (move < 3.0*MOVE && MeshB[i][j-1].pbilityOfMovement == 0.0 && MeshA[i][j-1].probabilityOfMovement == 0.0) {
					//Must be examined
					MeshB[i][j-1] = true;
				} else if (move < 4.0*MOVE && MeshB[i][j+1].probabilityOfMovement == 0.0 && MeshA[i][j+1].probabilityOfMovement == 0.0) {
					//Must be examined
					MeshB[i][j+1] = true;
				} else {
					//Must be examined
					MeshB[i][j] = true;
>>>>>>> FETCH_HEAD
				}
		} 
			unlock(i, locks);
		}

		MeshB = checkBoundary(MeshB);		
		swap(MeshA, MeshB);
		print(MeshA, n+1);	

<<<<<<< HEAD
/*
		cout<< "\n\nMesh A \n";
		for(int p = 0; p <= SIZE; p++) {
=======
/*		for(int p = 0; p <= SIZE; p++) {
>>>>>>> FETCH_HEAD
			for(int q = 0; q <= SIZE; q++) {
				if(MeshA[p][q].probabilityOfMovement == 0.1 ) {
					cout<<"H | ";
				} else if (MeshA[p][q].probabilityOfMovement == 0.075) {
					cout<<"Z | ";
				}else{
					cout<<"  | ";
				}
			}
			cout<<"\n";
		}
		cout<<"\n\n\n\n\n\n\n\n\n";
<<<<<<< HEAD
*/
	}
=======
	}*/
>>>>>>> FETCH_HEAD
}

