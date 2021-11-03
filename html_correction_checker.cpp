#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdlib>
#include<iomanip>
#include<string>
#include<fstream>
#include<stdexcept>
#include<cstring>
#define DLUG_LINII 120 ////Stala do wielkosci tablic pomocniczych 
#define BLAD_PLIKU "blad_otwarcia_pliku" ////deklaracaja stalej z kodem bledu

/*Ten program polega na sprawdzeniu skladni tekstu zapisanego w pliku, uzywajacego notacji HTTP
Ma na celu zaimplementowanie catcha, wlasnych klas bledow i innych metod oblugi plikow. Autor: Jakub Koterba*/

using namespace std;

/////Deklaracja klas

class blad; /////Klasa reprezentujaca moj typ bledow
class blad_otwarcia_pliku; /////Dziedziczaca po bledzie klasa odpowiedzialna za bledy otwierania plikow
class blad_skladniowy; /////Dziedziczaca po bledzie klasa odpowiedzialna za bledy skladniowe
class tag; /////Klasa przechowujaca pojedynczego taga z linijki
class stos; /////Klasa przechowujaca stos ze wszystkimi tagami z pliku
class obluga_pliku; /////Klasa obslugujaca plik i zawierajaca metody do odczytu

/////Definicja klas

class blad: public exception { 
protected:
	string wiadomosc;

public:
	blad(): wiadomosc("*BLAD*") {}; ////Konstruktor domyslny
	explicit blad(const string& message) {
		wiadomosc = message;
	}
	virtual const char* what() const throw() /////Rzuca blad wyzej.
	{
		return wiadomosc.c_str();
	}
};

class blad_otwarcia_pliku : public blad {
public:
	blad_otwarcia_pliku() :blad() {}; ////Konstruktor domyslny
	explicit blad_otwarcia_pliku(const string& message) {
		wiadomosc = message;
	}
	virtual const char* what() const throw() /////Rzuca blad wyzej.
	{
		return wiadomosc.c_str();
	}
};

class blad_skladniowy : public blad {
public:
	explicit blad_skladniowy() : blad() {}; ////Konstruktor domyslny
	explicit blad_skladniowy(const string& message) {
		wiadomosc = message;
	}
	virtual const char* what() const throw() /////Rzuca blad wyzej.
	{
		return wiadomosc.c_str();
	}
};

class tag {
protected:
	string wyrazenie; //////Tag do potegowych
	friend class stos;
public:
	tag(int) :wyrazenie("\0"), nast(NULL) {}; ////Konstruktor w wypadku blednej deklaracji elementu
	tag() : nast(NULL) {}; ////Konstruktor domyslny
	~tag(); /////Destruktor
	tag(string tab) { wyrazenie = tab; nast = NULL; }; ////Konstruktor z argumentem
	void wypisz(void); ////Wypisuje
	string wartosc(void); ////zwraca tekst z pola wartosc
	tag(tag& kopia) : wyrazenie(kopia.wyrazenie), nast(kopia.nast) {}; ////Konstruktor kopiujacy
	tag& operator=(tag& kopia); 
	tag* nast;
};

tag::~tag() {
	nast = NULL;
}

void tag::wypisz(void) {
	cout << wyrazenie << endl;
}

string tag::wartosc(void) {
	return wyrazenie;
}

tag& tag::operator=(tag& kopia) {
	this->wyrazenie = kopia.wyrazenie;
	this->nast = kopia.nast;
	return *this;
}

class stos : public tag {
protected:
	tag* glowa, * wsk;
public:
	~stos(); ////Destruktor
	stos() :glowa(NULL), wsk(NULL) { glowa = NULL; wsk = NULL; }; ////Konstruktor domyslny
	void dodaj(string tab); /////Dodaje do stosu
	tag* zdejmij(void); /////Zdejmuje i zwraca adres zjdetego elementu
	tag* pobierz_leb(void); //////Pobiera glowe stosu
	int policz(void); //////Liczy elementy stosu
	void wypisz_tagi(void); //////Wypisuje tagi
};

