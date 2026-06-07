#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// -------------------- UTIL --------------------
int nahodneCislo(int a, int b) {
    return a + rand() % (b - a + 1);
}

// -------------------- SCHOPNOST --------------------
struct Schopnost {
    string nazev;
    string popis;
    int manaCena;
    int dmgSingle;
    int dmgVsem;
    int leceni;
};

// -------------------- HRAC --------------------
struct Hrac {
    string trida;
    int maxHP;
    int hp;
    int maxMana;
    int mana;
    int zlato;
    int level;
    int xp;
    int utok;
    vector<Schopnost> schopnosti;
};

void vypisStatyHrace(const Hrac &h) {
    cout << "Trida: " << h.trida << "\n";
    cout << "Level: " << h.level << "  XP: " << h.xp << "\n";
    cout << "HP: " << h.hp << "/" << h.maxHP
         << "  Mana: " << h.mana << "/" << h.maxMana << "\n";
    cout << "Utok: " << h.utok << "  Zlato: " << h.zlato << "\n";
}

void levelUp(Hrac &h) {
    while (h.xp >= 10 * h.level) {
        h.xp -= 10 * h.level;
        h.level++;
        h.maxHP += 2;
        h.maxMana += 1;
        h.utok += 1;
        h.hp = h.maxHP;
        h.mana = h.maxMana;

        cout << "\n*** LEVEL UP! ***\n";
        cout << "Novy level: " << h.level << "\n";
        cout << "Max HP zvyseno, utok zvysen, mana zvysena.\n\n";
    }
}
// -------------------- MONSTRA --------------------
struct Monstrum {
    string nazev;
    int hp;
    int utok;
    int xpOdm;
    int zlatoMin;
    int zlatoMax;
    bool vzdyZlato;
    bool jeMiniBoss;
    bool jeBoss;
};

Monstrum vytvorMonstrum(string nazev, int hp, int utok, int xp, int gmin, int gmax, bool vzdy=false, bool mini=false, bool boss=false) {
    Monstrum m;
    m.nazev = nazev;
    m.hp = hp;
    m.utok = utok;
    m.xpOdm = xp;
    m.zlatoMin = gmin;
    m.zlatoMax = gmax;
    m.vzdyZlato = vzdy;
    m.jeMiniBoss = mini;
    m.jeBoss = boss;
    return m;
}

// -------------------- BLINKRON BOSS --------------------
struct Blinkron {
    Monstrum zaklad;
    int kolo;
    bool neviditelny;
    float bonusDalsiUtok;
};

Blinkron vytvorBlinkron() {
    Blinkron b;
    b.zaklad = vytvorMonstrum("Blinkron", 80, 10, 50, 30, 60, true, false, true);
    b.kolo = 0;
    b.neviditelny = false;
    b.bonusDalsiUtok = 1.0f;
    return b;
}
// -------------------- FUNKCE --------------------
bool monstrumZije(const Monstrum &m) {
    return m.hp > 0;
}

void odmenaHrace(Hrac &h, const Monstrum &m) {
    h.xp += m.xpOdm;
    cout << "Ziskal jsi " << m.xpOdm << " XP.\n";

    bool drop = m.vzdyZlato || (nahodneCislo(0, 1) == 1);
    if (drop) {
        int g = nahodneCislo(m.zlatoMin, m.zlatoMax);
        h.zlato += g;
        cout << "Nasel jsi " << g << " zlata.\n";
    }

    levelUp(h);
}

