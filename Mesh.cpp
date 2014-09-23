#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include "Occupant.cpp"
#include <math.h>
using namespace std;

#define STEPS	500
#define SIZE 	486
#define DEATHPC 0.0002175
#define BIRTHPC 0.0000358
#define ZOMBIEAGE 28
#define ZOMBIEATTACK 0.7

bool isEmpty(Occupant o) {
  if (o.type == 'O') {
     return true;
  }
  return false;
}

bool isReproducible(Occupant o) {
  if (o.type == 'H' && ((o.gender == 'F' && o.age < 16060 && o.age > 5475) || (o.gender == 'M' && o.age > 5475))) {
    return true;
  }
  return false;
}

bool isExposed(Occupant o) {
  if (o.type == 'E') {
  	 	return true;
  }
  return false;
}

bool isHuman(Occupant o) {
  if (o.type == 'H') {
    return true;
  }
  return false;
}


bool isZombie(Occupant o) {
  if (o.type == 'Z') {
    return true;
  }
  return false;
}

bool isOppositeGender(Occupant h, char gender) {
	if(h.gender == gender) {
		return false;
	}
	return true;
}

void zombify(Occupant **MeshA){
	#if defined(_OPENMP)
	#pragma omp parallel for default(none) shared(MeshA)
	#endif
	for (int i = 1; i <= SIZE; i++) {
		for (int j = 1; j <= SIZE; j++) {
			if (isZombie(MeshA[i][j])){
				if (isHuman(MeshA[i-1][j])	) {
					if (drand48() < ZOMBIEATTACK) {
						MeshA[i-1][j].type = 'E';
						MeshA[i-1][j].exposedPeriod = 0;
					}
				}
				if (isHuman(MeshA[i+1][j])	) {
					if (drand48() < ZOMBIEATTACK) {
						MeshA[i+1][j].type = 'E';
						MeshA[i+1][j].exposedPeriod = 0;
					}
				}
				if (isHuman(MeshA[i][j-1])	) {
					if (drand48() < ZOMBIEATTACK) {
						MeshA[i][j-1].type = 'E';
						MeshA[i][j-1].exposedPeriod = 0;
					}
				}
				if (isHuman(MeshA[i][j+1])	) {
					if (drand48() < ZOMBIEATTACK) {
						MeshA[i][j+1].type = 'E';
						MeshA[i][j+1].exposedPeriod = 0;
					}
				}
			} else if(isExposed(MeshA[i][j])) {
					if(MeshA[i][j].exposedPeriod > 2) {		// Exposed to Zombie takes 3 days
						Zombie z;
						MeshA[i][j] = z;
					} else {
						MeshA[i][j].exposedPeriod++;
					}
			}
		}
	}
}		
				
int getHumanPop(Occupant **Mesh){
	int humanPop = 0;
  for (int i = 1; i <= SIZE; i++) {
    for (int j = 1; j <= SIZE; j++) {
			if (isHuman(Mesh[i][j]) ) {
				humanPop++;	
			}
		}
	}
	return humanPop;
}


int getZombiePop(Occupant **Mesh){
	int zombiePop = 0;
  for (int i = 1; i <= SIZE; i++) {
    for (int j = 1; j <= SIZE; j++) {
			if (isZombie(Mesh[i][j]) ) {
				zombiePop++;	
			}	
		}
	}
	return zombiePop;
}

double getBirthRate(Occupant ** Mesh){
  double delta = getHumanPop(Mesh)/(SIZE*SIZE - getZombiePop(Mesh));
  double birthRate = BIRTHPC/(1-pow(1-delta,4));
  return birthRate;
}

void placeBaby(int i1,int j1,int i2, int j2,Occupant ** Mesh, Occupant ** MeshB){
	double g = drand48();
	char gender;
	if (g < 1.0*0.5) {
		gender = 'M';
	} else {
		gender = 'F';
	}
	Human baby(gender, 0);

	if (i1 == i2){
		if (isEmpty(MeshB[i1-1][j1]) && isEmpty(Mesh[i1-1][j1])){
			MeshB[i1-1][j1] = baby;
		}
		else if (isEmpty(MeshB[i1-1][j2]) && isEmpty(Mesh[i1-1][j2])){
			MeshB[i1-1][j2] = baby;
		}
		else if (isEmpty(MeshB[i1+1][j1]) && isEmpty(Mesh[i1+1][j1])){
			MeshB[i1+1][j1] = baby;
		}
		else if (isEmpty(MeshB[i1+1][j2]) && isEmpty(Mesh[i1+1][j2])){
			MeshB[i1+1][j2] = baby;
		}
		else if (isEmpty(MeshB[i1][min(j1,j2)-1]) && isEmpty(Mesh[i1][min(j1,j2)-1])){	
			MeshB[i1][min(j1,j2)-1] = baby;
		}
		else if (isEmpty(MeshB[i1][max(j1,j2)+1]) && isEmpty(Mesh[i1][max(j1,j2)+1])){	
			MeshB[i1][max(j1,j2)+1]= baby;
		}
	}
	else if (j1==j2){
		if (isEmpty(MeshB[i1][j1-1]) && isEmpty(Mesh[i1][j1-1])){
			MeshB[i1][j1-1] = baby;
		}
		else if (isEmpty(MeshB[i2][j1-1]) && isEmpty(Mesh[i2][j1-1])){	
			MeshB[i2][j1-1] = baby;
		}
		else if (isEmpty(MeshB[i1][j1+1]) && isEmpty(Mesh[i1][j1+1])){			
			MeshB[i1][j1+1] = baby;
		}
		else if (isEmpty(MeshB[i2][j1+1]) && isEmpty(Mesh[i2][j1+1])){
			MeshB[i2][j1+1] = baby;
		}
		else if (isEmpty(MeshB[min(i1,i2)-1][j1]) && isEmpty(Mesh[min(i1,i2)-1][j1])){
			MeshB[min(i1,i2)-1][j1]= baby;
		}
		else if (isEmpty(MeshB[max(i1,i2)+1][j1]) && isEmpty(Mesh[max(i1,i2)+1][j1])){
			MeshB[max(i1,i2)+1][j1]= baby;
		}
	}
}



