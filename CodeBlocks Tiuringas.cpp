#include <iostream> // Isvedimo komandos
#include <fstream> // Skaitymas is failo
#include <iomanip> // Setw komandai
#include <windows.h> // Klaviaturos nuskaitymui
#include <string> // String naudojimui
#include <mutex> // Graziam spauzdinimui
#include <thread> // Treadams
std::mutex mtx;
using namespace std;

void MENU(int i);
int klausimai();


class turing {
private:
    string B[100]; // Pradine busena
    char K[100]; // Pradinis Kintamasis
    char P[100]; // Pakeistas Kintamasis
    char J[100]; // Judejimo Kryptis
    string N[100]; // Nauja busena
    string eil; // Eilute
    int kiek; // Instrukciju eiluciu kiekis
    int kuris;
    string busena; // Dabartine busena
public:
    void skaitymas(std::string file, int y)
    {
        bool baigta1 = 0;
        ifstream F((file + ".txt")); // Failo pavadinimas + galune
        F >> kuris; // Galvutes pozicija juostoje
        kuris--; // Ciklai vyksta nuo 0 todel minusuojame viena.
        F >> eil; // Nuskaitoma Juostos Eilute
        kiek = 0;
        while (!F.eof()) // Skaitoma tol kol nebera ka skaityti.
        {
            F >> B[kiek];
            F >> K[kiek];
            F >> P[kiek];
            F >> J[kiek];
            F >> N[kiek];
            kiek++; // Sumuojama kiek instrukciju eiluciu yra
        }
        F.close(); // Uzdaromas duomenu failas
        kiek--; // Atimama vienas , nes buvo pradeta nuo 0
        busena = "0"; // Nurodyta busena yra 0
        while (baigta1 != 1) // Tol kol lygybe nera tiesa daromas judejimas.
        {
            if (!baigta1) judejimas(baigta1,y);
            else
            { // Spauzdinama paskutini karta irasyta eilute.
                last(eil);
                cout << eil << endl;
                cout << endl;
            }
            Sleep(10);
        }
    }
    void judejimas(bool & baigta, int y) // Paduodama lygybes reiksme ir Y kordinate.
    {
        COORD cursorPosition;    cursorPosition.X = 0;    cursorPosition.Y = y; // Apsirasoma kordinaciu pozicijas.
        baigta = false;
            for (int i = 0; i < kiek; i++) // Einama per instrukcijos eilutes
            {
                if (GetAsyncKeyState(VK_DELETE)){baigta = true; } // Isjungimo funkcija su DELETE.
                    if (i == kiek - 1 && (B[i] != busena || K[i] != eil[kuris])) baigta = true; // Jeigu pradine busena nera lygi naujai busenai arba pradinis kintamasis lygus eilutes elementui tuomet simulaciaja baigiama.
                    if (B[i] == busena && K[i] == eil[kuris]) // Jeigu pradine busena lygi dabartiniai busenai ir pradinis kintamasis lygus eilutes elementui tuomet galima judeti
                    {
                        eil[kuris] = P[i]; // Pakeiciamas eilutes kintamasis i duota.
                        if (J[i] == 'R') kuris++; // Jeigu kryptis R judama i desne
                        else kuris--; // Kitaip judama i kaire
                        if (kuris < 0) baigta = true; // Jeigu galvutes pozicija juostoja mazesne uz 0 , simuliacija baigiama                 *** RIBOS ****
                        if (kuris > eil.length()) baigta = true; // Jeigu galvute iseina uz ribos, simuliacija baigiama
                        busena = N[i]; // Sena busena keiciama i nauja busena


                        mtx.lock(); // Mutex duomenu spausdinimo uzrakinimas
                        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition); // Spauzdinimo vietos nustatymas
                        cursorPosition.X = 0;    cursorPosition.Y = y;
                        cout << eil << endl; // Spauzdinima eilute
                        cout << "                                                                                                                 "; // Istrinamas zenkliukas
                        cursorPosition.X = 0;    cursorPosition.Y = y+1;
                        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
                        cout << setw(kuris) << "^"; // Spauzdinamas zenkliukas
                        mtx.unlock(); // Spauzdinimo atrakinimas
                        i = kiek;
                    }
        }
    }

    void last(string& eilut)
    {
        eilut = eil;
    }
};



