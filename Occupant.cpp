#include <iostream>

using namespace std;

class Occupant{
   
	public:
		double probabilityOfMovement;

		Occupant() {
			probabilityOfMovement = 0.0;	
		}

};

class Human: public Occupant{
	public :
		Human(){
			probabilityOfMovement = 0.1;

		}
};
class Zombie: public Occupant{
	public :
		Zombie(){
			probabilityOfMovement = 0.075;
		}
};

/**int main(){	
 Occupant oc;
 Human h;
 Zombie z;
 h.probabilityOfMovement = 0.7;
 z.probabilityOfMovement = 0.4;
 cout << oc.probabilityOfMovement<<endl;
 cout << h.probabilityOfMovement<<endl;
 cout << z.probabilityOfMovement<<endl;
 return 0;
}**/