stos::~stos() {
	cout << "Usuwam stos..." << endl;
	while (glowa) {
		tag* pom = glowa;
		glowa = glowa->nast;
		delete pom;
	}
	wsk = glowa = NULL;
}

void stos::dodaj(string tab) {
	if (glowa == NULL) {
		glowa = new tag(tab);
		wsk = glowa;
		wsk->nast = NULL;
	}
	else {
		wsk->nast = new tag(tab);
		wsk = wsk->nast;
		wsk->nast = NULL;
	}
}

tag* stos::zdejmij(void) {
	tag* pom = glowa;
	glowa = glowa->nast;
	return pom;
}

tag* stos::pobierz_leb(void) {
	return glowa;
}

int stos::policz(void) {
	wsk = glowa;
	int rozmiar = 0;
	while (wsk) {
		rozmiar++;
		wsk = wsk->nast;
	}
	wsk = glowa;
	return rozmiar;
}

void stos::wypisz_tagi(void) {
	wsk = glowa;
	while (wsk) {
		cout << wsk->wartosc() << endl;
		wsk = wsk->nast;
	}
}

class obsluga_pliku {
protected:
	string bania; //////Inaczej glowa, czyli tag otwierajacy
	stos tagi; ///////Stos zawierajacy wszystkie tagi, sluzy do sprawdzenia tagow
	ifstream pliczek; ////Obiekt posiadajacy w sobie otwarty plik
	string linijka_bufor; ////Nieuzyte zostanie
	void odczyt_linijki(int& nr_linii); ////Odczytuje i sprawdza jedna linijke
public:
	obsluga_pliku() : linijka_bufor("\0") {}; ////Konsturktor domyslny
	obsluga_pliku(string& nazwa_pom); ////Konstyuktor z arguementem
	~obsluga_pliku(); ////Destruktor
	void sprawdzenie_pliku(); ////Odczytuje i sprawdza caly plik przy uzyciu funkcji wyzej
	void sprawdz_flagi_wyr_pot(void);
};

////Funkcja z wyjatkiem wewnatrz, ktory przerzuca w koncowej fazie sterowanie do maina
obsluga_pliku::obsluga_pliku(string& nazwa_pom) { 
	try {
		char tab[DLUG_LINII]; ////Nazwa pliku
		strcpy(tab, nazwa_pom.c_str());
		cout << "Otwieram plik...\n" << endl;
		pliczek.open(tab, ios::in);
		if (!pliczek.is_open()) throw blad_otwarcia_pliku("*BLAD_PLIK* Nieudana proba otwarcia pliku\n");
	}
	catch (blad_otwarcia_pliku &e) {
		cout << e.what() << endl;
		pliczek.close();
		throw blad_otwarcia_pliku("\n*BLAD_PLIK* Sprawdz, czy dobrze wpisales nazwe pliku, albo czy w ogole istnieje");
	}
}
obsluga_pliku::~obsluga_pliku() {
	cout << "\nZamykam plik..." << endl;
	pliczek.close();
}

