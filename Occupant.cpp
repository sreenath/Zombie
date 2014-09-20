#include <iostream>

using namespace std;

class Occupant{
   
	public:
		double probabilityOfMovement;
		char type;
		int age;
		char gender;

		Occupant() {
			probabilityOfMovement = 0.0;	
			type = 'O';
			age = 0;
			gender = ' ';
		}
};

class Human: public Occupant{
	public :
		char gender;
		Human(){
			probabilityOfMovement = 0.1;
			type = 'H';
			age = 0;
		}
		Human(char c) {
			probabilityOfMovement = 0.1;
			type = 'H';
			age = 0;
			gender = c;
		}
};


class Zombie: public Occupant{
	public :
		Zombie(){
			probabilityOfMovement = 0.075;
			type = 'Z';
			 age = 0;
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


