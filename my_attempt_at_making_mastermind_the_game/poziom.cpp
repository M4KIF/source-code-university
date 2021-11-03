#include"poziom.hpp"

////Szablony funkcji 



////Konstruktory i destruktory

poziom::poziom() : pola(NULL), ile_pol(0), ile_trafionych_pol(0), ile_trafionych_kolorow(0), nast(NULL) {};
poziom::poziom(int ile) : pola(NULL), ile_pol(ile), ile_trafionych_pol(0), ile_trafionych_kolorow(0), nast(NULL) {};
poziom::~poziom() {
	delete pola;
	pola = NULL;
	if (nast != NULL) delete nast;
}

////Definicje setterow i getterow

void poziom::ustaw_ilosc_pol(int ile) {
	ile_pol = ile;
}
int poziom::pobierz_ilosc_pol(void) {
	return ile_pol;
}

//Definicje Metod

void poziom::stworz_tablice_pol(void) {
	pola = new int[ile_pol];
}

int* poziom::pobierz_tablice(void) {
	return pola;
}

void poziom::ustaw_wartosc_pol(int n_1, int n_2, int n_3, int n_4) {
	for (int i = 0; i < ile_pol; i++) {
		if (i == 0) {
			pola[i] = n_1;
		}
		else if (i == 1) {
			pola[i] = n_2;
		}
		else if (i == 2) {
			pola[i] = n_3;
		}
		else if (i == 3) {
			pola[i] = n_4;
		}
	}
}

void poziom::ustaw_wartosc_pol(int n_1, int n_2, int n_3, int n_4, int n_5) {
	for (int i = 0; i < ile_pol; i++) {
		if (i == 0) {
			pola[i] = n_1;
		}
		else if (i == 1) {
			pola[i] = n_2;
		}
		else if (i == 2) {
			pola[i] = n_3;
		}
		else if (i == 3) {
			pola[i] = n_4;
		}
		else if (i == 4) {
			pola[i] = n_5;
		}
	}
}

void poziom::wypisz_wartosc_pol(void) {
	for (int i = 0; i < ile_pol; i++) {
		cout << pola[i];
	}
}

void poziom::wypisz_rezultat_porownania(void) {
	cout << "Poprawnych trafien: " << ile_trafionych_pol << "\nPoprawnych kolorow, ale ulozonych w zlej kolejnosci: " << ile_trafionych_kolorow;
}

bool poziom::porownaj_probe_z_referencja(int* ref) {
	int trafione[5] = { 0 , 0, 0, 0, 0 };
	int duble_proba[5] = { 0 };
	for (int i = 0; i < ile_pol; i++) {
		if (ref[i] == pola[i]) {
			ile_trafionych_pol++;
			trafione[i] = pola[i];
		}
	}
	if (ile_trafionych_pol == ile_pol) {
		return true;
	}
	for (int i = 0; i < ile_pol; i++) {
		int pom1 = pola[i];
		for (int j = 0; j < ile_pol; j++) {
			if (pom1 == pola[j]) {
				duble_proba[i] = pom1;
				break;
			}
		}
	}
	for (int i = 0; i < ile_pol; i++) {
		for (int j = 0; j < ile_pol; j++) {
			if (pola[j] == ref[i] && (pola[j] != duble_proba[i])) {
				ile_trafionych_kolorow++;
				break;
			}
		}
	}
	int ile = 0;
	if (ile_pol == 4) {
		for (int i = 0; i < ile_pol; i++) {
			if (trafione[i] != 0) {
				ile++;
			}
		}
		if (ile == 4) {
			return true;
		}
		else return false;
	}
	else if (ile_pol == 5) {
		for (int i = 0; i < ile_pol; i++) {
			if (trafione[i] != 0) {
				ile++;
			}
		}
		if (ile == 5) {
			return true;
		}
		else return false;
	}
	return false;
}
