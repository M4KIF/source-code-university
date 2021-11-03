#ifndef poziom_hpp
#define poziom_hpp
////Za³¹czenie plików i bibliotek
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<cstdio>
#include<cstdbool>
#include<string.h>

using namespace std;

class poziom {
protected:
	int* pola; ////Wskaznik do tablicy dynamicznej z polami w pojedynczej probie
	int ile_pol; ////Liczba pol, czyli poziom trudnosci wybrany przez uzytkownika
	int ile_trafionych_pol; ////Ile trafien po sprawdzeniu
	int ile_trafionych_kolorow; ////Ile poprawnie wybranych kolorow, ale zle przyporzadkowanych
public:
	////Destruktor obiektu i dwa konstruktory
	poziom(); ////Konstruktor standardowy
	poziom(int); ////Konstruktor z argumentem, ktory od razu ustawia ilosc pol
	~poziom(); ////Usuwa tablice i nastepny obiekt
	////Setter
	void ustaw_ilosc_pol(int);
	////Getter
	int pobierz_ilosc_pol(void);
	////Metody
	void stworz_tablice_pol(void); ////Deklaruje dynamiczna tablice elementow dla wybranej ilosci pol(4||5)
	int* pobierz_tablice(void); ////Pobiera adres do 1 elementu tablicy  
	void ustaw_wartosc_pol(int, int, int, int); ////Ustawia wartosc pol dla 4 arg
	void ustaw_wartosc_pol(int, int, int, int, int); ////Ustawia wartosc pol dla 5 arg(funkcja przeciazana)
	void wypisz_wartosc_pol(void); ////Wypisuje wartosc odpowiednich pol(cala tablice "pola")
	bool porownaj_probe_z_referencja(int* tablica); ////Metoda sprawdzajaca obecna tablice z referencja, pobieraadres tablicy
	void wypisz_rezultat_porownania(void); ////Wypisuje rezultat metody powyzszej
	////Wskaznik do nastepnego elementu ewentualnej listy
	poziom* nast;
};
#endif