#if defined(_OPENMP)
void lock(int i, bool *locks, int lockRows) {
	for (bool locked = false; locked == false; /*NOP*/) {
		#pragma omp critical (LockRegion)
		{
			bool temp = true;
			for(int j = 0; j <= lockRows; j++) {
				locked = temp && !locks[i - j] && !locks[i + j];
				temp = locked;
			}
			if (locked) {
				for(int j = 0; j <= lockRows; j++) {
					locks[i-j] = true;
				}
			}
		}
	}
}
void unlock(int i, bool *locks, int lockRows) {
	#pragma omp critical (LockRegion)
	{
		for(int j = 0; j <= lockRows; j++) {
			locks[i - j] = false;
		} 
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
			}
			Mesh[i][0] = temp;
		}
		
		if(!isEmpty(Mesh[i][SIZE + 1])) {
			if (isEmpty(Mesh[i][SIZE])) {
				Mesh[i][SIZE] = Mesh[i][SIZE + 1];
			} 
			Mesh[i][SIZE + 1] = temp;
		}
		
		if(!isEmpty(Mesh[0][i])) {
			if (isEmpty(Mesh[SIZE][i])) {
				Mesh[SIZE][i] = Mesh[0][i];
			} 
			Mesh[0][i] = temp;
		}

		if(!isEmpty(Mesh[SIZE + 1][i])) {
			if (isEmpty(Mesh[1][i])) {
				Mesh[1][i] = Mesh[SIZE + 1][i];
			} 
			Mesh[SIZE + 1][i] = temp;
		} 
	}
	return(Mesh);
}
		
void print(Occupant **Mesh, int t) {
	int humanPop = 0;
	int zombiePop = 0;
	int exposedPop = 0;
  for (int i = 1; i <= SIZE; i++) {
    for (int j = 1; j <= SIZE; j++) {
			if (isHuman(Mesh[i][j])) {
				humanPop++;	
			} else if (isZombie(Mesh[i][j])){
					zombiePop++;
			} else if (isExposed(Mesh[i][j])) {
					exposedPop++;
			}
		}
	}
	cout<<t<<"\t"<<humanPop<<"\t"<<zombiePop<<"\t"<<exposedPop<<std::endl;
}

