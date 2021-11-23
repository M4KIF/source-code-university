#include<iostream>
#include<string>
#include<stdexcept>
#include<iomanip>
#define SZER 3
#define WYS 3

using namespace std;

/*Program ma na celu przeprowadzenie rozgrywki w Kolko i Krzyzyk pomiedzy dwoma graczami.Obsluguje wybor nicku
i wybor znaku, ktory zaczyna rozgrywke. Autor: Gal Anonim\n*/

/////Klasa gra

class Gra {
protected:
	string gracz1, gracz2; ////Imiona graczy
	char** tab; ////Dynamiczna tab, do przech planszy
public:
	void wczytaj_imiona();
	bool ustaw_pole(char znak);
	void wypisz_plansze();
	void wypisz_spis();
	bool sprawdz_stan();
	bool rozgrywka();
	Gra(): gracz1(" "), gracz2(" ") { ////Konstruktor domyslny, alokuje pamiec do tablicy z plansza
		cout << "\nTworze nowa gre!\n\n";
		tab = new char* [WYS];
		for (int i = 0; i < SZER; i++) {
			tab[i] = new char[SZER];
		}
		for (int i = 0; i < WYS; i++) {
			for (int j = 0; j < SZER; j++) {
				tab[i][j] = ' ';
			}
		}
	}
	~Gra() { ////Destruktor, usuwa zmienna dynamiczna
		cout << "\nKoncze gre!\n\n";
		for (int i = 0; i < SZER; i++) {
			delete tab[i];
		}
		delete tab;
		tab = NULL;
	}
};

void Gra::wczytaj_imiona() {
	bool czy_pomyslnie = false;
	while (!czy_pomyslnie) {
		try {
			cout << "Podaj prosze imie pierwszego gracza: ";
			cin >> gracz1;
			cout << "\nPodaj prosze imie drugiego gracza: ";
			cin >> gracz2;
			if (gracz1 == gracz2) {
				throw exception("\nImiona sa takie same, prosze powtorzyc ich wybor\n");
			}
			else {
				cout << "Nicki graczy to: " << gracz1 << "(X) i " << gracz2 << "(O)" << endl;
				czy_pomyslnie = true;
			}
			
		}
		catch (exception& a) {
			cout << a.what();
		}
	}
}

bool Gra::ustaw_pole(char znak) {
	bool poprawnosc = false;
	while (!poprawnosc) {
		try {
			cout << "Prosze o podanie cyfry od 1 do 9, celem ustawienia znaku " << znak << " na polu." << endl;
			cout << "Twoja cyfra to: ";
			int pom;
			int pole = 0;
			cin >> pom;
			if (pom < 1 || pom>9) {
				throw exception("\nWartosci sie nie zgadzaja\n");
			}
			else poprawnosc = true;
			if (pom > 3 && pom <= 6) {
				pole = pom - 3;
				pole--;
				if (tab[1][pole] == ' ') {
					tab[1][pole] = znak;
				}
				else return false;
			}
			else if (pom > 6) {
				pole = pom - 6;
				pole--;
				if (tab[2][pole] == ' ') {
					tab[2][pole] = znak;
				}
				else return false;
			}
			else if (pom <= 3) {
				pole = pom;
				pole--;
				if (tab[0][pole] == ' ') {
					tab[0][pole] = znak;
				}
				else return false;
			}
			return true;
		}
		catch (exception& a) {
			cout<< a.what();
			continue;
		}
	}
}

void Gra::wypisz_plansze() {
	cout << "\nWypisze obecny stan planszy\n" << endl;
	for (int i = 0; i < WYS; i++) {
		for (int j = 0; j < SZER; j++) {
			if(j< SZER-1) cout << tab[i][j] << " | ";
			else if (j == SZER - 1) {
				cout << tab[i][j] << "\n";
			}
		}
		if (i < WYS - 1) {
			cout << setfill('-') << setw(10) << "\n";
		}
	}
	cout << endl;
}

void Gra::wypisz_spis() {
	cout << "\nWypisze pomoc do wprowadzania danych\n" << endl;
	int spis = 1;
	for (int i = 0; i < WYS; i++) {
		for (int j = 0; j < SZER; j++) {
			if (j < SZER - 1) cout << spis << " | ";
			else if (j == SZER - 1) {
				cout << spis << "\n";
			}
			spis++;
		}
		if (i < WYS - 1) {
			cout << setfill('-') << setw(10) << "\n";
		}
	}
	cout << endl;
}

