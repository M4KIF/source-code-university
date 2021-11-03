#include "gracz.hpp"

////Definicje konstruktora i destruktora

gracz::~gracz() {
	delete osoba;
	delete save;
}
gracz::gracz() : plansza(), osoba(NULL), ile_zapisow(1), save(NULL) {};

////Definicje metod

void gracz::dodaj_gracza() {
	osoba = new plansza();
}

plansza* gracz::pobierz_adres_gracza(void) {
	return osoba;
}

void gracz::otworz_plik_save_w(char* tab) {
	if ((save = fopen(tab, "w")) == NULL) {
		perror("Blad w utworzeniu pliku: ");
	}
	else {
		perror("Otwarto plik: ");
	}
}

void gracz::otworz_plik_save(char* tab) {
	if ((save = fopen(tab, "a+")) == NULL) {
		perror("Blad w utworzeniu pliku");
	}
	else {
		perror("Otwarto plik: ");
	}
}

FILE* gracz::pobierz_adres_pliku(void) {
	return save;
}

int gracz::pobierz_ilosc_zapisow(void) {
	return ile_zapisow;
}

void gracz::zapisz_runde(int pola, int numer, int cala, int pierwsza, int druga, int trzecia, int czwarta) {
	fprintf(save, "%d %d %d %d %d %d %d\n", pola, numer, cala, pierwsza, druga, trzecia, czwarta/*, ile_zapisow*/);////Ile pol, ktora proba, odpowiedz, liczba 1, liczba 2, liczba 3, liczba 4
	ile_zapisow++;
}

void gracz::zapisz_runde(int pola, int numer, int cala, int pierwsza, int druga, int trzecia, int czwarta, int piata) {
	fprintf(save, "%d %d %d %d %d %d %d %d\n", pola, numer, cala, pierwsza, druga, trzecia, czwarta, piata/*, ile_zapisow*/);////Ile pol, ktora proba, odpowiedz, liczba 1, liczba 2, liczba 3, liczba 4
	ile_zapisow++;
}

void gracz::zamknij_plik_save(void) {
	fclose(save);
	perror("Zamknieto plik save. ");
	save = NULL;
}

poziom* gracz::wczytaj_save(void) {
	char nazwa_save[] = "save.txt";
	if (save == NULL) {
		otworz_plik_save(nazwa_save);
	}
	if (feof(save)) {
		cout << "Plik jest pusty, zwroce wskaznik do glowy" << endl;
		return this->glowa;
	}
	wsk = glowa;
	poziom* pom = NULL;
	bool dlugosc;
	int g = 1;
	while (!feof(save) && wsk != NULL) {
		int pol, ciag, liczba[5] = { 0 };
		int zapis;
		fscanf(save, "%d", &pol);
		if (pol != 4 && pol != 5) {
			return glowa;
		}
		if (pol == 4 && !feof(save)) {
			fscanf(save, "%d %d %d %d %d %d", &zapis, &ciag, &liczba[0], &liczba[1], &liczba[2], &liczba[3]);
			ile_zapisow = zapis;
			dlugosc = sprawdz_dlugosc(ciag);
			if (pol != ile_pol) {
				ciag = 0;
				for (int i = 0; i < ile_pol; i++) {
					if (i == 0) {
						liczba[3] = rand() % 9 + 1;
						ciag += liczba[3] * 10000;
					}
					else if (i == 1) {
						liczba[2] = rand() % 9 + 1;
						ciag += liczba[2] * 1000;
					}
					else if (i == 2) {
						liczba[1] = rand() % 9 + 1;
						ciag += liczba[1] * 100;
					}
					else if(i == 3) {
						liczba[0] = rand() % 9 + 1;
						ciag += liczba[0]*10;
					}
					else if (i == 4) {
						liczba[0] = rand() % 9 + 1;
						ciag += liczba[0];
					}
				}
				cout << "\n//////// Proba " << g << " to: " << ciag << endl;
				ile_zapisow = g;
				wsk->ustaw_wartosc_pol(liczba[4], liczba[3], liczba[2], liczba[1], liczba[0]);
				wsk->porownaj_probe_z_referencja(pobierz_tablice());
				wsk->wypisz_rezultat_porownania();
			}
			else {
				cout << "\n//////// Proba " << zapis << " to: " << ciag << endl;
				wsk->ustaw_wartosc_pol(liczba[3], liczba[2], liczba[1], liczba[0]);
				wsk->porownaj_probe_z_referencja(pobierz_tablice());
				wsk->wypisz_rezultat_porownania();
			}
		}
		else if (pol == 5 && !feof(save)) {
			bool check = false;
			fscanf(save, "%d %d %d %d %d %d %d", &zapis, &ciag, &liczba[0], &liczba[1], &liczba[2], &liczba[3], &liczba[4]);
			if (pol != ile_pol) {
				ciag = 0;
				for (int i = 0; i < ile_pol; i++) {
					if (i == 0) {
						liczba[3] = rand() % 9 + 1;
						ciag += liczba[3] * 1000;
					}
					else if (i == 1) {
						liczba[2] = rand() % 9 + 1;
						ciag += liczba[2] * 100;
					}
					else if (i == 2) {
						liczba[1] = rand() % 9 + 1;
						ciag += liczba[1] * 10;
					}
					else if (i == 3) {
						liczba[1] = rand() % 9 + 1;
						ciag += liczba[1] * 1;
					}
				}
				cout << "\n//////// Proba " << g << " to: " << ciag << endl;
				ile_zapisow = g;
				wsk->ustaw_wartosc_pol(liczba[3], liczba[2], liczba[1], liczba[0]);
				wsk->porownaj_probe_z_referencja(pobierz_tablice());
				wsk->wypisz_rezultat_porownania();
			}
			else {
				cout << "\n//////// Proba " << zapis << " to: " << ciag << endl;
				ile_zapisow = zapis;
				wsk->ustaw_wartosc_pol(liczba[4], liczba[3], liczba[2], liczba[1], liczba[0]);
				wsk->porownaj_probe_z_referencja(pobierz_tablice());
				wsk->wypisz_rezultat_porownania();
			}
		}
		g++;
		pom = wsk;
		wsk = wsk->nast;
	}
	zamknij_plik_save();
	ile_zapisow++;
	return pom;
}

