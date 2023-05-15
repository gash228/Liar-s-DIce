#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <chrono>
#include <vector>
#include <thread>



using namespace std;
using namespace std::this_thread;
using namespace std::chrono;


namespace Menu {
    sf::RenderWindow window;
    sf::RenderWindow igra;
    sf::Text prikaz;
    sf::Font fon;
    string izpis;
    bool zacni_igro;

    void open_Menu() {
        window.create(sf::VideoMode(1920, 1080), "Liar's Dice", sf::Style::Default);
        izpis.resize(0);
        izpis += "Ce ste pripravljeni, pritisnite Enter";
        fon.loadFromFile("Aller.ttf");
        prikaz.setFont(fon);
        prikaz.setString(izpis);
        prikaz.setCharacterSize(96);
        prikaz.setFillColor(sf::Color::Red);
        prikaz.setPosition(128, window.getSize().y / 2);
        window.draw(prikaz);
        window.display();
        zacni_igro = 0;
    }

    void Zacni_igro() {
        while (window.isOpen()) {
            sf::Event eve;
            while (window.pollEvent(eve)) {
                switch (eve.type) {
                case sf::Event::Closed:
                    window.close(); break;
                case sf::Event::KeyPressed:
                    if (eve.key.code == sf::Keyboard::Escape)
                        window.close();
                    if (eve.key.code == sf::Keyboard::Enter) {
                        window.close();
                        zacni_igro = 1;
                    }
                    break;

                }
            }

        }
    }




}

struct Kocka {
    int vred;
    Kocka* nasl;
    sf::Sprite dice;
    sf::Texture texture;
    
};


static int vrstica_za_kocke=0;                          //Spremenljivka, da se kocke pojavijo v svoji vrsti
static int koliko_za_izracun = 0;                       //Količina igralcev v igri


using namespace Menu; 


class Igralec {
    Kocka* zacetek;
public:
    Kocka* beri;
    string ime;
    sf::Text player;
    string za_text; 
    sf::Font font;


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
        koliko_za_izracun++;
    }

    void Izpisi();                              //Prototip
    int koliko(int num, Kocka* beri);            //Prototip
    void odstrani();
    void annihilation();
    int seznam_prazen();
    void nove_kocke();
    int prepis();
    void Kocke_dislpay();
    

};


void Igralec::Izpisi() {                     //Izpisi kocke v seznamu
    Kocka* kaz;
    int pozicija = 0;
    for (kaz = zacetek; kaz != NULL; kaz = kaz->nasl) {
        cout << '\t' << kaz->vred << endl;
        

        if (kaz->vred == 1) kaz->texture.loadFromFile("perspective-dice-six-faces-one.png");
        if (kaz->vred == 2) kaz->texture.loadFromFile("perspective-dice-six-faces-two.png");
        if (kaz->vred == 3) kaz->texture.loadFromFile("perspective-dice-six-faces-three.png");
        if (kaz->vred == 4) kaz->texture.loadFromFile("perspective-dice-six-faces-four.png");
        if (kaz->vred == 5) kaz->texture.loadFromFile("perspective-dice-six-faces-five.png");
        if (kaz->vred == 6) kaz->texture.loadFromFile("perspective-dice-six-faces-six.png");
        
        
            kaz->dice.setTexture(kaz->texture);
            kaz->dice.setScale(sf::Vector2f(0.3, 0.3));
            kaz->dice.setPosition(igra.getSize().x / 5 + pozicija +64, igra.getSize().y / 13 + vrstica_za_kocke);

            pozicija+= 250;

    }
    za_text.resize(0);
    za_text += "Igralec ";
    za_text += ime;
    za_text += ": ";
    font.loadFromFile("Aller.ttf");
    player.setFont(font);
    player.setString(za_text);
    player.setCharacterSize(36);
    player.setFillColor(sf::Color::White);
    player.setPosition(128, igra.getSize().y / 13 + vrstica_za_kocke + 36);
    cout << endl;
    vrstica_za_kocke += 900/koliko_za_izracun;


};