int main()
{
    char p;
    turing one; // Klases apsirasymas
    bool baigta1 = 0;
    string eil;
    int ats;
    int i = 0;
    while (i != 4) // Meniu infinity loop funkcija
    {
        bool baigta1 = 0; // Kiekviena karta nustatoma jog programa, dar nera pabaigta.
        i = klausimai();
        if (i == 1)
        {
            string file = "1";
            system("cls");
            one.skaitymas(file,2); // Kreipimasis i klases funkcija
            cout << "Ar norite testi darba? : Y/N  ";
            cin >> p;
            if (p == 'y' || p == 'Y') {}
            else { return 0; }
            system("cls");
        }
        if (i == 2)
        {
            system("cls");
            turing* taskPtr = new turing(); // Susikuriama operatoriai kurie bus panaudoti threadams, kadangi kreipiamasi i klases funkcija.
            turing* taskPtr1 = new turing();
            turing* taskPtr2 = new turing();
            turing* taskPtr3 = new turing();
            std::thread t1(&turing::skaitymas, taskPtr, "1",0); // Treadas nukreipiamas i classes funkcija skaitymas su duomenimis 1 ir 0

            std::thread t2(&turing::skaitymas, taskPtr1, "2",7);

            std::thread t3(&turing::skaitymas, taskPtr2, "3",14);

            std::thread t4(&turing::skaitymas, taskPtr3, "4",21);
            t1.join(); // Padaro jog uzsibaigus treadui , jis nieko nereturnintu , o leistu kitoms programos pabaigti darba.
            t2.join();
            t3.join();
            t4.join();
            COORD cursorPosition;    cursorPosition.X = 0;    cursorPosition.Y = 24;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
            cout << "Programos ivygdytos!" << endl;
            cout << "Ar norite testi darba? : Y/N  ";
            cin >> p;
            if (p == 'y' || p == 'Y') {}
            else { return 0; }
            system("cls");


        }
        if (i == 3)
        {
            cout << "Iveskite failo vardo pavadinima: ";
            string file;
            cin >> file;
            system("cls");
            one.skaitymas(file,2);
            cout << "Ar norite testi darba? : Y/N  ";
            cin >> p;
            if (p == 'y' || p=='Y') {}
            else { return 0; }
            system("cls");
        }
        if (i == 4) { cout << "EXITING!"; Sleep(500); }

    }
    Sleep(10);
    return 0;
}

void MENU(int i)
{
    COORD cursorPosition;    cursorPosition.X = 0;    cursorPosition.Y = 0;    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "============================================  " << endl;
    cout << "   MENIU! Pasirinkite ka norite daryti!" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);                   // Spalvu kodai
    cout << " Norint sustabdyti masina paspauskite DEL!" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "============================================  " << endl;
    if (i == 1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 46); // SPALVA PARYSKINTA
    else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // NEPARYSKINTA
    cout << "#1 Paleiskite pirma tiuringo programa!" << endl;
    if (i == 2) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 46);
    else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "#2 Paleiskite 4 tiuringo masinas vienu metu!" << endl;
    if (i == 3) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 46);
    else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "#3 Pasirinkite failo pavadinima ir paleiskite tiuringo masina!" << endl;
    if (i == 4) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 46);
    else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "#4 EXIT!" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "============================================  " << endl;
}
int klausimai()
{
    int i = 0;
    bool log = true;
    MENU(i); // Spauzdina meniu teksta
    while (log)
    {
        if (GetAsyncKeyState(VK_RETURN)) log = false; // Paspaudus enter , nustoja fiksuoti klavisus ir grazina paskutine I reiksme.
        if (GetAsyncKeyState(VK_DOWN))
        {
            if (i < 4) i++; // Neleidzia pereit ribu
            cout << i;
            system("CLS");
            MENU(i);
        }
        if (GetAsyncKeyState(VK_UP))
        {
            if (i > 1) i--; // Neleidzia pereit ribu
            cout << i;
            system("CLS");
            MENU(i);
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        Sleep(100);
        //system("CLS");
    }
    return i;
}
