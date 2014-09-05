#include <iostream>
#include <stdlib.h>
#include <omp.h>
using namespace std;

#define STEPS	100
#define SIZE	100
#define MOVE	0.1

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

bool **CreateMesh(int I, int J) {
	bool **Mesh = new bool *[I];
	for (int i = 0; i < I; i++) {
		Mesh[i] = new bool[J];
		for (int j = 0; j < J; j++) Mesh[i][j] = false;
	}
	return(Mesh);
}

/* 
 * Boundary condition check using combination of No Flux & Periodic Approach 
 */
 
bool **checkBoundary(bool **Mesh) {
	for (int i = 1; i <= SIZE; i++) {
		if(Mesh[i][0] == true) {
			Mesh[i][0] = false;
			if (Mesh[i][1] == false) {
				Mesh[i][1] = true;
			} else {
				Mesh[i][SIZE] = true;
			}
		}
		
		if(Mesh[i][SIZE + 1] == true) {
			Mesh[i][SIZE + 1] = false;
			if (Mesh[i][SIZE] == false) {
				Mesh[i][SIZE] = true;
			} else {
				Mesh[i][1] = true;
			}
		}
		
		if(Mesh[0][i] == true) {
			Mesh[0][i] = false;
			if (Mesh[1][i] == false) {
				Mesh[1][i] = true;
			} else {
				Mesh[SIZE][i] = true;
			}
		}

		if(Mesh[SIZE + 1][i] == true) {
			Mesh[SIZE + 1][i] = false;
			if (Mesh[SIZE][i] == false) {
				Mesh[SIZE][i] = true;
			} else {
				Mesh[1][i] = true;
			}
		} 
	}
	return(Mesh);
}

void print(bool **Mesh, int t) {
	int populationTotal = 0;
         for (int i = 1; i <= SIZE; i++) {
                        for (int j = 1; j <= SIZE; j++) {
				if (Mesh[i][j] == true) {
					populationTotal++;	
				}
			}
	}
	cout<<t<<"\t"<<populationTotal<<std::endl;
}

int main(int argc, char **argv) {
	srand48(8767134);
	bool *locks = new bool[SIZE + 2];
	for (int i = 0; i < SIZE + 2; i++) locks[i] = false;
	bool **MeshA = CreateMesh(SIZE + 2, SIZE + 2);
	bool **MeshB = CreateMesh (SIZE + 2, SIZE + 2);

	for (int i = 1; i <= SIZE; i++) {
		for (int j = 1; j <= SIZE; j++) {
			if (drand48() < 0.10) {
				MeshA[i][j] = true;
			}
		}
	}

        std::cout<<"Time"<<"\t"<<"population"<<std::endl;	
	print(MeshA, 0);
	for (int n = 0; n < STEPS; n++) {
		#if defined(_OPENMP)
		#pragma omp parallel for default(none) shared(MeshA, MeshB, locks, n)
		#endif
		for (int i = 1; i <= SIZE; i++) {
			lock(i, locks);
			for (int j = 1; j <= SIZE; j++) if (MeshA[i][j] == true) { 
				MeshA[i][j] = false;
				double move = drand48();
				if (move < 1.0*MOVE && MeshB[i-1][j] == false && MeshA[i-1][j] == false) {
					MeshB[i-1][j] = true;
				} else if (move < 2.0*MOVE && MeshB[i+1][j] == false && MeshA[i+1][j] == false) {
					MeshB[i+1][j] = true;
				} else if (move < 3.0*MOVE && MeshB[i][j-1] == false && MeshA[i][j-1] == false) {
					MeshB[i][j-1] = true;
				} else if (move < 4.0*MOVE && MeshB[i][j+1] == false && MeshA[i][j+1] == false) {
					MeshB[i][j+1] = true;
				} else {
					MeshB[i][j] = true;
				}
			} 
			unlock(i, locks);
		}

		MeshB = checkBoundary(MeshB);		
		swap(MeshA, MeshB);
		print(MeshA, n+1);		
/*
		for(int p = 0; p <= SIZE; p++) {
			for(int q = 0; q <= SIZE; q++) {
				if(MeshA[p][q] == true) {
					cout<<"X | ";
				} else {
					cout<<"0 | ";
				}
			}
			cout<<"\n";
		}
		cout<<"\n\n\n\n\n\n\n\n\n";*/
	}
}

