#define _CRT_SECURE_NO_WARNINGS
////Za³¹czenie plików i bibliotek

#include"poziom.hpp"
#include"plansza.hpp"
#include"gracz.hpp"
#include"gracz_komputer.hpp"
#include<iostream>
#include<string>
#include<cstdio>
#include<cstdbool>
#include<string.h>
#include<ctime>
#include<time.h>

using namespace std;

////Deklaracja stalych

#define MAKS_PROB 9
#define SZER_LATWA 4
#define SZER_TRUDNA 5
#define SZER_MAKS 5

/////Szablon funkcji

/*Szablon funkcji do wyciagania czynnikow z liczb w postaci 1xxxxx, gdzie x >0
a to parametr ograniczajacy licznik, b to przyjmowana liczba, a *tab to tablica o rozmiarze a, do ktorej zapisze sie cyfry*/

template <typename T>
void wyjmij_wspolczynniki(T &a, T &b, T* tab) {
	for (int i = 0; i < a; i++) {
		tab[i] = b % 10;
		b /= 10;
	}
}

/*Slu¿y do zerowania tablicy dowolnej*/

template <typename T>
void zeruj_tablice(T& a, T* tab) {
	for (int i = 0; i < a; i++) {
		tab[i] = 0;
	}
}

/////Deklaracje metod niezaleznych, z informacjami itd.

/*Informacje o programie, zasady itd.*/
void informacje(void);
/*Funkcja dajaca flage zamkniecia programu i wypisujaca informacje koncowe*/
bool koniec(void);

////Definicje metod niezaleznych

void informacje(void) {
	srand((unsigned)time(NULL));
	cout << "\n/////////////////////////////////////////////////////\n";
	cout << "//\t\tGra Mastermind\t\t\t   //\n";
	cout << "//\t\tAutor: Jakub Koterba\t\t   //\n";
	cout << "/////////////////////////////////////////////////////\n\nInformacje:\n";
	cout << "- Uruchomiles wlasnie autorska gre Mastermind, ktorej zasady sa bardzo proste.\n- Komputer wylosuje 4, lub 5 kolorow(liczb od 1 do 9)." << endl;
	cout << "- Twoim zadaniem jest odgadnac jaki szyfr wylosowal komputer w 9 ruchach" << endl;
	cout << "- Aby zakonczyc dzialanie programu w locie wystarczy wpisac cyfre 0 podczas podawania ciagu\n" << endl;
}

bool koniec(void) {
	cout << "\n/////////////////////////////////////////////////////////////////////////////////////////////////\n";
	cout << "Koniec gry. Jezeli trzeba, to zapisuje obecny stan gry(o ile nie wykonano 9 prob)... Do widzenia!\n";
	cout << "/////////////////////////////////////////////////////////////////////////////////////////////////\n";
	return true;
}

