#ifndef gracz_komputer_hpp
#define gracz_komputer_hpp
////Za³¹czenie plików i bibliotek
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<cstdio>
#include<cstdbool>
#include<string.h>
#include"gracz.hpp"
#include"poziom.hpp"

using namespace std;

class gracz_komputer: public gracz {
public:
	int pobierz_odpowiedz(void); ////Symuluje pobieranie odpowiedzi od uzytkownika
	plansza* pobierz_adres_gracza(void); ////Pobiera adres do planszy wybranego gracza
};
#endif
