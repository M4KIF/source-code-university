#include "gracz_komputer.hpp"
#include "plansza.hpp"

////Definicja dwoch metod wirtualnych metody

int gracz_komputer::pobierz_odpowiedz(void) {
	int odpowiedz = 0;
	if (ile_pol == 4) {
		for (int i = 0; i < ile_pol; i++) {
			int pom;
			if (i == 0) {
				pom = (rand() % 9) + 1;
				odpowiedz += (pom * 1000);
			}
			else if (i == 1) {
				pom = (rand() % 9) + 1;
				odpowiedz += (pom * 100);
			}
			else if (i == 2) {
				pom = (rand() % 9) + 1;
				odpowiedz += (pom * 10);
			}
			else if (i == 3) {
				pom = (rand() % 9) + 1;
				odpowiedz += pom;
			}
		}
	}
	if (ile_pol == 5) {
		for (int i = 0; i < ile_pol; i++) {
			int pom;
			if (i == 0) {
				pom = (rand() % 9) + 1;
				odpowiedz += (pom * 10000);
			}
			else if (i == 1) {
				pom = (rand() % 9) + 1;
				odpowiedz += (pom * 1000);
			}
			else if (i == 2) {
				pom = (rand() % 9) + 1;
				odpowiedz += (pom * 100);
			}
			else if (i == 3) {
				pom = (rand() % 9) + 1;
				odpowiedz += pom * 10;
			}
			else if (i == 4) {
				pom = (rand() % 9) + 1;
				odpowiedz += pom;
			}
		}
	}
	bool flaga = sprawdz_dlugosc(odpowiedz);
	if (flaga != true) {
		flaga = pobierz_odpowiedz();
	}
	return odpowiedz;
}

plansza* gracz_komputer::pobierz_adres_gracza(void) {
	cout << "Gra komputer, adres nie jest ani troche potrzebny" << endl;
	return NULL;
}