void Igralec::Kocke_dislpay() {
    Kocka* kaz;
    for (kaz = zacetek; kaz != NULL; kaz = kaz->nasl) {
        igra.draw(kaz->dice);
    }
    igra.draw(player);
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










class Igra{
public:
    int k;
    Igralec* tab;    //Več igralcev v tabeli, kazalec na tabelo
    int vse_kocke;
    sf::Text *ekran;
    sf::Font pisava;
    string output;
    bool zadnji_rezultat;


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
        pisava.loadFromFile("Aller.ttf");

    }



    void Izpisi_igralce() {                      //Izpiše kocke vseh igralcev v seznamu
        for (int i = 0; i < k; i++) {
            cout << "Igralec " << tab[i].ime << ": " << endl;
            tab[i].Izpisi();
        }
        igra.clear();
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
        podatki.open("podatki.txt", ios::out|ios::app);


        if (kolicina > koliko_na_mizi(num)) {
            cout << "Igralec " << tab[i].ime << ", Igralec " << tab[pomozni].ime << " je zlagal, zmagali ste." << endl;
            tab[pomozni].annihilation();
            zadnji_rezultat = 1;
            podatki<< "Igralec " << tab[pomozni].ime << " je povedal, da je na mizi toliko (" << kolicina << ") " << num <<"Na mizi pa je bilo toliko "<<num <<" : " << koliko_na_mizi(num) << endl;
            podatki << "Igralec " << tab[i].ime << " je zmagal."<<endl<<endl;
        }
        else if (kolicina <= koliko_na_mizi(num)) {
            cout << "Igralec " << tab[i].ime << ", Igralec " << tab[pomozni].ime << " je govoril resnico, zgubili ste." << endl;
            tab[i].annihilation();
            if (pomozni == 1) {
                zadnji_rezultat = 0;
            }
            else zadnji_rezultat = 1;
            podatki << "Igralec " << tab[pomozni].ime << " je povedal, da je na mizi toliko (" << kolicina << ") " << num << "Na mizi pa je bilo toliko " << num << " : " << koliko_na_mizi(num) << endl;
            podatki << "Igralec " << tab[pomozni].ime << " je zmagal."<<endl<<endl;
        }
        vse_kocke -= 5;
        podatki.close();

    }

    void avtomatski_potek() {                               //Celoten potek igre
        int num = 0, numpr = 0, kolicina = 0, kolicinapr = 0;
        char input = 'a';
        int i = 0;
        while (input != 'y') {  //Dokler en od igralcev ne obtoži, drugega da laže
            


            if (i == 0) {
                cout << "Igralec " << tab[i].ime << ", " << "Koliko neke vrednosti je na mizi? " << endl;
                cin >> kolicina;
                cout << "Igralec " << tab[i].ime << ", " << "vnesite vrednost: " << endl;
                cin >> num;

                while (kolicina > vse_kocke || num > 6 || num < 1 || kolicina <= kolicinapr) {  //Vse pogoji, da se ne vnesejo neveljavna stevila
                    if (kolicina == kolicinapr && num > numpr && num <= 6) break;
                    cout << "Neveljaven vnos. Ponovi." << endl;
                    cout << "Igralec " << tab[i].ime << ", " << "Koliko neke vrednosti je na mizi? " << endl;
                    cin >> kolicina;
                    cout << "Igralec " << tab[i].ime << ", " << "vnesite vrednost: " << endl;
                    cin >> num;

                };
            }
            else {
                ekran = new sf::Text;
                output.resize(0);
                output += "Igralec ";
                output += tab[i].ime;
                output += " razmislja...";
                ekran->setString(output);
                ekran->setFont(pisava);
                ekran->setCharacterSize(48);
                ekran->setFillColor(sf::Color::White);
                ekran->setPosition(window.getSize().x / 2 - 200, window.getSize().y / 2);
                igra.clear();
                //tab[0].Izpisi();
                tab[0].Kocke_dislpay();
                igra.draw(*ekran);
                igra.display();
                sleep_for(seconds(4));
                delete ekran;
                igra.display();

                if (kolicinapr == vse_kocke) input = 'y';               
                int dolocilo;
                dolocilo = rand() % 20 + 1;
                if (dolocilo <= 18) kolicina = kolicinapr + 1;              //Določa za koliko poviša stav
                else kolicina = kolicinapr + 2;
                num = numpr;
                if (numpr == 0) num = rand() % 6 + 1;
                if (tab[i].koliko(numpr + 1, tab[i].beri) >= 3) {       //Ce ima kock z vrednostjo za 1 več od stava več ali enako 3, poviša vrednost
                    kolicina = kolicinapr;
                    num = numpr + 1;
                }
                cout << "Igralec " << tab[i].ime << " je rekel, da je na mizi " << kolicina << " " << num << endl;
            }



            i++;
            if (i == k) i = 0;
            if (tab[i].seznam_prazen() == 0) i++;


            kolicinapr = kolicina;
            numpr = num;
            int pomozni;
            if (i == 0)
                pomozni = k - 1;
            else pomozni = i - 1;

            if (i == 0) {
                cout << "Igralec " << tab[i].ime << ", ali vam prejsni igralec laze? [Y/N]" << endl;
                cin >> input;
                while (input != 'n' && input != 'y') cin >> input;
            }
            else {
                sleep_for(seconds(1));
                int izhod = 10+k;
                int dolocilo = rand()%5+4;
                if (dolocilo + kolicina > izhod) {             //Formula, da računalnik delno slučajno določa, če mu je prejšni igralec zlagal
                    cout << "Igralec " << tab[i].ime << " je rekel, da igralec " << tab[pomozni].ime <<" je zlagal"<< endl;
                    input = 'y';
                }
                else input = 'n';
            }
        }
        int pomozni;
        if (i == 0)
            pomozni = k - 1;
        else pomozni = i - 1;


        cout << endl << "Igralec " << tab[pomozni].ime << " je povedal, da je na mizi toliko (" << kolicina << ") " << num << endl;
        cout << "Na mizi pa je " << koliko_na_mizi(num) << endl;


        fstream podatki;
        podatki.open("podatki.txt", ios::out | ios::app);
        vrstica_za_kocke = 0;
        Izpisi_igralce();
        Kocke_display_za_vse();


        if (kolicina > koliko_na_mizi(num)) {
            cout << "Igralec " << tab[i].ime << ", Igralec " << tab[pomozni].ime << " je zlagal, zmagali ste." << endl;
            tab[pomozni].annihilation();
            zadnji_rezultat = 1;
            podatki << "Igralec " << tab[pomozni].ime << " je povedal, da je na mizi toliko (" << kolicina << ") " << num << "Na mizi pa je bilo toliko " << num << " : " << koliko_na_mizi(num) << endl;
            podatki << "Igralec " << tab[i].ime << "je zmagal." << endl << endl;
        }
        else if (kolicina <= koliko_na_mizi(num)) {
            cout << "Igralec " << tab[i].ime << ", Igralec " << tab[pomozni].ime << " je govoril resnico, zgubili ste." << endl;
            tab[i].annihilation();
            if (pomozni == 1) {
                zadnji_rezultat = 0;
            }
            else zadnji_rezultat = 1;
            podatki << "Igralec " << tab[pomozni].ime << " je povedal, da je na mizi toliko (" << kolicina << ") " << num << "Na mizi pa je bilo toliko " << num << " : " << koliko_na_mizi(num) << endl;
            podatki << "Igralec " << tab[pomozni].ime << "je zmagal." << endl << endl;
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
        k = konec;
        return konec;
    }

    void nove_kocke_za_vse() {
        cout << "Igralci dobijo nove kocke" << endl;
        for (int i = 0; i < k; i++)
            tab[i].nove_kocke();
        vrstica_za_kocke = 0;
    }


    void Kocke_display_za_vse() {
        for (int i = 0; i < k; i++) {
            tab[i].Kocke_dislpay();
        }

        igra.display();
    }


    void celotna_igra() {    // Potek celotne igre
        int ostalo_igralcev = koliko_igralcev_ostalo();
        while (ostalo_igralcev > 1) {
            cout << "Igralcev v igri: " << ostalo_igralcev << endl;
            igra.clear();
            tab[0].Izpisi();
            tab[0].Kocke_dislpay();
            igra.display();
            avtomatski_potek();
            igra.display();
            ekran = new sf::Text;
            output.resize(0);
            output += "Za nadaljevanje pritisni Enter ";
            ekran->setString(output);
            ekran->setFont(pisava);
            ekran->setCharacterSize(24);
            ekran->setFillColor(sf::Color::White);
            ekran->setPosition(igra.getSize().x -128, igra.getSize().y+128);
            igra.draw(*ekran);
            igra.display();
            while (igra.isOpen()) {
                bool p = 0;
                sf::Event breaking;
                while (igra.pollEvent(breaking)) {
                    if (breaking.type == sf::Event::KeyPressed) {
                        if (breaking.key.code == sf::Keyboard::Enter) { p++; break; }
                    }
                   
                }
                if(p==1) break;
            }
            delete ekran;
            izbrisi_prazne();
            nove_kocke_za_vse();
            ostalo_igralcev = koliko_igralcev_ostalo();
            igra.clear();
            


            
        }

        cout << "Igra se je koncala. Zmagal je Igralec: ";
        cout << tab[0].ime << endl;
        igra.close();
        sf::RenderWindow winner(sf::VideoMode(720, 400), "Liar's Dice", sf::Style::Default);
        sf::Text winning;
        string input;
        input.resize(0);
        if (zadnji_rezultat == 0) input += "Zgubil si...";
        else input += "Zmagal si!!!";
        winning.setString(input);
        winning.setFont(pisava);
        winning.setCharacterSize(64);
        winning.setFillColor(sf::Color::Green);
        if(zadnji_rezultat==0) winning.setFillColor(sf::Color::Red);
        winning.setPosition(128, winner.getSize().y/2);
        winner.clear();
        winner.draw(winning);
        winner.display();
        sleep_for(seconds(12));
        winner.close();
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

        vrstica_za_kocke = 0;
        koliko_za_izracun = koliko_igralcev_ostalo();
    }

   
};


int main()
{
    srand(time(NULL));
    open_Menu();
    Zacni_igro();

 
    if (zacni_igro == 1) {
        Igra a;
        int y = 1;
        igra.create(sf::VideoMode(1920, 1080), "Liar's Dice", sf::Style::Default);
        while(igra.isOpen()) {
            sf::Event ev;
            while (igra.pollEvent(ev)) {
                switch (ev.type) {
                case sf::Event::Closed:
                    igra.close(); break;
                case sf::Event::KeyPressed:
                    if (ev.key.code == sf::Keyboard::Escape)
                        igra.close();
                    if (ev.key.code == sf::Keyboard::G)
                        cout << "You pressed G" << endl;
                    break;

                }
            }
            if (y == 1) {
                a.celotna_igra();
                y--;
            }


        }
    }





        return 0;
}