////Funkcja wyrzucajaca wyjatek wewnatrz, a potem przerzucajaca do maina
void obsluga_pliku::odczyt_linijki(int& nr_linii) {
	try {
		char pom[DLUG_LINII]; ////Pomocnicza do wczytywania linii_char
		string pomocnicza; ////Pomocnicza do wczytywania linii_string
		pliczek >> ws;
		pliczek.getline(pom, DLUG_LINII, '\n');
		pomocnicza = pom; //// KOnwersja z chara na stringa, latwiej obrobic
		int limit_gorny_petli = pomocnicza.length(); ////Dlugosc stringa obrabianego obecnie
		pomocnicza.begin();
		string tag_start, tag_end; ////Przechowuja tagi xml jak mo, mi, msup itp.
		int i = 0; ////Licznik do obslugi
		if (pomocnicza[i] == '<') {
			i++;
			for (; i < limit_gorny_petli; i++) { ////Petla glowna, prowadzaca do konca linii
				if ((pomocnicza[i] == '>' || pomocnicza[i] == '<') && i != limit_gorny_petli - 1) {
					if (pomocnicza[i] == '>' && i != limit_gorny_petli - 1) {
						for (; i < limit_gorny_petli; i++) { ////Petla sprawdzajaca, czy w sytuacji linii o dwoch tagach jest to poprawny zapis.
							if (i == limit_gorny_petli - 1) {
								if (pomocnicza[i] != '>') { ////Wyjatek
									cout << "BLAD\n";
									string blad = "*BLAD_SKLADNIOWY* Blad w notacji w linii nr ";
									blad += to_string(nr_linii);
									const char* blad_char = blad.c_str();
									throw blad_skladniowy(blad_char);
								}
							}
						}
					}
					if (pomocnicza[i] == '<' && i != limit_gorny_petli - 1) {
						for (; i < limit_gorny_petli; i++) {////Petla sprawdzajaca, czy w sytuacji linii o dwoch tagach jest to poprawny zapis.
							if (i == limit_gorny_petli - 1) {
								if (pomocnicza[i] != '>') { ////Wyjatek
									cout << "BLAD\n";
									string blad = "*BLAD_SKLADNIOWY* Blad w notacji w linii nr ";
									blad += to_string(nr_linii);
									const char* blad_char = blad.c_str();
									throw blad_skladniowy(blad_char);
								}
							}
						}
					}
				}
				if (i == limit_gorny_petli - 1) {  ////Ogolny warunek konczacy
					if (pomocnicza[i] != '>') { ////Wyjatek
						cout << "BLAD\n";
						string blad = "*BLAD_SKLADNIOWY* Blad w notacji w linii nr ";
						blad += to_string(nr_linii);
						const char* blad_char = blad.c_str();
						throw blad_skladniowy(blad_char);
					}
				}
			}
		}
		pomocnicza.begin();
		int j = 0;
		string wnetrze; //////////Sprawdzanie co jest pomiedzy tagami 
		if (pomocnicza[j] == '<') {
			while (pomocnicza[j] != '>' && j < limit_gorny_petli - 1) {
				if (pomocnicza[j] != '<') tag_start += pomocnicza[j];
				j++;
			}
			while (pomocnicza[j] != '\n' && pomocnicza[j] != '<' && j < limit_gorny_petli - 1) {
				j++;
				if (pomocnicza[j] != '<') wnetrze += pomocnicza[j];
			}
			j++;
			while (pomocnicza[j] != '\n' && pomocnicza[j] != '>' && j < limit_gorny_petli - 1) {
				tag_end += pomocnicza[j];
				j++;
			}
		}
		if (tag_start == "TABLE") {
			tagi.dodaj(tag_start);
			tag* pomoc = tagi.pobierz_leb();
			bania = pomoc->wartosc();
		}
		else if (bania == "TABLE" && tag_start != "/TABLE") {
			tagi.dodaj(tag_start);
			if (isprint(tag_end[0])) {
				tagi.dodaj(tag_end);
			}
		}
		else if (tag_start == "/TABLE") {
			bania.clear();
			tagi.dodaj(tag_start);
			tagi.zdejmij();
		}
		else {
			cout << " BLAD" << endl;
			throw blad_skladniowy("*BLAD_SKLADNIOWY* W pierwszej linii");
		}
		const char* pom1 = wnetrze.c_str(); ///// Konwersja ze stringa na chara, czeby bylo latwiej sprawdzac 
		if (tag_end.length() > 1) {
			string pom = "/";
			pom += tag_start;
			if (!(tag_end == pom)) { ////Wyjatek
				string blad = "*BLAD_SKLADNIOWY* Blad w notacji w linii nr ";
				blad += to_string(nr_linii);
				const char* blad_char = blad.c_str();
				cout << " BLAD" << endl;
				throw blad_skladniowy(blad_char);
			}
			else cout << " OK" << endl;
		}
		else if (nr_linii == 1) {
			cout << " OK" << endl;
		}
		else if (nr_linii != 1 && (tag_start == "TABLE" || tag_start == "/TABLE" || tag_start == "TR" || tag_start == "/TR")) {
			cout << " OK" << endl;
		}
		else {
			string blad = "*BLAD_SKLADNIOWY* Blad w notacji w linii nr ";
			blad += to_string(nr_linii);
			const char* blad_char = blad.c_str();
			throw blad_skladniowy(blad_char);
		}
	}
	catch (blad& e) {
		cout << "\n\n" << e.what() << endl; ////Tutaj wypisuje komunikat bledu z numerem linii
		pliczek.close();
		throw blad_skladniowy("\n*BLAD_SKLADNIOWY* Musisz poprawic skladnie w pliku");
	}
}

