#include <iostream>

using namespace std;

class Occupant{
    
    public:
        double probabilityOfMovement;
        char type;
        int age;
        int exposedPeriod;
        char gender;
    
        Occupant() {
            probabilityOfMovement = 0.0;
            type = 'O';
            age = 0;
            gender = ' ';
        }
        Occupant(char ty) {
        
            gender = ' ';
            age = 0;
            if (ty == 'H')
            {
                probabilityOfMovement = 0.1;
                type = 'H';
            }
            else
            if (ty == 'Z')
            {
                probabilityOfMovement = 0.075;
                type = 'Z';
            }else
                if (ty == 'E')
                {
                    probabilityOfMovement = 0.0;
                    type = 'E';
                }
                else
                {
                    probabilityOfMovement = 0.0;
                    type = 'O';
                }
        
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