int main(int argc, char **argv) {
	srand48(getpid());
	int random;
	bool *locks = new bool[SIZE + 2];
	for (int i = 0; i < SIZE + 2; i++) locks[i] = false;
	Occupant **MeshA = CreateMesh(SIZE + 2, SIZE + 2);
	Occupant **MeshB = CreateMesh (SIZE + 2, SIZE + 2);

	for (int i = 1; i <= SIZE; i++) {
		for (int j = 1; j <= SIZE; j++) {
			//#zombies should be reduced
			double randNum = drand48();
			double ageDistribution = drand48();
			if (randNum < 0.01) {
				// Modification required to incorporate Male, Female & Zombie differentiation
				if(getZombiePop(MeshA) < 2) {
					Zombie z;
					MeshA[i][j] = z;
				}
			} else {
				if (ageDistribution < 0.182) {
					 random = rand()%(5475 - 0 + 1) + 0;
				}
				else if (ageDistribution < 0.61) {
					 random = rand()%(16060 - 5475 + 1) + 5475;
				} else {
					 random = rand()%(36500 - 16060 + 1) + 16060;
				}
				if(randNum < 0.05){
					char gender = 'M';
					Human h(gender, random);
					MeshA[i][j] = h;
				} else if (randNum < 0.10) {
					char gender = 'F';
					Human h(gender, random);
					MeshA[i][j] = h;
				}	
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
			lock(i, locks, 1);
			for (int j = 1; j <= SIZE; j++) {
				if (!isEmpty(MeshA[i][j])) { 
					double move = drand48();
					MeshA[i][j].age++ ;
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
				}
		} 
			unlock(i, locks, 1);
		}

		MeshB = checkBoundary(MeshB);		
		swap(MeshA, MeshB);
		print(MeshA, n+1);	
/*
cout<< "\n\nAFTER Moving  - Mesh A \n";
		for(int p = 1; p <= SIZE; p++) {
			for(int q = 1; q <= SIZE; q++) {
				if(isHuman(MeshA[p][q])) {
					cout<<MeshA[p][q].age<<" H | ";
				} else if (isZombie(MeshA[p][q])) {
					cout<<MeshA[p][q].age<<" Z | ";
				}else{
					cout<<"    | ";
				}
			}
			cout<<"\n";
		}
		cout<<"\n\n\n\n\n\n\n\n\n";
*/
		//Birth
		#if defined(_OPENMP)
		#pragma omp parallel for default(none) shared(MeshA, MeshB, locks, n)
		#endif
		for (int i = 1; i <= SIZE; i++) {
			lock(i, locks, 2);
			for (int j = 1; j <= SIZE; j++) {
				if (isReproducible(MeshA[i][j])){
					if (isReproducible(MeshA[i][j-1]) && isOppositeGender(MeshA[i][j-1], MeshA[i][j].gender)){
						double birth = drand48();
						if (birth <= 1.0*getBirthRate(MeshA)){
							placeBaby(i,j,i,j-1,MeshA, MeshB);
							MeshB[i][j]= MeshA[i][j];
							Occupant temp;
							MeshA[i][j] = temp;	
							continue;
						}						
					}
					else if (isReproducible(MeshA[i][j+1]) && isOppositeGender(MeshA[i][j+1], MeshA[i][j].gender)){
						double birth = drand48();
						if (birth <= 1.0*getBirthRate(MeshA)){
							placeBaby(i,j,i,j+1,MeshA, MeshB);
							MeshB[i][j]= MeshA[i][j];
							Occupant temp;
							MeshA[i][j] = temp;	
							continue;
						}			
					}

					else if (isReproducible(MeshA[i-1][j]) && isOppositeGender(MeshA[i - 1][j], MeshA[i][j].gender)){
						double birth = drand48();
						if (birth <= 1.0*getBirthRate(MeshA)){
							placeBaby(i,j,i-1,j,MeshA, MeshB);
							MeshB[i][j]= MeshA[i][j];
							Occupant temp;
							MeshA[i][j] = temp;	
							continue;
						}			
					}

					else if (isReproducible(MeshA[i+1][j]) && isOppositeGender(MeshA[i + 1][j], MeshA[i][j].gender)){
						double birth = drand48();
						if (birth <= 1.0*getBirthRate(MeshA)){
							placeBaby(i,j,i+1,j,MeshA, MeshB);
							MeshB[i][j]= MeshA[i][j];
							Occupant temp;
							MeshA[i][j] = temp;	
							continue;
						}			
					} else {
						MeshB[i][j]= MeshA[i][j];
						Occupant temp;
						MeshA[i][j] = temp;	
					}
				}	else {
						MeshB[i][j]= MeshA[i][j];
						Occupant temp;
						MeshA[i][j] = temp;
				}
			}
			unlock(i, locks, 2);
		}		
		MeshB = checkBoundary(MeshB);		
		swap(MeshA, MeshB);
		
		// Death 
		for (int i = 1; i <= SIZE; i++) {
			for (int j = 1; j <= SIZE; j++) {
				double die = drand48();
				if (isHuman(MeshA[i][j]) && die < 1.0*DEATHPC) {
					Occupant temp;
					MeshA[i][j] = temp;
				} else if (isZombie(MeshA[i][j]) && MeshA[i][j].age == ZOMBIEAGE ) {
            Occupant temp;
            MeshA[i][j] = temp;
        }
			}
		}

	// Zombification
	zombify(MeshA);
/*
		cout<< "\n\nMesh A \n";
		for(int p = 1; p <= SIZE; p++) {
			for(int q = 1; q <= SIZE; q++) {
				if(isHuman(MeshA[p][q])) {
					cout<<MeshA[p][q].gender<<" H | ";
				} else if (isZombie(MeshA[p][q])) {
					cout<<MeshA[p][q].age<<" Z | ";
				}else if (isExposed(MeshA[p][q])) {
					cout<<MeshA[p][q].age<<" E | ";
				} else {
					cout<<"    | ";
				}
			}
			cout<<"\n";
		}
		cout<<"\n\n\n\n\n\n\n\n\n";

cout<< "\n\nMesh B \n";
		for(int p = 1; p <= SIZE; p++) {
			for(int q = 1; q <= SIZE; q++) {
				if(isHuman(MeshB[p][q])) {
					cout<<MeshB[p][q].gender<<" H | ";
				} else if (isZombie(MeshB[p][q])) {
					cout<<MeshB[p][q].age<<" Z | ";
				}else if (isExposed(MeshB[p][q])) {
					cout<<MeshB[p][q].age<<" E | ";
				} else {
					cout<<"    | ";
				}
			}
			cout<<"\n";
		}
		cout<<"\n\n\n\n\n\n\n\n\n";
		*/

	}
}

