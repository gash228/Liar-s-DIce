#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>


using namespace std;

struct Kocka {
    int vred;
    Kocka* nasl;
};

class Igralec {
    Kocka* zacetek;
public:
    Kocka* beri;
    string ime;


    Igralec() {                                  //Kazalčni seznam kock za vsakega igralca
        zacetek = NULL;
        for (int i = 0; i < 5; i++) {
            Kocka* novi = new Kocka;
            novi->vred = rand() % 6 + 1;
            novi->nasl = zacetek;
            zacetek = novi;
        }
        beri = zacetek;
        cout << endl;
    }

    void Izpisi();                              //Prototip
    int koliko(int num, Kocka* beri);            //Prototip
    void odstrani();
    void annihilation();
    int seznam_prazen();
    void nove_kocke();
    int prepis();

};

void Igralec::Izpisi() {                     //Izpisi kocke v seznamu
    Kocka* kaz;
    for (kaz = zacetek; kaz != NULL; kaz = kaz->nasl) cout << '\t' << kaz->vred << endl;
    cout << endl;
};

int Igralec::koliko(int num, Kocka* beri) {   //Prešteje koliko kock določene vrednosti je v seznamu vsakega igralca
    if (beri != NULL) {
        if (num == beri->vred) return 1 + koliko(num, beri->nasl);
        else return koliko(num, beri->nasl);
    }
    else return 0;

};

void Igralec::annihilation() {
    Kocka* kaz = zacetek;
    while (kaz != NULL) {
        Kocka* next = kaz->nasl;
        delete kaz;
        kaz = next;
    }
    zacetek = NULL;
};


int Igralec::seznam_prazen() {
    if (zacetek == NULL) {
        return 0;
    }
    else {
        return 1;
    }
};


int Igralec::prepis() {
    if (zacetek == NULL) {
        return 1;
    }
    else {
        return 0;
    }
};


void Igralec::odstrani() {                       //Odstrani eno kocko iz seznama kock igralca
    Kocka* kaz, * kazd;
    for (kaz = zacetek; kaz->nasl->nasl != NULL; kaz = kaz->nasl);
    kazd = kaz->nasl;
    kaz->nasl = NULL;
    delete kazd;
};


void Igralec::nove_kocke() {         //Igralec dobi novi seznam kock
    for (beri = zacetek; beri != NULL; beri = beri->nasl)
        beri->vred = rand() % 6 + 1;
    beri = zacetek;
};

class Igra {
public:
    int k;
    Igralec* tab;    //Več igralcev v tabeli, kazalec na tabelo
    int vse_kocke;


    Igra() {
        cout << "Koliko igralcev bo? ";           //Koliko igralcev bo v igri
        cin >> k;                                 //k=količina igralcev
        tab = new Igralec[k];
        cin.ignore();
        for (int i = 0; i < k; i++) {
            cout << "Napisi ime igralca: ";
            getline(cin, tab[i].ime);
            fflush(stdin);
            vse_kocke = k * 5;
        }

    }



    void Izpisi_igralce() {                      //Izpiše kocke vseh igralcev v seznamu
        for (int i = 0; i < k; i++) {
            cout << "Igralec " << tab[i].ime << ": " << endl;
            tab[i].Izpisi();
        }
    }

    int koliko_na_mizi_test() {                  //Izračuna kocke vseh igralcev, za uporabo v mainu
        int x = 0;
        int stev, kol;
        cout << "Vnesite stevilko: ";
        cin >> stev;
        cout << "Koliko te stevilke je na mizi? ";
        cin >> kol;
        for (int i = 0; i < k; i++) {
            x += tab[i].koliko(stev, tab[i].beri);
        }
        return x;
    }

    int koliko_na_mizi(int num) {                //Izračuna kocke vse igralcev v tabeli
        int x = 0;
        for (int i = 0; i < koliko_igralcev_ostalo(); i++) {
            x += tab[i].koliko(num, tab[i].beri);
        }
        return x;
    }

    /*void vnos(int num, int kolicina, int i){         //Za vnos količine in vrednosti kock
        cout<<"Igralec "<<tab[i].ime<<", "<<"vnesite vrednost: "<<endl;
        cin>>num;
        cout<<"Igralec "<<tab[i].ime<<", "<<"Koliko te vrednosti je na mizi? "<<endl;
        cin>>kolicina;
    }*/