int main(void) {
	////Informacje o wykonaniu i przeznaczeniu gry
	informacje();
	////Wybor poziomu trudnosci przez uzytkownika
	FILE* log; ////Wskaznik do pliku z logami tego programu
	time_t t = time(&t); ////zmienna pomocnicza do warunku z czasem, przechowuje katualny seed czasu
	struct tm tm = *localtime(&t); ////Uzupelnia structa  z biblioteki ctime aktualnymi danymi
	log = freopen("log.txt", "w", stderr);
	//////////Info
	fprintf(log, "%d-%02d-%02d %02d:%02d:%02d start programu\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	cout << "///////////////////" << endl;
	cout << "Ustawienia wstepne.\n";
	cout << "///////////////////" << endl;
	gracz* player = new gracz(); ////Wskaznik do nowego gracza
	player->wybor_poziomu_trudnosci();
	int tryb_gry, wczytac_gre, koniec_pom, i, pom, ile, odpowiedz1, odpowiedz; /*Zmienne do trybu grym wczytania gry, wyboru konca gry, numeru proby, pomocniczych do rozkladania liczby na cyfry*/
	wczytac_gre = 0;
	tryb_gry = player->wybor_trybu_gry();
	char nazwa_save[] = "save.txt";
	gracz* drugi = NULL;
	bool trafione, koniec_flaga;
	odpowiedz = 0;
	int liczba[SZER_MAKS];
	koniec_flaga = false;
	/*Tutaj switch bedzie wykonywal wybrane przez gracza tryby gry, mozliwe single, albo dual, albo demo, odpowiednio case 1, case 2, case 3*/
	switch (tryb_gry) { 
	default:
		t = time(&t);
		tm = *localtime(&t);
		fprintf(log, "%d-%02d-%02d %02d:%02d:%02d blad krytyczny przy wyborze trybu gry: \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		perror(" nie uda³o siê otworzyæ");
		fclose(log);
		return 0;
	case 1:
		cout << "//////////////////////////////////////////\n";
		cout << "Wybrales tryb pojedynczego gracza, czyli:\n- Komputer ustala szyfr referencyjny\n- Probujesz w 9 probach odgadnac szyfr";
		cout << "\n- Po kazdej probie dostajesz hinta" << endl;
		cout << "//////////////////////////////////////////\n";
		wczytac_gre = player->wybor_wczytania();
		if (wczytac_gre == 1) {
			poziom* player_glowa, *player_wsk;
			player_glowa = player_wsk = NULL;
			player->przygotuj_rozgrywke();
			player_glowa = player_wsk = player->wczytaj_save();
			if (player->pobierz_ilosc_zapisow() == 9) {
				cout << "\n\nOsiagnieto maksymalna liczbe prob w rozgrywce\nWczytano pelnego save'a, nie bylo czego konczyc w tamtej rozgrywce."<<endl;
				delete player;
				player = NULL;
				player_glowa = NULL;
				player_wsk = NULL;
				koniec_flaga = koniec();
			}
			if (koniec_flaga) {
				t = time(&t);
				tm = *localtime(&t);
				fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zakonczenie gry - wczytano z save'a wszystkie pola\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
				fclose(log);
				return 0;
			}
			player->otworz_plik_save(nazwa_save);
			i = player->pobierz_ilosc_zapisow();
			/*i++;*/
			ile = player->pobierz_ilosc_pol();
			liczba[SZER_MAKS];
			pom = 0;
			while (player_wsk) {
				int odpowiedz = 0;
				cout << "\n///////////////////////////////////////\n";
				cout << "//\t\tProba " << i << "\t\t     //";
				cout << "\n///////////////////////////////////////\n";
				odpowiedz = player->pobierz_odpowiedz();
				if (odpowiedz == 0) {
					cout << "\nWpisales cyfre odpowiadajaca zakonczeniu gry, czy jestes pewien: 1. Tak, 2. Nie, twoj wybor: ";
					cin >> koniec_pom;
					while (koniec_pom != 1 && koniec_pom != 2) {
						cout << "\nWpisales cyfre odpowiadajaca zakonczeniu gry, czy jestes pewien: 1. Tak, 2. Nie, twoj wybor: ";
						cin >> koniec_pom;
					}
					if (koniec_pom == 1) {
						player->zamknij_plik_save();
						delete player;
						player_glowa = player_wsk = NULL;
						player = NULL;
						koniec_flaga = koniec();
					}
					else {
						continue;
					}
				}
				if (koniec_flaga) {
					t = time(&t);
					tm = *localtime(&t);
					fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zakonczenie gry\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
					fclose(log);
					return 0;
				}
				pom = odpowiedz;
				wyjmij_wspolczynniki<int>(ile, pom, liczba);
				t = time(&t);
				tm = *localtime(&t);
				odpowiedz1 = odpowiedz;
				fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zapisanie odpowiedzi: %d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, odpowiedz1);
				switch (ile) {
				case 4:
					player_wsk->ustaw_wartosc_pol(liczba[3], liczba[2], liczba[1], liczba[0]);
					player->zapisz_runde(ile, i, odpowiedz1, liczba[3], liczba[2], liczba[1], liczba[0]);
					trafione = player_wsk->porownaj_probe_z_referencja(player->pobierz_tablice());
					if (trafione) {
						break;
					}
					player_wsk->wypisz_rezultat_porownania();
					break;
				case 5:
					player_wsk->ustaw_wartosc_pol(liczba[4], liczba[3], liczba[2], liczba[1], liczba[0]);
					player->zapisz_runde(ile, i, odpowiedz1, liczba[4], liczba[3], liczba[2], liczba[1], liczba[0]);
					trafione = player_wsk->porownaj_probe_z_referencja(player->pobierz_tablice());
					if (trafione) {
						break;
					}
					player_wsk->wypisz_rezultat_porownania();
					break;
				}
				player_wsk = player_wsk->nast;
				i++;
			}
			if (trafione == true) {
				cout << "/////////Wygrales//////////\n";
				cout << endl;
				player->wypisz_referencje();
				player->zamknij_plik_save();
				delete player;
				player = NULL;
				player_wsk = NULL;
				player_glowa = NULL;
				koniec_flaga = koniec();
			}
			else if (player_wsk == NULL) {
				cout << endl;
				player->wypisz_referencje();
				player->zamknij_plik_save();
				delete player;
				player = NULL;
				player_wsk = NULL;
				player_glowa = NULL;
				koniec_flaga = koniec();
			}
			if (koniec_flaga) {
				t = time(&t);
				tm = *localtime(&t);
				fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zakonczenie gry\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
				fclose(log);
				return 0;
			}
		}
		else if (wczytac_gre == 2) {
			i = 1;
			poziom* player_glowa, * player_wsk;
			player_glowa = player_wsk = NULL;
			player->przygotuj_rozgrywke();
			player->otworz_plik_save_w(nazwa_save);
			player_glowa = player_wsk = player->pobierz_glowe();
			ile = player->pobierz_ilosc_pol();
			pom = 0;
			while (player_wsk) {
				cout << "\n///////////////////////////////////////\n";
				cout << "//\t\tProba " << i << "\t\t     //";
				cout << "\n///////////////////////////////////////\n";
				odpowiedz = player->pobierz_odpowiedz();
				if (odpowiedz == 0) {
					cout << "\nWpisales cyfre odpowiadajaca zakonczeniu gry, czy jestes pewien: 1. Tak, 2. Nie, twoj wybor: ";
					cin >> koniec_pom;
					while (koniec_pom != 1 && koniec_pom != 2) {
						cout << "Wybrales opcje zakonczenia gry, czy jestes pewien?";
						cout << "\n1. Tak\n2. Nie\nTwoj wybor:";
						cin >> koniec_pom;
					}
					if (koniec_pom == 1) {
						player->zamknij_plik_save();
						player->wypisz_referencje();
						delete player;
						player_glowa = player_wsk = NULL;
						player = NULL;
						koniec_flaga = koniec();
					}
					else {
						continue;
					}
				}
				if (koniec_flaga) {
					t = time(&t);
					tm = *localtime(&t);
					fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zakonczenie gry\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
					fclose(log);
					return 0;
				}
				pom = odpowiedz;
				wyjmij_wspolczynniki<int>(ile, pom, liczba);
				t = time(&t);
				tm = *localtime(&t);
				odpowiedz1 = odpowiedz;
				fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zapisanie odpowiedzi: %d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, odpowiedz1);
				switch (ile) {
				case 4:
					player_wsk->ustaw_wartosc_pol(liczba[3], liczba[2], liczba[1], liczba[0]);
					player->zapisz_runde(ile, i, odpowiedz1, liczba[3], liczba[2], liczba[1], liczba[0]);
					trafione = player_wsk->porownaj_probe_z_referencja(player->pobierz_tablice());
					if (trafione) {
						break;
					}
					player_wsk->wypisz_rezultat_porownania();
					break;
				case 5:
					player_wsk->ustaw_wartosc_pol(liczba[4], liczba[3], liczba[2], liczba[1], liczba[0]);
					player->zapisz_runde(ile, i, odpowiedz1, liczba[4], liczba[3], liczba[2], liczba[1], liczba[0]);
					trafione = player_wsk->porownaj_probe_z_referencja(player->pobierz_tablice());
					if (trafione) {
						break;
					}
					player_wsk->wypisz_rezultat_porownania();
					break;
				}
				player_wsk = player_wsk->nast;
				i++;
				if (trafione == true) {
					player->zamknij_plik_save();
					cout << endl;
					player->wypisz_referencje();
					delete player;
					player = NULL;
					player_wsk = NULL;
					player_glowa = NULL;
					koniec_flaga = koniec();
				}
				else if (player_wsk == NULL) {
					player->zamknij_plik_save();
					cout << endl;
					player->wypisz_referencje();
					delete player;
					player = NULL;
					player_wsk = NULL;
					player_glowa = NULL;
					koniec_flaga = koniec();
				}
				if (koniec_flaga) {
					t = time(&t);
					tm = *localtime(&t);
					fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zakonczenie gry\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
					fclose(log);
					return 0;
				}
			}
		}
		break;
	case 2:
		cout << "//////////////////////////////////////////\n";
		cout << "Wybrales tryb 1v1, czyli:\n- Komputer ustala szyfr referencyjny\n- Probujesz wraz z drugim graczem w 9 probach odgadnac szyfr";
		cout << "\n- Po kazdej probie dostajecie hinta" << "\n-Kto pierwszy ten lepszy"<<endl;
		cout << "//////////////////////////////////////////\n";
		drugi = new gracz();
		drugi->dodaj_gracza();
		drugi->ustaw_ilosc_pol(player->pobierz_ilosc_pol());
		player->przygotuj_rozgrywke();
		drugi->przygotuj_rozgrywke(player->pobierz_tablice());
		poziom* gracz1_glowa, * gracz1_wsk;
		gracz1_glowa = gracz1_wsk = NULL;
		gracz1_glowa = gracz1_wsk = player->pobierz_glowe();
		poziom* gracz2_glowa, * gracz2_wsk;
		gracz2_glowa = gracz2_wsk = NULL;
		gracz2_glowa = gracz2_wsk = drugi->pobierz_glowe();
		ile = player->pobierz_ilosc_pol();
		liczba[SZER_MAKS];
		pom = 0;
		i = 1;
		while (gracz1_wsk||gracz2_wsk) {
			cout << "\n///////////////////////////////////////\n";
			cout << "//\tGracz 1 / Proba " << i << "\t     //";
			cout << "\n///////////////////////////////////////\n";
			odpowiedz = player->pobierz_odpowiedz();
			if (odpowiedz == 0) {
				cout << "\nWpisales cyfre odpowiadajaca zakonczeniu gry, czy jestes pewien: 1. Tak, 2. Nie, twoj wybor: ";
				cin >> koniec_pom;
				while (koniec_pom != 1 && koniec_pom != 2) {
					cout << "Wybrales opcje zakonczenia gry, czy jestes pewien?";
					cout << "\n1. Tak\n2. Nie\nTwoj wybor:";
					cin >> koniec_pom;
				}
				if (koniec_pom == 1) {
					player->wypisz_referencje();
					delete drugi;
					delete player;
					player = NULL;
					gracz1_glowa = gracz1_wsk = NULL;
					gracz2_glowa = gracz2_wsk = NULL;
					drugi = NULL;
					koniec_flaga = koniec();
				}
				else {
					continue;
				}
			}
			if (koniec_flaga) {
				t = time(&t);
				tm = *localtime(&t);
				fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zakonczenie gry\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
				fclose(log);
				return 0;
			}
			pom = odpowiedz;
			wyjmij_wspolczynniki<int>(ile, pom, liczba);
			t = time(&t);
			tm = *localtime(&t);
			odpowiedz1 = odpowiedz;
			fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zapisanie odpowiedzi: %d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, odpowiedz1);
			switch (ile) {
			case 4:
				gracz1_wsk->ustaw_wartosc_pol(liczba[3], liczba[2], liczba[1], liczba[0]);
				trafione = gracz1_wsk->porownaj_probe_z_referencja(player->pobierz_tablice());
				if (trafione) {
					break;
				}
				gracz1_wsk->wypisz_rezultat_porownania();
				cout << endl;
				break;
			case 5:
				gracz1_wsk->ustaw_wartosc_pol(liczba[4], liczba[3], liczba[2], liczba[1], liczba[0]);
				trafione = gracz1_wsk->porownaj_probe_z_referencja(player->pobierz_tablice());
				if (trafione) {
					break;
				}
				gracz1_wsk->wypisz_rezultat_porownania();
				cout << endl;
				break;
			}
			if (trafione == true) {
				cout << "\n/////////////////\nWygrywa Gracz 1\n//////////////////";
				delete player;
				delete drugi;
				player = NULL;
				drugi = NULL;
				gracz1_wsk = NULL;
				gracz1_glowa = NULL;
				gracz2_wsk = NULL;
				gracz2_glowa = NULL;
				t = time(&t);
				tm = *localtime(&t);
				fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zakonczenie gry\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
				koniec();
			}
			else if (gracz1_wsk == NULL) {
				player->wypisz_referencje();
				delete player;
				delete drugi;
				player = NULL;
				drugi = NULL;
				gracz1_wsk = NULL;
				gracz1_glowa = NULL;
				gracz2_wsk = NULL;
				gracz2_glowa = NULL;
				t = time(&t);
				tm = *localtime(&t);
				fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zakonczenie gry\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
				koniec();
			}
			cout << "\n///////////////////////////////////////\n";
			cout << "//\tGracz 2 / Proba " << i << "\t     //";
			cout << "\n///////////////////////////////////////\n";
			odpowiedz = drugi->pobierz_odpowiedz();
			if (odpowiedz == 0) {
				cout << "\nWpisales cyfre odpowiadajaca zakonczeniu gry, czy jestes pewien: 1. Tak, 2. Nie, twoj wybor: ";
				cin >> koniec_pom;
				while (koniec_pom != 1 && koniec_pom != 2) {
					cout << "Wybrales opcje zakonczenia gry, czy jestes pewien?";
					cout << "\n1. Tak\n2. Nie\nTwoj wybor:";
					cin >> koniec_pom;
				}
				if (koniec_pom == 1) {
					player->wypisz_referencje();
					delete player;
					delete drugi;
					gracz1_glowa = gracz1_wsk = NULL;
					gracz2_glowa = gracz2_wsk = NULL;
					player = NULL;
					drugi = NULL;
					koniec_flaga = koniec();
				}
				else {
					continue;
				}
			}
			if (koniec_flaga) {
				t = time(&t);
				tm = *localtime(&t);
				fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zakonczenie gry\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
				fclose(log);
				return 0;
			}
			pom = odpowiedz;
			wyjmij_wspolczynniki<int>(ile, pom, liczba);
			t = time(&t);
			tm = *localtime(&t);
			odpowiedz1 = odpowiedz;
			fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zapisanie odpowiedzi: %d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, odpowiedz1);
			switch (ile) {
			case 4:
				gracz2_wsk->ustaw_wartosc_pol(liczba[3], liczba[2], liczba[1], liczba[0]);
				trafione = gracz2_wsk->porownaj_probe_z_referencja(player->pobierz_tablice());
				if (trafione) {
					break;
				}
				gracz2_wsk->wypisz_rezultat_porownania();
				cout << endl;
				break;
			case 5:
				gracz2_wsk->ustaw_wartosc_pol(liczba[4], liczba[3], liczba[2], liczba[1], liczba[0]);
				trafione = gracz2_wsk->porownaj_probe_z_referencja(player->pobierz_tablice());
				if (trafione) {
					break;
				}
				gracz2_wsk->wypisz_rezultat_porownania();
				cout << endl;
				break;
			}
			if (trafione) {
				cout << "\n/////////////////\nWygrywa Gracz 1\n//////////////////" << endl;
				delete player;
				delete drugi;
				player = NULL;
				drugi = NULL;
				gracz1_wsk = NULL;
				gracz1_glowa = NULL;
				gracz2_wsk = NULL;
				gracz2_glowa = NULL;
				koniec();
			}
			gracz1_wsk = gracz1_wsk->nast;
			gracz2_wsk = gracz2_wsk->nast;
			i++;
		}
		cout << endl;
		player->wypisz_referencje();
		cout << "\nZaden z graczy nie wygral";
		if (koniec_flaga) {
			delete player;
			delete drugi;
			player = NULL;
			drugi = NULL;
			gracz1_wsk = NULL;
			gracz1_glowa = NULL;
			gracz2_wsk = NULL;
			gracz2_glowa = NULL;
			t = time(&t);
			tm = *localtime(&t);
			fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zakonczenie gry\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
			fclose(log);
			return 0;
		}
		if (gracz2_wsk == NULL){
			cout << endl;
			player->wypisz_referencje();
			delete player;
			delete drugi;
			player = NULL;
			drugi = NULL;
			gracz1_wsk = NULL;
			gracz1_glowa = NULL;
			gracz2_wsk = NULL;
			gracz2_glowa = NULL;
			koniec_flaga = koniec();
		}
		break;
	case 3:
		drugi = dynamic_cast<gracz*>(new gracz_komputer);
		drugi->ustaw_ilosc_pol(player->pobierz_ilosc_pol());
		drugi->przygotuj_rozgrywke();
		poziom* komputer_glowa, * komputer_wsk;
		komputer_glowa = komputer_wsk = NULL;
		komputer_glowa = komputer_wsk = drugi->pobierz_glowe();
		ile = player->pobierz_ilosc_pol();
		zeruj_tablice<int>(ile, liczba);
		i = 1;
		while (komputer_wsk) {
			odpowiedz = drugi->pobierz_odpowiedz();
			pom = odpowiedz;
			wyjmij_wspolczynniki<int>(ile, pom, liczba);
			t = time(&t);
			tm = *localtime(&t);
			odpowiedz1 = odpowiedz;
			fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zapisanie odpowiedzi: %d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, odpowiedz1);
			cout << "\n////////// Proba " << i << " / Komputera to: " << odpowiedz << endl;
			switch (ile) {
			case 4:
				komputer_wsk->ustaw_wartosc_pol(liczba[3], liczba[2], liczba[1], liczba[0]);
				trafione = komputer_wsk->porownaj_probe_z_referencja(drugi->pobierz_tablice());
				if (trafione) {
					break;
				}
				komputer_wsk->wypisz_rezultat_porownania();
				break;
			case 5:
				komputer_wsk->ustaw_wartosc_pol(liczba[4], liczba[3], liczba[2], liczba[1], liczba[0]);
				trafione = komputer_wsk->porownaj_probe_z_referencja(drugi->pobierz_tablice());
				if (trafione) {
					break;
				}
				komputer_wsk->wypisz_rezultat_porownania();
				break;
			}
			if (trafione == true) {
				drugi->wypisz_referencje();
				delete player;
				delete drugi;
				player = NULL;
				drugi = NULL;
				komputer_wsk = NULL;
				komputer_glowa = NULL;
				koniec();
			}
			komputer_wsk = komputer_wsk->nast;
			i++;
		}
		cout << endl;
		drugi->wypisz_referencje();
		delete player;
		delete drugi;
		player = NULL;
		drugi = NULL;
		komputer_wsk = NULL;
		komputer_glowa = NULL;
		koniec_flaga = koniec();
		break;
	}
	if (koniec_flaga) {
		t = time(&t);
		tm = *localtime(&t);
		fprintf(log, "%d-%02d-%02d %02d:%02d:%02d zakonczenie gry\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		fclose(log);
		return 0;
	}
}