bool gracz::sprawdz_dlugosc(int odp) {
	int licznik = 0;
	int pomocnik = odp;
	while ((pomocnik % 10) != 0) {
		licznik++;
		pomocnik /= 10;
		if ((pomocnik % 10) == 0) {
			if (licznik != ile_pol) {
				return false;
			}
			else return true;
		}
	}
	if (licznik != ile_pol) {
		return false;
	}
	else return true;
}

int	gracz::pobierz_odpowiedz(void) {
	int odpowiedz;
	cout << "Podaj prosze ciag " << ile_pol << " liczb: ";
	cin >> odpowiedz;
	if (odpowiedz == 0) {
		return 0;
	}
	bool flaga = sprawdz_dlugosc(odpowiedz);
	if (flaga != true) {
		flaga = pobierz_odpowiedz();
	}
	else return odpowiedz;
}

void gracz::wybor_poziomu_trudnosci(void) {
	int ile;
	cout << "\nPoziom trudnosci: 1. Latwy (4 Kratki), czy 2. Trudny (5 Kratek).\nTwoj wybor to: ";
	cin >> ile;
	cout << endl;
	while (ile != 1 && ile != 2) {
		cout << "\nPoziom trudnosci: 1. Latwy (4 Kratki), czy 2. Trudny (5 Kratek).\nTwoj wybor to: ";
		cin >> ile;
		cout << endl;
	}
	if (ile == 1) {
		ile_pol = 4;
	}
	else {
		ile_pol = 5;
	}
}

int gracz::wybor_trybu_gry(void) {
	cout << "Tryb gry: 1. Pojedynczy gracz. 2. Dwoch graczy 3. Przyklad grajacego komputera\nTwoj wybor to: ";
	int tryb_gry;
	cin >> tryb_gry;
	cout << endl;
	while (tryb_gry != 1 && tryb_gry != 2 && tryb_gry != 3) {
		cout << "Bledny wybor trybu gry, prosze wybierz jeszcze raz." << endl;
		cout << "Tryb gry:\n1. Pojedynczy gracz.\n2. Dwoch graczy\n3. Przyklad grajacego komputera\nTwoj wybor to: ";
		cin >> tryb_gry;
	}
	return tryb_gry;
}

int gracz::wybor_wczytania(void) {
	int wczytac_gre;
	cout << "\nWczytac poprzednia gre: 1. Tak 2. Nie\n";
	cout << "Twoj wybor: ";
	cin >> wczytac_gre;
	while ((wczytac_gre != 1) && (wczytac_gre != 2)) {
		cout << "Blad wprowadzanych danych\n";
		cout << "Wczytac poprzednia gre: 1. Tak 2. Nie\n";
		cout << "Twoj wybor: ";
		cin >> wczytac_gre;
	}
	return wczytac_gre;
}

void gracz::przygotuj_rozgrywke(void) {
	ustaw_ilosc_pol(ile_pol);
	ustaw_referencje();
	stworz_liste_poziomow();
}

void gracz::przygotuj_rozgrywke(int* tablica) {
	ustaw_ilosc_pol(ile_pol);
	ustaw_referencje(tablica);
	stworz_liste_poziomow();
}