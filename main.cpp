#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

using namespace std;

//------------Hrac--------------

struct Hrac{

    int HP;
    int MaxHP;
    int XP;
    int Level;
    int Mana;
    int MaxMana;
    int Utok;
    int Gold;
    string Class;
}



void LevelUp(Hrac &p) {
    while (p.XP >= 10 * p.Level) {
        p.XP -= 10 * p.Level;
        p.Level++;
        p.MaxHP += 2;
        p.Utok += 1;
        p.HP = p.MaxHP;
        cout << "\n*** LEVEL UP! ***\n";
        cout << "Nový level: " << p.Level << "\n";
        cout << "Max HP zvýšeno a útok zvýšen.\n";
    }
}




//-----------Postavy------------

Hrac VyberPostavy(){

    while (true) {
        cout << "Vyber si postavu" << endl;
        cout << "1) Rytir" << endl;
        cout << "2) Lovec" << endl;
        cout << "3) Mag" << endl;
        int c;
        cin >> c;

        Hrac p;

        if(c==1){
            p.Class = "Rytir";
            p.MaxHP =
            p.MaxMana = 0;
            p.Gold = 10;
            p.Level = 1;
            p.XP = 0;
            p.Utok =

        }else if (c==2){
            p.Class = "Lovec";
            p.MaxHP =
            p.MaxMana = 10;
            p.Gold = 10;
            p.Level = 1;
            p.XP = 0;
            p.Utok =

        }else if (c==3){
            p.Class = "Mag";
            p.MaxHP =
            p.MaxMana = 30;
            p.Gold = 10;
            p.Level = 1;
            p.XP = 0;
            p.Utok =
        }


    }


}





//-----------Monstra------------
struct Monstra{

    int MonstraHP;
    int MonstraUtok;
    int XPDrop;
    int GoldDrop;
};
//------------Main--------------
int main() {

    Hrac Hrac = VyberPostavy();




    return 0;
}