    void potek() {                               //Celoten potek igre
        int num = 0, numpr = 0, kolicina = 0, kolicinapr = 0;
        char input = 'a';
        int i = 0;
        while (input != 'y') {  //Dokler en od igralcev ne obtoži, drugega da laže

            cout << "Igralec " << tab[i].ime << ", " << "Koliko neke vrednosti je na mizi? " << endl;
            cin >> kolicina;
            cout << "Igralec " << tab[i].ime << ", " << "vnesite vrednost: " << endl;
            cin >> num;

            while (kolicina > vse_kocke || num > 6 || num < 1 || kolicina <= kolicinapr) {
                if (kolicina == kolicinapr && num > numpr && num <= 6) break;
                cout << "Neveljaven vnos. Ponovi." << endl;
                cout << "Igralec " << tab[i].ime << ", " << "Koliko neke vrednosti je na mizi? " << endl;
                cin >> kolicina;
                cout << "Igralec " << tab[i].ime << ", " << "vnesite vrednost: " << endl;
                cin >> num;

            };

            /*cout<<"Igralec "<<tab[i].ime<<", "<<"Koliko neke vrednosti je na mizi? "<<endl;
            cin>>kolicina;
            while(kolicina>vse_kocke){
                cout<<"Neveljaven vnos. Ponovi."<<endl;
                cout<<"Igralec "<<tab[i].ime<<", "<<"Koliko neke vrednosti je na mizi? "<<endl;
                cin>>kolicina;
                cout<<"Igralec "<<tab[i].ime<<", "<<"vnesite vrednost: "<<endl;
                cin>>num;
            };
            cout<<endl<<endl<<"Igralec "<<tab[i].ime<<", "<<"vnesite vrednost: "<<endl;
            cin>>num;
            while(num>6 || num<1){
                cout<<"Neveljaven vnos. Ponovi."<<endl;
                cout<<"Igralec "<<tab[i].ime<<", "<<"Koliko neke vrednosti je na mizi? "<<endl;
                cin>>kolicina;
                cout<<"Igralec "<<tab[i].ime<<", "<<"vnesite vrednost: "<<endl;
                cin>>num;
            }
            cout<<endl<<endl<<kolicinapr<<endl<<endl;
            while(kolicina<=kolicinapr){                         //Dela, če ni veljaven vnos
                if(kolicina==kolicinapr && num>numpr) break;
                cout<<"Neveljaven vnos. Ponovi."<<endl;
                cout<<"Igralec "<<tab[i].ime<<", "<<"Koliko neke vrednosti je na mizi? "<<endl;
                cin>>kolicina;
                cout<<"Igralec "<<tab[i].ime<<", "<<"vnesite vrednost: "<<endl;
                cin>>num;
            }*/


            i++;
            if (i == k) i = 0;
            if (tab[i].seznam_prazen() == 0) i++;


            kolicinapr = kolicina;
            numpr = num;

            cout << "Igralec " << tab[i].ime << ", ali vam prejsni igralec laze? [Y/N]" << endl;
            cin >> input;
            while (input != 'n' && input != 'y') cin >> input;
        }
        int pomozni;
        if (i == 0)
            pomozni = k - 1;
        else pomozni = i - 1;


        cout << endl << "Igralec " << tab[pomozni].ime << " je povedal, da je na mizi toliko (" << kolicina << ") " << num << endl;
        cout << "Na mizi pa je " << koliko_na_mizi(num) << endl;


        fstream podatki;
        podatki.open("podatki.txt", ios::out || ios::app);

        if (kolicina > koliko_na_mizi(num)) {
            cout << "Igralec " << tab[i].ime << ", Igralec " << tab[pomozni].ime << " je zlagal, zmagali ste." << endl;
            tab[pomozni].annihilation();

        }
        else if (kolicina <= koliko_na_mizi(num)) {
            cout << "Igralec " << tab[i].ime << ", Igralec " << tab[pomozni].ime << " je govoril resnico, zgubili ste." << endl;
            tab[i].annihilation();
        }
        vse_kocke -= 5;
        podatki.close();

    }


    ~Igra() {
        cout << endl << "Destruktor sem" << endl;
    }

    int koliko_igralcev_ostalo() {
        int konec = 0;
        for (int i = 0; i < k; i++) {
            konec += tab[i].seznam_prazen();
        }
        return konec;
    }

    void nove_kocke_za_vse() {
        cout << "Igralci dobijo nove kocke" << endl;
        for (int i = 0; i < k; i++)
            tab[i].nove_kocke();
    }


    void celotna_igra() {    // Potek celotne igre
        int ostalo_igralcev = koliko_igralcev_ostalo();
        while (ostalo_igralcev > 1) {
            cout << "Igralcev v igri: " << ostalo_igralcev << endl;
            tab[0].Izpisi();
            potek();
            izbrisi_prazne();
            Izpisi_igralce();
            nove_kocke_za_vse();
            ostalo_igralcev = koliko_igralcev_ostalo();
        }

        cout << "Igra se je koncala. Zmagal je Igralec: ";
        cout << tab[0].ime << endl;
    }

    void izbrisi_prazne() {
        int stari_koliko = k;
        Igralec* novi = new Igralec[k];

        int j = 0;
        for (int i = 0; i < stari_koliko; i++) {
            if (tab[i].seznam_prazen()) {
                novi[j++] = tab[i];
            }
        }

        k = j;
        delete[] tab;
        tab = novi;
    }
};


int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Liar's Dice", sf::Style::Default);

    sf::Event ev;

    while (window.isOpen()) {
        while (window.pollEvent(ev)) {
            switch (ev.type) {
            case sf::Event::Closed:
                window.close(); break;
            case sf::Event::KeyPressed:
                if (ev.key.code == sf::Keyboard::Escape)
                    window.close();
                if (ev.key.code == sf::Keyboard::G)
                    cout << "You pressed G" << endl;
                break;

            }
        }
        window.clear();
        window.display();


    }

    cout << "Hello World";
    srand(time(NULL));
    Igra a;
    a.celotna_igra();


    return 0;
}