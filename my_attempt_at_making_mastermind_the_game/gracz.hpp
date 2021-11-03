#ifndef gracz_hpp
#define gracz_hpp
////Za³¹czenie plików i bibliotek
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<cstdio>
#include<cstdbool>
#include<string.h>
#include"plansza.hpp"

using namespace std;

class gracz : public plansza {
protected:
	plansza* osoba; ////Wskaznik do gracza
	int ile_zapisow; ////Liczy ile razy zapisano do pliku
	FILE* save; ////Wskaznik do pliku z zapisem
public:
	////Konstruktor i destruktor
	gracz();
	~gracz();
	//Settery
	////Deklaracje metod
	virtual void dodaj_gracza(); ////Dodaje obiekt typu plansza do wskaznika w obiekcie gracz
	virtual plansza* pobierz_adres_gracza(void); ////Pobiera adres do planszy wybranego gracza
	virtual void otworz_plik_save_w(char* tab); ////Otwiera plik z zapisem w trybie write(tworzy nowy) 
	virtual void otworz_plik_save(char* tab); ////otwiera w trybie addycji i odczytu
	virtual FILE* pobierz_adres_pliku(void); ////Pobiera adres pliku
	virtual int pobierz_ilosc_zapisow(void); ////Pobiera ilosc zapisow
	virtual void zapisz_runde(int, int, int, int, int, int, int); ////Ile pol(tryb gry), numer zapisu, zeby wiedziec ile prob zapisalo, cala liczba w ciagu
	virtual void zapisz_runde(int, int, int, int, int, int, int, int); ////Ile pol(tryb gry), numer zapisu, zeby wiedziec ile prob zapisalo, cala liczba w ciagu
	virtual poziom* wczytaj_save(void); ////Wczytuje save uzupelnia poziomy itd.
	virtual void zamknij_plik_save(void); ////Zamyka plik z savem
	virtual int pobierz_odpowiedz(void); ////Pobiera od uzytkownika odpowiedz
	virtual bool sprawdz_dlugosc(int odp); ////Sprawdza poprawnosc wprowadzonych danych
	virtual void wybor_poziomu_trudnosci(void); ////Wczytuje dane odpowiadajace poziomowi trudnosci
	virtual int wybor_trybu_gry(void); ////Wczytuje dane dla trybu gry
	virtual int wybor_wczytania(void); ////Wybiera, czy chce wczytac gre
	virtual void przygotuj_rozgrywke(void); ////Przygotowuje rozgrywke, setuje tablice i liste
	virtual void przygotuj_rozgrywke(int* tablica);////Przygotowuje rozgrywke 1 v 1 z identyczna referencja, setuje tablice i liste
};
#endif
