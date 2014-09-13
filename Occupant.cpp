#include <iostream>

using namespace std;

class Occupant{
   
	public:
		double probabilityOfMovement;
		
	//	Occupant(double p) {
	//		probabilityOfMovement = p;
	//	}

};

class Human: public Occupant{

};
class Zombie: public Occupant{

};

/**int main(){	
 Occupant oc ;
 Human h;
 Zombie z;
 oc.probabilityOfMovement = 0.333;
 h.probabilityOfMovement = 0.7;
 z.probabilityOfMovement = 0.4;
 cout << oc.probabilityOfMovement<<endl;
 cout << h.probabilityOfMovement<<endl;
 cout << z.probabilityOfMovement<<endl;

}**/


