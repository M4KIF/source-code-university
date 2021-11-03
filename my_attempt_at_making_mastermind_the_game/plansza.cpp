#include "plansza.hpp"

////Konstruktory i destruktory

plansza::plansza() : poziom(), glowa(NULL), wsk(NULL) {};
plansza::plansza(int ile) : poziom(ile), glowa(NULL), wsk(NULL) {};
plansza::~plansza() {
	delete glowa;
	wsk = NULL;
	glowa = NULL;
}

////Deklaracje setterow i getterow

void plansza::ustaw_ilosc_pol(int ile) {
	ile_pol = ile;
}

int plansza::pobierz_ilosc_pol(void) {
	return ile_pol;
}

int* plansza::pobierz_referencje(void) {
	return pobierz_tablice();
}

void plansza::ustaw_referencje(void) {
	stworz_tablice_pol();
	switch (ile_pol) {
	case 4:
		ustaw_wartosc_pol((rand() % 9) + 1, (rand() % 9) + 1, (rand() % 9) + 1, (rand() % 9) + 1);
		break;
	case 5:
		ustaw_wartosc_pol((rand() % 9) + 1, (rand() % 9) + 1, (rand() % 9) + 1, (rand() % 9) + 1, (rand() % 9) + 1);
		break;
	}
}

void plansza::ustaw_referencje(int* tablica) {
	stworz_tablice_pol();
	switch (ile_pol) {
	case 4:
		ustaw_wartosc_pol(tablica[0], tablica[1], tablica[2], tablica[3]);
		break;
	case 5:
		ustaw_wartosc_pol(tablica[4], tablica[3], tablica[2], tablica[1], tablica[0]);
		break;
	}
}

//Deklaracje metod

void plansza::wypisz_referencje(void) {
	int ile = 1;
	int* pola = pobierz_tablice();
	cout << "Referencja to: ";
	for (int i = 0; i < ile_pol; i++) {
		cout << pola[i];
	}
	cout << endl;
}

void plansza::zeruj_liste(void) {
	wsk = glowa;
}

poziom* plansza::pobierz_glowe(void) {
	return	glowa;
}

void plansza::stworz_liste_poziomow(void) {
	for (int i = 0; i < 9; i++) {
		if (glowa == NULL) {
			glowa = new poziom(ile_pol);
			glowa->stworz_tablice_pol();
			wsk = glowa;
			wsk->nast = NULL;
		}
		else if (i == 1) {
			wsk->nast = new poziom(ile_pol);
			wsk = wsk->nast;
			wsk->stworz_tablice_pol();
			wsk->nast = NULL;
		}
		else {
			wsk->nast = new poziom(ile_pol);
			wsk = wsk->nast;
			wsk->stworz_tablice_pol();
			wsk->nast = NULL;
		}
	}
}

void plansza::wypisz_liste_poziomow(void) {
	wsk = glowa;
	cout << wsk;
	int i = 1;
	cout << endl;
	while (wsk) {
		cout << "W probie " << i << " podales takie ciag: ";
		wsk->wypisz_wartosc_pol();
		cout << endl;
		i++;
		wsk = wsk->nast;
	}
	wsk = glowa;
}