/////Rzuca wyjatek wewnetrzny 
void obsluga_pliku::sprawdz_flagi_wyr_pot(void) {
	try {
		int koniec = 0;
		int ile_zgod = 0;
		int ile = tagi.policz();
		tag* tab = new tag[ile];
		for (int i = 0; i < ile; i++) {
			tag* pom = tagi.zdejmij();
			tab[i] = *pom;
			delete pom;
		}
		for (int i = 0; i < ile; i++) {
			string pom = "/";
			pom += tab[i].wartosc();
			for (int j = i; j < ile; j++) {
				if (tab[j].wartosc() == pom) {
					ile_zgod++;
					break;
				}
			}
			if (tab[i].wartosc() == "/TABLE") koniec = 1;
		}
		if (koniec == 1 || ile_zgod == (ile / 2)) {
			cout << "\nWszystkie tagi sie zgadzaja - OK\n";
		}
		else throw blad_skladniowy("Blad w skladnii tagow");
	}
	catch (blad& e) {
		cout << e.what();
		throw blad_skladniowy("\n*BLAD_SKLADNIOWY* Musisz poprawic skladnie w pliku");
	}
}

void obsluga_pliku::sprawdzenie_pliku() {
	int ile_linijek = 1;
	cout << "\nSprawdzam teraz poprawnosc ogolna skladni...\n" << endl;
	while (!pliczek.eof() && pliczek.good()) {
		cout << "Linijka" << setw(3) << right << ile_linijek << " -";
		odczyt_linijki(ile_linijek);
		ile_linijek++;
	}
	sprawdz_flagi_wyr_pot();
}

/////Metody inne

void wstep(void) {
	cout << "\n\n/*Ten program polega na sprawdzeniu skladni tekstu zapisanego w pliku, uzywajacego notacji HTTP\n";
	cout << "Ma na celu zaimplementowanie catcha, wlasnych klas bledow i innych metod oblugi plikow. Autor: Jakub Koterba/*\n" << endl;
}

int koniec(void) {
	cout << "\nKoniec programu..." << endl;
	return 0;
}

/////Funkcja main

int main(void) {
	////Tu bedzie wstep i informacja o przeznaczeniu programu
	wstep();
	////Testowanie otwierania pliku z RAII
	string nazwa = "http.txt";
	try {
		obsluga_pliku http(nazwa); /////Deklaracja nowego obiektu z plikiem w srodku.
		http.sprawdzenie_pliku(); /////Glowne polecenie zadania
	}
	catch (blad_otwarcia_pliku& e) {
		cout << e.what();
		cout << "\nTyp to: " << typeid(e).name() << endl;
	}
	catch (blad_skladniowy& e) {
		cout << e.what();
		cout << "\nTyp to: " << typeid(e).name() << endl;
	}
	////Koniec programu
	koniec();
}