// -------------------- VESNICE --------------------
void vesnice(Hrac &h) {
    bool konec = false;

    while (!konec) {
        cout << "\n=== VESNICE ===\n";
        vypisStatyHrace(h);

        cout << "1) Vylecit HP (5 zlata, doplni na max)\n";
        cout << "2) Zvysit max HP (+2, 10 zlata)\n";
        cout << "3) Zvysit max Manu (+1, 8 zlata)\n";
        cout << "4) Zvysit utok (+1, 12 zlata)\n";
        cout << "5) Odejit\n";
        cout << "Volba: ";

        int volba;
        cin >> volba;

        switch (volba) {
            case 1:
                if (h.zlato >= 5) {
                    h.zlato -= 5;
                    h.hp = h.maxHP;
                    cout << "HP doplneny.\n";
                } else cout << "Nemas dost zlata.\n";
                break;

            case 2:
                if (h.zlato >= 10) {
                    h.zlato -= 10;
                    h.maxHP += 2;
                    cout << "Max HP zvyseno.\n";
                } else cout << "Nemas dost zlata.\n";
                break;

            case 3:
                if (h.zlato >= 8) {
                    h.zlato -= 8;
                    h.maxMana += 1;
                    cout << "Max mana zvysena.\n";
                } else cout << "Nemas dost zlata.\n";
                break;

            case 4:
                if (h.zlato >= 12) {
                    h.zlato -= 12;
                    h.utok += 1;
                    cout << "Utok zvysen.\n";
                } else cout << "Nemas dost zlata.\n";
                break;

            case 5:
                konec = true;
                break;

            default:
                cout << "Neplatna volba.\n";
        }
    }
}
// -------------------- SOUBOJ MULTI-MONSTRUM --------------------
bool souboj(Hrac &h, vector<Monstrum> nepratele, bool nepratelePrvni) {
    cout << "\n=== SOUBOJ ZACINA ===\n";

    while (h.hp > 0) {

        // zjisteni, zda nekdo zije
        bool nekdoZije = false;
        for (auto &e : nepratele)
            if (monstrumZije(e)) nekdoZije = true;

        if (!nekdoZije) {
            cout << "Vsechna monstra porazena!\n";
            for (auto &e : nepratele)
                odmenaHrace(h, e);
            return true;
        }

        // utoci nepratele jako prvni?
        if (nepratelePrvni) {
            for (auto &e : nepratele) {
                if (!monstrumZije(e)) continue;
                cout << e.nazev << " utoci za " << e.utok << " dmg.\n";
                h.hp -= e.utok;
                if (h.hp <= 0) {
                    cout << "Byl jsi zabit.\n";
                    return false;
                }
            }
        }

        // kolo hrace
        cout << "\nTvoje kolo.\n";
        vypisStatyHrace(h);

        cout << "Nepratele:\n";
        for (size_t i = 0; i < nepratele.size(); ++i) {
            cout << i+1 << ") " << nepratele[i].nazev
                 << " HP: " << nepratele[i].hp << "\n";
        }

        cout << "Vyber akci:\n";
        cout << "1) Zakladni utok\n";

        for (size_t i = 0; i < h.schopnosti.size(); ++i) {
            cout << (i+2) << ") " << h.schopnosti[i].nazev
                 << " (mana " << h.schopnosti[i].manaCena << ")\n";
        }

        int volba;
        cin >> volba;

        // zakladni utok
        if (volba == 1) {
            int cil;
            cout << "Cislo cile: ";
            cin >> cil;

            if (cil < 1 || cil > (int)nepratele.size() || !monstrumZije(nepratele[cil-1])) {
                cout << "Spatny cil.\n";
            } else {
                cout << "Utocis na " << nepratele[cil-1].nazev
                     << " za " << h.utok << " dmg.\n";
                nepratele[cil-1].hp -= h.utok;
            }
        }
        else {
            // schopnosti
            int idx = volba - 2;

            if (idx < 0 || idx >= (int)h.schopnosti.size()) {
                cout << "Neplatna schopnost.\n";
            } else {
                Schopnost &s = h.schopnosti[idx];

                if (h.mana < s.manaCena) {
                    cout << "Nemas dost many.\n";
                } else {
                    h.mana -= s.manaCena;

                    // utok na vsechny
                    if (s.dmgVsem > 0) {
                        cout << "Pouzivas " << s.nazev << " na vsechny nepratele.\n";
                        for (auto &e : nepratele)
                            if (monstrumZije(e))
                                e.hp -= s.dmgVsem;
                    }
                    // single target
                    else if (s.dmgSingle > 0) {
                        int cil;
                        cout << "Cislo cile: ";
                        cin >> cil;

                        if (cil < 1 || cil > (int)nepratele.size() || !monstrumZije(nepratele[cil-1])) {
                            cout << "Spatny cil.\n";
                        } else {
                            cout << "Pouzivas " << s.nazev << " na "
                                 << nepratele[cil-1].nazev
                                 << " za " << s.dmgSingle << " dmg.\n";
                            nepratele[cil-1].hp -= s.dmgSingle;
                        }
                    }

                    // leceni
                    if (s.leceni > 0) {
                        cout << "Lecis se o " << s.leceni << " HP.\n";
                        h.hp += s.leceni;
                        if (h.hp > h.maxHP) h.hp = h.maxHP;
                    }
                }
            }
        }


        if (!nepratelePrvni) {
            for (auto &e : nepratele) {
                if (!monstrumZije(e)) continue;
                cout << e.nazev << " utoci za " << e.utok << " dmg.\n";
                h.hp -= e.utok;
                if (h.hp <= 0) {
                    cout << "Byl jsi zabit.\n";
                    return false;
                }
            }
        }
    }

    return false;
}
// -------------------- SOUBOJ: BLINKRON --------------------
bool soubojBlinkron(Hrac &h, Blinkron &b) {
    cout << "\n=== SOUBOJ S HLAVNIM BOSSEM: BLINKRON ===\n";

    while (h.hp > 0 && b.zaklad.hp > 0) {

        b.kolo++;
        b.neviditelny = (nahodneCislo(0, 1) == 0);

        bool teleportKolo = (b.kolo % 3 == 0);

        // utok Blinkrot
        if (teleportKolo) {
            cout << "Blinkron mizi v zablesku svetla!\n";
            int dmg = b.zaklad.utok * 2;
            cout << "Teleport za tebe, nelze zablokovat, dmg: " << dmg << "\n";
            h.hp -= dmg;

            if (h.hp <= 0) {
                cout << "Byl jsi zabit.\n";
                return false;
            }
        }
        else {
            int dmg = (int)(b.zaklad.utok * b.bonusDalsiUtok);
            cout << "Blinkron utoci za " << dmg << " dmg.\n";
            h.hp -= dmg;
            b.bonusDalsiUtok = 1.0f;

            if (h.hp <= 0) {
                cout << "Byl jsi zabit.\n";
                return false;
            }
        }

        // kolo hrace
        cout << "\nTvoje kolo.\n";
        vypisStatyHrace(h);
        cout << "Blinkron HP: " << b.zaklad.hp << "\n";

        cout << "1) Zakladni utok\n";
        for (size_t i = 0; i < h.schopnosti.size(); ++i) {
            cout << (i+2) << ") " << h.schopnosti[i].nazev
                 << " (mana " << h.schopnosti[i].manaCena << ")\n";
        }

        int volba;
        cin >> volba;

        int celkDmg = 0;

        if (volba == 1) {
            celkDmg = h.utok;
        }
        else {
            int idx = volba - 2;

            if (idx < 0 || idx >= (int)h.schopnosti.size()) {
                cout << "Neplatna schopnost, provadi se zakladni utok.\n";
                celkDmg = h.utok;
            }
            else {
                Schopnost &s = h.schopnosti[idx];

                if (h.mana < s.manaCena) {
                    cout << "Nemas dost many, zakladni utok.\n";
                    celkDmg = h.utok;
                }
                else {
                    h.mana -= s.manaCena;
                    celkDmg = (s.dmgSingle > 0 ? s.dmgSingle : h.utok);

                    if (s.leceni > 0) {
                        cout << "Lecis se o " << s.leceni << " HP.\n";
                        h.hp += s.leceni;
                        if (h.hp > h.maxHP) h.hp = h.maxHP;
                    }
                }
            }
        }

        // neviditelny
        if (b.neviditelny) {
            cout << "Blinkron se vyhnul utoku! Nabiji si dalsi utok.\n";
            b.bonusDalsiUtok = 1.5f;
        }
        else {
            cout << "Zasahujes Blinkrona za " << celkDmg << " dmg.\n";
            b.zaklad.hp -= celkDmg;
        }
    }

    if (h.hp > 0) {
        cout << "Porazil jsi Blinkrona! Vyhral jsi hru!\n";
        return true;
    }

    cout << "Prohral jsi.\n";
    return false;
}
// -------------------- VYBER HRACE --------------------
Hrac vyberTridu() {
    while (true) {
        cout << "Vyber si tridu:\n";
        cout << "1) Paladin\n";
        cout << "2) Lovec\n";
        cout << "3) Mag\n";

        int volba;
        cin >> volba;

        Hrac h;

        if (volba == 1) {
            h.trida = "Paladin";
            h.maxHP = 20; h.hp = 20;
            h.maxMana = 10; h.mana = 10;
            h.zlato = 10;
            h.level = 1;
            h.xp = 0;
            h.utok = 4;

            h.schopnosti = {
                    {"Uder svetlem", "1 dmg vsem", 2, 0, 3, 0},
                    {"Oziveni", "Heal 3 HP", 2, 0, 0, 3}
            };
        }
        else if (volba == 2) {
            h.trida = "Lovec";
            h.maxHP = 16; h.hp = 16;
            h.maxMana = 8; h.mana = 8;
            h.zlato = 12;
            h.level = 1;
            h.xp = 0;
            h.utok = 5;

            h.schopnosti = {
                    {"Presna rana", "Silny utok", 2, 9, 0, 0},
                    {"Salva", "2 dmg vsem", 3, 0, 2, 0}
            };
        }
        else if (volba == 3) {
            h.trida = "Mag";
            h.maxHP = 14; h.hp = 14;
            h.maxMana = 14; h.mana = 14;
            h.zlato = 8;
            h.level = 1;
            h.xp = 0;
            h.utok = 3;

            h.schopnosti = {
                    {"Ohniva koule", "Silny single dmg", 3, 10, 0, 0},
                    {"Bleskova boure", "3 dmg vsem", 5, 0, 3, 0}
            };
        }
        else {
            cout << "Neplatna volba.\n";
            continue;
        }

        cout << "\nNahlad tridy:\n";
        vypisStatyHrace(h);

        cout << "Schopnosti:\n";
        for (auto &s : h.schopnosti) {
            cout << "- " << s.nazev << " (" << s.popis
                 << ", mana " << s.manaCena << ")\n";
        }

        cout << "Potvrdit? (1 = ano, 0 = ne): ";
        int ok;
        cin >> ok;

        if (ok == 1) return h;
    }
}
// -------------------- MAIN --------------------
int main() {
    srand((unsigned)time(nullptr));
    cout << "Textove RPG - Blinkron\n\n";

    Hrac hrac = vyberTridu();

    // priprava monster
    Monstrum goblin = vytvorMonstrum("Goblin", 8, 3, 3, 2, 5);
    Monstrum ork = vytvorMonstrum("Ork", 12, 4, 4, 3, 7);
    Monstrum vlk = vytvorMonstrum("Vlk", 10, 3, 3, 2, 6);
    Monstrum troll = vytvorMonstrum("Troll", 18, 5, 6, 5, 10);

    Monstrum mini1 = vytvorMonstrum("MiniBoss - Temny rytir", 25, 6, 10, 10, 20, true, true, false);
    Monstrum mini2 = vytvorMonstrum("MiniBoss - Ohnivy obr", 28, 7, 12, 12, 22, true, true, false);

    Blinkron blinkron = vytvorBlinkron();


    // 1: V
    vesnice(hrac);
    if (hrac.hp <= 0) return 0;

    // 2: M
    {
        vector<Monstrum> e = {goblin};
        if (!souboj(hrac, e, false)) return 0;
    }

    // 3: M
    {
        vector<Monstrum> e = {ork};
        if (!souboj(hrac, e, false)) return 0;
    }

    // 4: 2xM
    {
        vector<Monstrum> e = {goblin, vlk};
        if (!souboj(hrac, e, false)) return 0;
    }

    // 5: MB
    {
        vector<Monstrum> e = {mini1};
        if (!souboj(hrac, e, true)) return 0;
    }

    // 6: V
    vesnice(hrac);
    if (hrac.hp <= 0) return 0;

    // 7: M
    {
        vector<Monstrum> e = {troll};
        if (!souboj(hrac, e, false)) return 0;
    }

    // 8: 2xM
    {
        vector<Monstrum> e = {ork, vlk};
        if (!souboj(hrac, e, false)) return 0;
    }

    // 9: 2xM
    {
        vector<Monstrum> e = {goblin, goblin};
        if (!souboj(hrac, e, false)) return 0;
    }

    // 10: MB
    {
        vector<Monstrum> e = {mini2};
        if (!souboj(hrac, e, true)) return 0;
    }

    // 11: V
    vesnice(hrac);
    if (hrac.hp <= 0) return 0;

    // 12: 2xM
    {
        vector<Monstrum> e = {troll, vlk};
        if (!souboj(hrac, e, false)) return 0;
    }

    // 13: 2xM
    {
        vector<Monstrum> e = {ork, ork};
        if (!souboj(hrac, e, false)) return 0;
    }

    // 14: 3xM
    {
        vector<Monstrum> e = {goblin, vlk, ork};
        if (!souboj(hrac, e, false)) return 0;
    }

    // 15: V
    vesnice(hrac);
    if (hrac.hp <= 0) return 0;

    // 16: HB (Blinkron)
    if (!soubojBlinkron(hrac, blinkron)) return 0;

    cout << "\nDiky za hrani.\n";
    return 0;
}
