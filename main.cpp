#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

using namespace std;

//------------Hrac----------------

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
};

//-----------Postavy------------

Hrac VyberPostavy(){

    while (true) {
        cout << "Vyber si postavu" << endl;
        cout << "1) Rytíř" << endl;
        cout << "2) Lovec" << endl;
        cout << "3) Mág" << endl;
        int C;
        cin >> C;

        Hrac p;

        if(c==1){
            p.Class = "Rytíř";
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

        }else (c==3){
            p.Class = "Mág";
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

    cout << "Vítej v RPG " << endl;

    cout << "Class: " << Hrac.Class << endl;
    cout << "Level: " << Hrac.level << "  XP: " << Hrac.xp << endl;
    cout << "HP: " << Hrac.hp << "/" << Hrac.maxHP
    cout << "Mana: " << Hrac.mana << "/" << Hrac.maxMana << endl;
    cout << "Utok: " << Hrac.attack << "  Gold: " << Hrac.gold << endl;




    cout <<"Diky";
    return 0;
}
