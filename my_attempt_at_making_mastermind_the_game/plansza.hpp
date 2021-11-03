#ifndef plansza_hpp
#define plansza_hpp
/////////Reszta kodu
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<cstdio>
#include<cstdbool>
#include<string.h>
#include "poziom.hpp"

using namespace std;

class plansza : public poziom {
protected:
	poziom* glowa, * wsk; ////Wskazniki do listy poziomow, ktore tworza dana plansze
public:
	////Destruktor i konstruktor
	~plansza(); ////Domyslny
	plansza(); ////Konstruktor domyslny
	plansza(int); ////Konstruktor z argumentem, ktory przekazuje ten argument do konstrukotra obiektu poziom;
	////Setter dla poziomu trudnosci
	void ustaw_ilosc_pol(int);
	//Getter dla poziomu trudnosci
	int pobierz_ilosc_pol(void);
	////Setter dla referencji
	void ustaw_referencje(void);
	void ustaw_referencje(int* tablica);
	////Getter dla referencji
	int* pobierz_referencje(void);
	////Getter dla glowy(poczatku listy)
	poziom* pobierz_glowe(void); ////Pobiera poczatek utworzonej listy poziomow rozgrywki
	////Definicja metod
	void wypisz_referencje(void); ////Wypisuje w schludnym stylu referencje
	void zeruj_liste(void); ////przyrownuje wskaznik wsk do glowy, czyli cofa liste do poczatku, w razie potrzeby
	void stworz_liste_poziomow(void); ////Tworzy liste 9 poziomow
	void wypisz_liste_poziomow(void); ////Wypisuje cala liste poziomow, dajmy na to po zakonczeniu rozgrywki
};
#endif