bool Gra::sprawdz_stan() {
	int ile_wygranych;
	for (int i = 0; i < WYS; i++) { ///Sprawdzenie w poziomie
		if (tab[i][0] != ' ' && tab[i][1] != ' ' && tab[i][2] != ' ') {
			if (tab[i][0] == tab[i][1] && tab[i][1] == tab[i][2] && tab[i][0] == tab[i][2]) {
				return true;
			}
		}
	}
	for (int i = 0; i < WYS; i++) {
		if (tab[0][i] != ' ' && tab[1][i] != ' ' && tab[2][i] != ' ') {
			if (tab[0][i] == tab[1][i] && tab[1][i] == tab[2][i] && tab[0][i] == tab[2][i]) {
				return true;
			}
		}
	}
	if (tab[0][0] != ' ' && tab[1][1] != ' ' && tab[2][2] != ' ') {
		if (tab[0][0] == tab[1][1] && tab[1][1] == tab[2][2] && tab[0][0] == tab[2][2]) {
			return true;
		}
	}
	if (tab[0][2] != ' ' && tab[1][1] != ' ' && tab[2][0] != ' ') {
		if (tab[0][2] == tab[1][1] && tab[1][1] == tab[2][0] && tab[0][2] == tab[2][0]) {
			return true;
		}
	}
	return false;
}

bool Gra::rozgrywka() {
	////Pobiera imiona;
	bool poprawna_konf = false;
	while (!poprawna_konf) {
		try {
			wczytaj_imiona();
			bool czy_wygral = false;
			int ile = 0;
			cout << "\nKto zaczyna? X(1), czy O(2)?\n";
			cout << "Wybor: ";
			int znak;
			cin >> znak;
			bool kolejnosc = false;
			if (znak == 1) {
				kolejnosc = false;
				poprawna_konf = true;
			}
			else if (znak == 2) {
				kolejnosc = true;
				poprawna_konf = true;
			}
			else throw exception("Bledna decyzja przy znakach. Zaczne od poczatku. Tym razem przeczytaj to, co masz zrobic!\n");
			while (!(czy_wygral && ile != 8)) {
				if (!kolejnosc) {
					///Jezeli false to X
					cout << setw(40) << setfill('/') << "\n" << endl;
					cout << "Ruch" << "(" << ile+1 << ") " << "gracza " << gracz1 << " (X)" << endl;
					wypisz_spis();
					while (!ustaw_pole('X')) {
						cout << "\n!!!BLAD!!!\nProsze poprawic\n\n";
					}
					wypisz_plansze();
					czy_wygral = sprawdz_stan();
					kolejnosc = true; ///zamiana na kolejnego gracza
					ile++;
					if (czy_wygral) {
						cout << "\nGRATULUJE!\n~Wygrywa gracz " << gracz1 << " X" << endl;
						break;
					}
					continue;
				}
				else if (kolejnosc) {
					///Jezeli true to O
					cout << setw(40) << setfill('/') << "\n" << endl;
					cout << "Ruch" << "(" << ile+1 << ") " << "gracza " << gracz1 << " (O)" << endl;
					wypisz_spis();
					while (!ustaw_pole('O')) {
						cout << "\n!!!BLAD!!!\nProsze poprawic\n\n";
					}
					wypisz_plansze();
					czy_wygral = sprawdz_stan();
					kolejnosc = false; ///zamiana na kolejnego gracza
					ile++;
					if (czy_wygral) {
						cout << "\nGRATULUJE!\n~Wygrywa gracz " << gracz2 << " O" << endl;
						break;
					}
					continue;
				}
			}
			if (czy_wygral) {
				return true;
			}
		}
		catch (exception& a) {
			cout << a.what();
			poprawna_konf = false;
			continue;
		}
	}
}

void wstep() {
	cout << "\nProgram ma na celu przeprowadzenie rozgrywki w Kolko i Krzyzyk pomiedzy dwoma graczami. Obsluguje wybor nicku\n";
	cout << "i wybor znaku, ktory zaczyna rozgrywke. Autor: Gal Anonim" << endl;
}

int main() {
	//////Wstep
	wstep();
	//////Rozgrywka
	Gra nowa;
	nowa.rozgrywka();
	//////Koniec
	return 0;
}