// Obiektowy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <time.h>
#include <typeinfo>
using namespace std;

//DEKLARACJA ZMIENNYCH GLOBALNCYH ORAZ DEKLARACJA PROCEDUR
bool koniec;
void glowne_menu();
void cennik_menu();
void plan_menu();
void reklamodawcy_menu();
void reklamodawcy_pokaz(string nazwa, double budzet, bool dlg_spotow[4], bool dni_tygodnia[2], bool godziny[8]);
void reklamodawcy_odswiez(fstream& plik_reklamodawcy);
void cenniki_pokaz(string nazwa, double cena[48][2], double mnozniki[4]);
void cenniki_odswiez(fstream& plik_cenniki);
void plany_odswiez(fstream& plik_plany);
//KLASY
template <class T>
class poprawnosc
{
private:
	string in;
	int daneint (int min, int maks)
	{
		bool zly_typ = false;
		bool zly_przedzial = false;
		int n;
		cin >> in;
		for (unsigned int i = 0; i < in.length(); i++)
		{
			if (in[i] < 48 || in[i] > 57)
			{
				zly_typ = true;
				break;
			}
		}
		if (!zly_typ)
		{
			n = stoi(in);
			if (n<min || n>maks)
			{
				zly_przedzial = true;
			}
		}
		if (zly_typ || zly_przedzial)
		{
			cout << "ZLY TYP DANYCH LUB WARTOSC POZA ZAKRESEM! PODAJ PONOWNIE" << endl << endl;
			n = daneint(min, maks);
		}
		return n;
	}
	double danedouble(double min, double maks)
	{
		bool zly_typ = false;
		bool zly_przedzial = false;
		double n;
		int ile_kropek = 0;
		cin >> in;
		for (unsigned int i = 0; i < in.length(); i++)
		{
			if (in[i] == 46)
			{
				ile_kropek++;
			}
			if ((in[i] < 48 || in[i] > 57) && in[i] != 46)
			{
				zly_typ = true;
				break;
			}
		}
		if (!zly_typ)
		{
			if (ile_kropek > 1 || in[0] == 46 || in[in.length() - 1] == 46)
			{
				zly_typ = true;
			}
		}
		if (!zly_typ)
		{
			n = stod(in);
			if (n<min || n>maks)
			{
				zly_przedzial = true;
			}
		}
		if (zly_typ || zly_przedzial)
		{
			cout << "ZLY TYP DANYCH LUB WARTOSC POZA ZAKRESEM! PODAJ PONOWNIE" << endl << endl;
			n = danedouble(min, maks);
		}
		return n;
	}
	bool danebool()
	{
		string in;
		bool zly_typ = false;
		bool n;
		cin >> in;
		if (in.length() != 1)
		{
			zly_typ = true;
		}
		if (!zly_typ)
		{
			if (in[0] != '0' && in[0] != '1')
			{
				zly_typ = true;
			}
		}
		if (!zly_typ)
		{
			if (in[0])
				n = true;
			else
				n = false;
		}
		if (zly_typ)
		{
			cout << "ZLY TYP DANYCH LUB WARTOSC POZA ZAKRESEM! PODAJ PONOWNIE" << endl << endl;
			n = danebool();
		}
		return n;
	}
public:
	T check(T min = 0, T maks = 1) 
	{
		T n;
		if (typeid(T) == typeid(int))
		{
			n = daneint(min, maks);
		}
		if (typeid(T) == typeid(double))
		{
			n = danedouble(min, maks);
		}
		if (typeid(T) == typeid(bool))
		{
			n = danebool();
		}
		return n;
	}
};
poprawnosc <int> inty;
poprawnosc <double> doubly;
poprawnosc <bool> booly;
class blok_reklamowy//klasa pomocznicza do planu reklam
{
public:
	int czas;//wolne sekundy w danych pasmie,300
	string reklamodawca[36];//nazwa reklamodawcy
	int dlugosc[36]; //dlugosc reklamy
	blok_reklamowy()//kostruktor, dane przy deklaracji
	{
		czas = 300;
		for (int i = 0; i < 36; i++)
		{
			this->reklamodawca[i] = "brak";
			this->dlugosc[i] = 0;
		}
	}
	void czysc()//usuwanie danych
	{
		czas = 0;
		for (int i = 0; i < 36; i++)
		{
			this->reklamodawca[i] = "";
			this->dlugosc[i] = 0;
		}
	}
};
class plan
{
private:
	string nazwa;//nazwa planu
	blok_reklamowy blok[48][7];//jego dane z klasy pomoczniczej
public:
	plan(string& nazwa, blok_reklamowy blok[48][7])//konstrukor, wpisywanie danych podanych jako argumenty deklaracji
	{
		this->nazwa = nazwa;
		for (int i = 0; i < 48; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				this->blok[i][j].czas = blok[i][j].czas;
				for (int k = 0; k < 36; k++)
				{
					this->blok[i][j].reklamodawca[k] = blok[i][j].reklamodawca[k];
					this->blok[i][j].dlugosc[k] = blok[i][j].dlugosc[k];
				}
			}
		}
	}
	void reklamy(string& reklamodawca)//wysiwetlanie wszytskich reklam jednego reklamodawcy
	{
		string dni[7] = { "Poniedzialek", "Wtorek      ", "Sroda       ", "Czwartek     ", "Piatek    ", "Sobota    ", "Niedziela  " };
		cout << "Dzien\t\tPasmo\t\tDlugosc reklamy" << endl << endl;
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 48; j++)
			{
				for (int k = 0; k < 36; k++)
				{
					if (this->blok[j][i].reklamodawca[k] == reklamodawca)
					{
						int godzina = 0;
						string minuta = "00";
						int godzina1 = 0;
						string minuta1 = "00";
						int godzina2 = 0;
						string minuta2 = "00";
						for (int i = 0; i < 48; i++)
						{
							if (j - 1 == i)
							{
								godzina1 = godzina;
								minuta1 = minuta;
							}
							if (minuta == "00")
							{
								minuta = "30";
							}
							else
							{
								godzina++;
								minuta = "00";
							}
							if (j - 1 == i)
							{
								godzina2 = godzina;
								minuta2 = minuta;
								break;
							}
						}
						if (godzina1 == 0 && minuta1 == "00")
						{
							godzina2 = 0;
							minuta2 = "30";
						}
						cout << dni[i] << "\t" << godzina1<<":"<<minuta1<<" - "<<godzina2<<":"<<minuta2 << "\t" << this->blok[j][i].dlugosc[k] << endl;
					}
				}
			}
		}
		cout << "\nDzien\t\tPasmo\t\tDlugosc reklamy" << endl;
	}
	void pokaz()//pokazuje czas wolny we wszystkich blokach aktualnego planu
	{
		string dni[7] = { "Poniedzialek", "Wtorek      ", "Sroda       ", "Czwartek     ", "Piatek    ", "Sobota    ", "Niedziela  " };
		int godzina = 0;
		string minuta = "00";
		cout << "Nazwa: " << this->nazwa << endl << endl;
		cout << "Pasmo" << "\t\t\t";
		for (int i = 0; i < 7; i++)
		{
			cout << dni[i] << "\t";
		}
		cout << endl;

		for (int i = 0; i < 48; i++)
		{
			cout << godzina << ":" << minuta << " - ";
			if (minuta == "00")
			{
				minuta = "30";
			}
			else
			{
				godzina++;
				minuta = "00";
			}
			cout << godzina << ":" << minuta << "\t\t";
			for (int j = 0; j < 7; j++)
			{

				cout << this->blok[i][j].czas << "\t\t";
			}
			cout << endl;
		}
	}
	void pokaz_dokladnie()//Wypisuje wszystkie reklamy w danym pasmie
	{
		string dni[7] = { "Poniedzialek", "Wtorek", "Sroda", "Czwartek", "Piatek", "Sobota", "Niedziela" };
		int godzina = 0;
		string minuta = "00";
		int dzien;
		int pasmo;
		cout << "Wybierz dzien, wpisz odpowiedni numer" << endl << endl;
		for (int i = 0; i < 7; i++)
		{
			cout << i + 1 << ". " << dni[i] << endl;
		}
		cout << endl;
		dzien = inty.check(1, 7);
		system("cls");
		cout << "Wybierz pasmo, wpisz odpowiedni numer" << endl << endl;
		for (int i = 0; i < 48; i++)
		{
			cout << i + 1 << ". ";
			cout << godzina << ":" << minuta << " - ";
			if (minuta == "00")
			{
				minuta = "30";
			}
			else
			{
				godzina++;
				minuta = "00";
			}
			cout << godzina << ":" << minuta << "\t\t";
			cout << endl;
		}
		cout << endl;
		pasmo = inty.check(1, 48);
		system("cls");
		cout << "Nazwa planu: " << this->nazwa << endl  << endl;
		cout << "Dzien: " << dni[dzien - 1] << endl << endl;
		godzina = 0;
		minuta = "00";
		for (int i = 0; i < 48; i++)
		{
			if (pasmo - 1 == i)
			{
				cout << "Pasmo: " << godzina << ":" << minuta << " - ";
			}
			if (minuta == "00")
			{
				minuta = "30";
			}
			else
			{
				godzina++;
				minuta = "00";
			}
			if (pasmo - 1 == i)
			{
				cout << godzina << ":" << minuta << endl << endl;
				break;
			}
		}
		cout << "Pozostaly czas: " << this->blok[pasmo - 1][dzien - 1].czas << endl << endl;
		cout << "Nr reklamy\t" << "Nazwa reklamodawcy\t" << "Dlugosc reklamy" << endl;
		for (int i = 0; i < 36; i++)
		{
			if (this->blok[pasmo - 1][dzien - 1].dlugosc[i])//nie wypisuj wolnych miejsc
			{
				cout << i + 1 << ".\t\t";
				cout << this->blok[pasmo - 1][dzien - 1].reklamodawca[i] << "\t\t";
				if (this->blok[pasmo - 1][dzien - 1].reklamodawca[i].length() <= 7) cout << "\t";
				cout << this->blok[pasmo - 1][dzien - 1].dlugosc[i] << endl;
			}
		}
	}		
	void zapis(fstream& plik)//zapis danych planu do pliku
	{

		plik << this->nazwa << endl;
		for (int i = 0; i < 48; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				plik<< this->blok[i][j].czas<<endl;
				for (int k = 0; k < 36; k++)
				{
					plik << this->blok[i][j].reklamodawca[k] << endl;
					plik << this->blok[i][j].dlugosc[k] << endl;
				}
			}
		}
	}
};
class cennik
{//KLASA CENNIKA
private:
	string nazwa;
	double cena[48][2];
	//dzień podzielony na 48 30 min przedziałów
	//tydzien podzielony na 2 grupy dni
	double mnozniki[4];
	//10, 20, 30, 40 sekund
public:
	cennik(string nazwa, double cena[48][2], double mnozniki[4])//konstruktor
	{
		this->nazwa = nazwa;
		for (int i = 0; i < 48; i++)
			for (int j = 0; j < 2; j++)
				this->cena[i][j] = cena[i][j];
		for (int i = 0; i < 4; i++)
			this->mnozniki[i] = mnozniki[i];
	}
	void pokaz()
	{//WYSWIETLANIE DANYCH INSTANCJI KLASY
		cout << "Nazwa cennika: " << nazwa << endl << endl;
		cout << "ID pasma\t";
		cout << "Pasmo\t\t\t";
		cout << "Pon-Czw" << "\t\t" << "Pt-Niedz" << endl;
		int godzina = 0;
		string minuta = "00";
		for (int i = 0; i < 48; i++)//48 przedziałów po 30 minut
		{
			cout << i + 1 << "\t\t";
			cout << godzina << ":" << minuta << " - ";
			if (minuta == "00")
			{
				minuta = "30";
			}
			else
			{
				godzina++;
				minuta = "00";
			}
			cout << godzina << ":" << minuta << "\t\t";
			for (int j = 0; j < 2; j++)//tydzien podzielony na 2 grupy dni
			{
				cout << cena[i][j] << "\t\t";
			}
			cout << endl;
		}
		cout << "Mnozniki" << endl;
		cout << "ID\t\t" << "czas trwania\t\t" << "Wartosc mnoznika" << endl;
		for (int i = 0; i < 4; i++)
		{
			cout  << i + 1 << "\t\t";
			cout << (i + 1) * 10 << " sekund\t\t";
			cout << mnozniki[i] << endl;
		}
		cout << endl << endl;
	}
	void pokaz_tylko_nazwa()//pokazuje tylko nazwe cennika
	{
		cout << "Nazwa cennika: " << nazwa << endl << endl;
	}
	void zapis_do_pliku(fstream& plik_cenniki)//zapis do pliku
	{
		plik_cenniki << nazwa << endl;
		for (int i = 0; i < 48; i++)
			for (int j = 0; j < 2; j++)
				plik_cenniki << cena[i][j] << endl;
		for (int i = 0; i < 4; i++)
			plik_cenniki << mnozniki[i] << endl;
	}
	void modyfikuj_cena(int ID, int dzien, double cena)//nowa cena, po modyfikacji
	{
		this->cena[ID - 1][dzien - 1] = cena;
	}
	void modyfikuj_mnoznik(int ID, double mnoznik)//nowy mnoznik, po modyfikacji
	{
		this->mnozniki[ID - 1] = mnoznik;
	}
	double aktualna_cena(int dzien, int pasmo, int dlugosc)//zwraca cene reklamy w danym dniu, pasmie, i dlugosci reklamy
	{
		int dni = 1;
		if (dzien <= 4)
		{
			dni = 0;
		}
		return this->cena[pasmo - 1][dni] * this->mnozniki[dlugosc - 1];
	}
};
class reklamodawca
{//KLASA REKLAMODAWCY
private:
	string nazwa;
	double budzet;
	bool dlg_spotow[4];		//10, 20, 30, 40 sekund
	bool dni_tygodnia[2]; // pon-czw i pt-niedz
	bool godziny[8];	//doba podzielona na 8 przedziałów po 3h każdy
public:
	reklamodawca operator+(reklamodawca X)//sumuje budzety
	{
		double kasa = this->budzet + X.budzet;
		return reklamodawca(this->nazwa, kasa, this->dlg_spotow, this->dni_tygodnia, this->godziny);
	}
	reklamodawca(string nazwa, double budzet, bool dlg_spotow[4], bool dni_tygodnia[2], bool godziny[8])
	{//KONSTRUKOR, WCZYTYWANIE DANYCH DO ZMIENNYCH INSTANCJI KLASY
		this->nazwa = nazwa;
		this->budzet = budzet;
		for (int i = 0; i < 4; i++)
			this->dlg_spotow[i] = dlg_spotow[i];
		for (int i = 0; i < 2; i++)
			this->dni_tygodnia[i] = dni_tygodnia[i];
		for (int i = 0; i < 8; i++)
			this->godziny[i] = godziny[i];
	}
	void pokaz_budzet()//pokazuje budzet
	{
		cout << this->budzet << endl;
	}
	void pokaz()
	{//WYŚWIETLANIE DANYCH INSTANCJI KLASY
		cout << endl;
		cout << "Nazwa reklamodawcy: " << nazwa << endl << "Budzet reklamodawcy: " << budzet << endl;
		cout << "Wybrane dlugosci spotow (w sekundach):\t" ;
		for (int i = 0; i < 4; i++)
			if (dlg_spotow[i])
				cout << (i + 1) * 10 << "  ";
			cout << endl << "Wybrane dni:\t";
		for (int i = 0; i < 2; i++)
			if (dni_tygodnia[i])
				if (!i)
					cout << "Poniedzialek - Czwartek  ";
				else
					cout << "Piatek - Niedziela  ";
		cout << endl << "Wybrane godziny nadawania:\t";
		for (int i = 0; i < 8; i++)
			if (godziny[i])
				cout << (i + 1) * 3 - 3 << "-" << (i + 1) * 3 <<"  ";
		cout << endl << endl;
	}
	void modyfikuj(string nazwa, double budzet, bool dlg_spotow[4], bool dni_tygodnia[2], bool godziny[8])
	{//MODYFIKACJA DANYCH OBIEKTU
		this->nazwa = nazwa;
		this->budzet = budzet;
		for (int i = 0; i < 4; i++)
			this->dlg_spotow[i] = dlg_spotow[i];
		for (int i = 0; i < 2; i++)
			this->dni_tygodnia[i] = dni_tygodnia[i];
		for (int i = 0; i < 8; i++)
			this->godziny[i] = godziny[i];
	}
	void zapis_do_pliku(fstream& plik_reklamodawcy)
	{//ZAPIS DO PLIKU PODANEGO JAKO ARGUMENT
		plik_reklamodawcy << nazwa << endl;
		plik_reklamodawcy << budzet << endl;
		for (int i = 0; i < 4; i++)
			plik_reklamodawcy << dlg_spotow[i] << endl;
		for (int i = 0; i < 2; i++)
			plik_reklamodawcy << dni_tygodnia[i] << endl;
		for (int i = 0; i < 8; i++)
			plik_reklamodawcy << godziny[i] << endl;
	}
	void podaj_dane(string &nazwa, double &budzet, bool dlg_spotow[4], bool dni_tygodnia[2], bool godziny[8])
	{//zwraca dane instancji do argumentow
		nazwa = this->nazwa;
		budzet = this->budzet;
		for (int i = 0; i < 4; i++)
			dlg_spotow[i] = this->dlg_spotow[i];
		for (int i = 0; i < 2; i++)
			dni_tygodnia[i] = this->dni_tygodnia[i];
		for (int i = 0; i < 8; i++)
			godziny[i] = this->godziny[i];
	}
	void podaj_nazwe(string& nazwa)
	{//^to samo, tylko sama nazwa
		nazwa = this->nazwa;
	}
};

//DEKLARACJA TABLIC DYNAMICZNYCH DANYCH 
vector <reklamodawca> reklamodawcy;
vector <cennik> cenniki;
vector <plan> plany;

//DEFINCIJE PROCEDUR MENU

void glowne_menu()
{//glowne menu operte na swtch-case
	int menu;
	cout << "----MENU GLOWNE----" << endl << endl;
	cout << "1. Plan reklamowy" << endl << endl;
	cout << "2. Cenniki" << endl << endl;
	cout << "3. Reklamodawcy" << endl << endl;
	cout << "4. Wyjscie" << endl << endl;
	menu = inty.check(1, 4);
	switch (menu)
	{
	case 1:
	{
		system("cls");
		plan_menu();

	}break;
	case 2:
	{
		system("cls");
		cennik_menu();
	}break;
	case 3:
	{
		system("cls");
		reklamodawcy_menu();
	}break;
	case 4:
	{
		system("cls");
		koniec = true;//warunek wyjscia z petli glownej programu
	}break;
	default:
	{
		system("cls");
		glowne_menu();
	}break;
	}
}

void cennik_menu()
{//menu cennikow
	fstream plik_cenniki;
	int menu;
	cout << "----MENU CENNIKI----" << endl << endl;
	cout << "1. Wyswietl wszystkie cenniki (szczegolowo, wszystkie dane)" << endl << endl;
	cout << "2. Wyswietl wszystkie cenniki (krotko, tylko numeracja i nazwy)" << endl << endl;
	cout << "3. Wybierz aktualny cennik" << endl << endl;
	cout << "4. Wyswietl aktualny cennik" << endl << endl;
	cout << "5. Dodaj nowy cennik" << endl << endl;
	cout << "6. Usun cennik" << endl << endl;
	cout << "7. Modyfikuj cennik (ceny)" << endl << endl;
	cout << "8. Modyfikuj cennik (mnozniki)" << endl << endl;
	cout << "9. Powrot do menu glownego" << endl << endl;
	menu = inty.check(1, 9);
	switch (menu)
	{
	case 1:
	{//wyswietlanie wszystkich cennikow po kolei
		system("cls");
		cenniki_odswiez(plik_cenniki);
		if (cenniki.size() > 0)
		{
			for (unsigned int i = 0; i < cenniki.size(); i++)
			{
				cout << "Lista cennikow" << endl << endl;
				cout << "Nr " << i + 1 << " z " << cenniki.size() << endl;
				cenniki[i].pokaz();
				system("pause");
				system("cls");
			}
		}
		else
			cout << "Brak cennikow" << endl << endl;
		cennik_menu();
	}break;
	case 2:
	{//wyswietlanie wszystkich cennikow po kolei, tylko nazwy wyswietlone
		system("cls");
		cenniki_odswiez(plik_cenniki);
		cout << "Lista cennikow" << endl << endl;
		if (cenniki.size() > 0)
		{
			for (unsigned int i = 0; i < cenniki.size(); i++)
			{
				cout << "Nr " << i + 1 << endl;
				cenniki[i].pokaz_tylko_nazwa();
			}
		}
		else
			cout << "Brak cennikow" << endl << endl;
		system("pause");
		system("cls");
		cennik_menu();
	}break;
	case 3:
	{//WYBOR AKTUALNEGO CEENIKA
		//aktualny cennik jest tym cennikiem, który ma index zerowy (jest pierwszy) w tablicy cennikow
		system("cls");
		cenniki_odswiez(plik_cenniki);
		cout << "Lista cennikow" << endl << endl;
		if (cenniki.size() > 0)
		{
			for (unsigned int i = 0; i < cenniki.size(); i++)
			{
				cout << "Nr " << i + 1 << endl;
				cenniki[i].pokaz_tylko_nazwa();
			}
			cout << endl << "Aktualny cennik" << endl << endl;
			cenniki[0].pokaz();
			cout << "Wybierz cennik aktualny, wpisz jego numer" << endl << endl;
			int nr;
			nr = inty.check(1, cenniki.size());
			iter_swap(cenniki.begin(), cenniki.begin() + nr - 1);//zamiana dotychczasowego aktualnego cennika z nowowybranym
			FILE *plik;
			fopen_s(&plik, "cenniki.txt", "w");//"w" - czyszczenie pliku
			fclose(plik);
			plik_cenniki.open("cenniki.txt");
			for (unsigned int i = 0; i < cenniki.size(); i++)
			{
				cenniki[i].zapis_do_pliku(plik_cenniki);//zapis nowym danych do pliku
			}
			plik_cenniki.close();
		}
		else
			cout << "Brak cennikow" << endl << endl;
		cout << "Nowy cennik aktualny zostal zapisany, za chwile nastapi powrot do menu cennikow";
		Sleep(2000);
		system("cls");
		cennik_menu();
	}break;
	case 4:
	{//wyswietla tylko aktulny cennik
		system("cls");
		cenniki_odswiez(plik_cenniki);
		cout << "Aktualny cennik" << endl << endl;
		if (cenniki.size() > 0)
		{
				cenniki[0].pokaz();
		}
		else
			cout << "Brak cennikow" << endl << endl;
		system("pause");
		system("cls");
		cennik_menu();
	}break;
	case 5:
	{//TEN CASE REALIZUJE DODAWANIE NOWEGO CENNIKA
		plik_cenniki.open("cenniki.txt", fstream::app);
		system("cls");
		string nazwa = "<nie podano>";
		double cena[48][2] = { 0 };
		double mnozniki[4] = { 0 };
		cenniki_pokaz(nazwa, cena, mnozniki);
		cout << "Podaj nazwe cennika" << endl << endl;
		cin >> nazwa;
		//plik_cenniki << nazwa << endl;
		system("cls");
		int godzina = 0;
		string minuta = "00";
		for (int i = 0; i < 48; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				cenniki_pokaz(nazwa, cena, mnozniki);
				cout << "Przedzial czasowy: " << godzina << ":" << minuta << " - ";
				if (minuta == "00")
				{
					minuta = "30";
				}
				else
				{
					godzina++;
					minuta = "00";
				}
				cout << godzina << ":" << minuta << endl;
				if (!j)
					cout << "Dni: Pon-Czw" << endl;
				else
					cout << "Dni: Pt-Niedz" << endl;
				cout << "Podaj cene:" << endl << endl;
				cena[i][j] = doubly.check(0, 1000000);
				//plik_cenniki << cena[i][j] << endl;
				if (!j)
				{
					if (minuta == "00")
					{
						minuta = "30";
						godzina--;
					}
					else
					{
						minuta = "00";
					}
				}
				system("cls");
			}
		}
		for (int i = 0; i < 4; i++)
		{
			cenniki_pokaz(nazwa, cena, mnozniki);
			cout << "Podaj mnoznik dla reklam " << (i + 1) * 10 << " sekundowych" << endl << endl;
			mnozniki[i] = doubly.check(0, 1000000);
			//plik_cenniki << mnozniki[i] << endl;
			system("cls");
		}
		//zapis do pliku
		plik_cenniki << nazwa << endl;
		for (int i = 0; i < 48; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				plik_cenniki << cena[i][j] << endl;
			}
		}
		for (int i = 0; i < 4; i++)
		{
			plik_cenniki << mnozniki[i] << endl;
		}
		cennik nowy(nazwa, cena, mnozniki);
		cenniki.push_back(nowy);
		plik_cenniki.close();
		cenniki_pokaz(nazwa, cena, mnozniki);
		cout << "Nowy cennik zostal zapisany, za chwile nastapi powrot do menu cennikow";
		Sleep(3000);
		system("cls");
		cennik_menu();
	}break;
	case 6:
	{
		//W TYM CASE'IE USUWA SIE WYBRANY Z LISTY CENNIK 
		system("cls");
		//1. ODSWIEŻANIE TABLICY - USUWANIE DOTYCHCZASOWEJ ZAWARTOŚCI TABLICY I WCZYTYWANIE NOWEJ Z PLIKU TEKSTOWEGO
		cenniki_odswiez(plik_cenniki);
		if (cenniki.size() > 0)
		{
			//2. WYSWIETLANIE LISTY REKLAMODAWCÓW
			for (unsigned int i = 0; i < cenniki.size(); i++)
			{
				cout << "Nr " << i + 1 << endl;
				cenniki[i].pokaz_tylko_nazwa();
			}
			//3. WYBÓR Z LISTY CENNIKA DO USUNIĘCIA
			cout << endl << "Wpisz nr cennika, ktory chcesz usunac" << endl << endl;
			int nr;
			nr = inty.check(1, cenniki.size());
			cenniki.erase(cenniki.begin() + nr - 1);
			system("cls");
			cout << "Wybrany cennik zostal usuniety, za chwile nastapi powrot do menu cennikow" << endl << endl;
			//4. USUWANIE DOTYCHCZASOWAEJ ZAWARTOŚCI PLIKU TEKSTOWEGO
			FILE *plik;
			fopen_s(&plik, "cenniki.txt", "w");
			fclose(plik);
			//5. ODŚWIEŻANIE ZAWARTOŚCI PLIKU TEKSTOWEGO ZMODYFIKOWANYMI DANYMI
			plik_cenniki.open("cenniki.txt");
			for (unsigned int i = 0; i < cenniki.size(); i++)
			{
				cenniki[i].zapis_do_pliku(plik_cenniki);
			}
			plik_cenniki.close();
		}
		else
			cout << "Brak cennikow, nie mozna nic usunac, za chwile nastapi powrot do menu cennikow" << endl << endl;
		Sleep(2000);
		system("cls");
		cennik_menu();
	}break;
	case 7:
	{//MODYFIKACJA CEN
		system("cls");
		cenniki_odswiez(plik_cenniki);
		cout << "Lista cennikow" << endl << endl;
		if (cenniki.size() > 0)
		{
			for (unsigned int i = 0; i < cenniki.size(); i++)
			{
				cout << "Nr " << i + 1 << endl;
				cenniki[i].pokaz_tylko_nazwa();
			}
		}
		else
		cout << "Brak cennikow" << endl << endl;
		cout << "Wpisz numer cennika, ktorego chcesz modyfikowac" << endl << endl;
		int nr;
		nr = inty.check(1, cenniki.size());
		system("cls");
		cout << "Nr " << nr << endl;
		cenniki[nr - 1].pokaz() ;
		cout << "Wpisz nr ID pasma, ktorego cene chcesz edytowac" << endl << endl;
		int pasmo;
		pasmo = inty.check(1, 48);
		system("cls");
		cout << "Nr " << nr << endl;
		cenniki[nr - 1].pokaz();
		cout << "Wpisz 1 by wybrac Pon-Czw lub 2 wybrac Pt-Niedz" << endl << endl;
		int dni;
		dni = inty.check(1, 2);
		system("cls");
		cout << "Nr " << nr << endl;
		cenniki[nr - 1].pokaz();
		cout << "Wpisz nowa cene" << endl << endl;
		double cena;
		cena = doubly.check(0, 1000000);
		system("cls");
		cenniki[nr - 1].modyfikuj_cena(pasmo, dni, cena);
		FILE *plik;
		fopen_s(&plik, "cenniki.txt", "w");
		fclose(plik);
		plik_cenniki.open("cenniki.txt");
		for (unsigned int i = 0; i < cenniki.size(); i++)
		{
			cenniki[i].zapis_do_pliku(plik_cenniki);
		}
		plik_cenniki.close();
		cout << "Zmiana zostala zapisana, za chwile nastapi powrot na menu cennikow" << endl << endl;
		cenniki[nr - 1].pokaz();
		cout << "Zmiana zostala zapisana, za chwile nastapi powrot na menu cennikow";
		Sleep(4000);
		system("cls");
		cennik_menu();

	}break;
	case 8:
	{//MODYFIKACJA MNOZNIKA
		system("cls");
		cenniki_odswiez(plik_cenniki);
		cout << "Lista cennikow" << endl << endl;
		if (cenniki.size() > 0)
		{
			for (unsigned int i = 0; i < cenniki.size(); i++)
			{
				cout << "Nr " << i + 1 << endl;
				cenniki[i].pokaz_tylko_nazwa();
			}
		}
		else
			cout << "Brak cennikow" << endl << endl;
		cout << "Wpisz numer cennika, ktorego chcesz modyfikowac" << endl << endl;
		int nr;
		nr = inty.check(1, cenniki.size());
		system("cls");
		cout << "Nr " << nr << endl;
		cenniki[nr - 1].pokaz();
		cout << "Wpisz nr ID mnoznika, ktorego wartosc chcesz edytowac" << endl << endl;
		int id;
		id = inty.check(1, 4);
		system("cls");
		cout << "Nr " << nr << endl;
		cenniki[nr - 1].pokaz();
		cout << "Wpisz nowa wartosc mnoznika" << endl << endl;
		double mnoznik;
		mnoznik = doubly.check(0, 1000000);
		system("cls");
		cenniki[nr - 1].modyfikuj_mnoznik(id, mnoznik);
		FILE *plik;
		fopen_s(&plik, "cenniki.txt", "w");
		fclose(plik);
		plik_cenniki.open("cenniki.txt");
		for (unsigned int i = 0; i < cenniki.size(); i++)
		{
			cenniki[i].zapis_do_pliku(plik_cenniki);
		}
		plik_cenniki.close();
		cout << "Zmiana zostala zapisana, za chwile nastapi powrot na menu cennikow" << endl << endl;
		cenniki[nr - 1].pokaz();
		cout << "Zmiana zostala zapisana, za chwile nastapi powrot na menu cennikow";
		Sleep(4000);
		system("cls");
		cennik_menu();
	}break;
	case 9:
	{
		system("cls");
		glowne_menu();
	}break;
	default:
	{
		system("cls");
		cennik_menu();
	}break;
	}
}

void plan_menu()
{
	fstream plik_cenniki;
	fstream plik_reklamodawcy;
	fstream plik_plany;
	int menu;
	cout << "----MENU PLAN----" << endl << endl;
	cout << "1. Stworz recznie nowy plan reklamowy" << endl << endl;
	cout << "2. Stworz automatycznie nowy plan reklamowy Metoda 1\n\tPrzydzial najdrozszy - wykorzystuje mozliwie caly budzet reklamodawcow" << endl << endl;
	cout << "3. Stworz automatycznie nowy plan reklamowy Metoda 2\n\tPrzydzial ekonomiczny - dla kazdego reklamodawcy okresla sie czesc budzetu do wykorzystania" << endl << endl;
	cout << "4. Wyswietl aktualna zajetosc planu reklamowego,\n\tDane dla calego tygodnia, podany tylko pozostaly czas wolny w danym pasmie" << endl << endl;
	cout << "5. Wyswietl aktualna zajetosc planu reklamowego,\n\tDane wybranego pasma w wybranym dniu, podane szczegolowe informacje" << endl << endl;
	cout << "6. Powrot do menu glownego" << endl << endl;
	menu = inty.check(1, 6);
	switch (menu)
	{
	case 1:
	{//tworzenie nowego planu recznie
		plik_plany.open("plany.txt");
		system("cls");
		cenniki_odswiez(plik_cenniki);
		reklamodawcy_odswiez(plik_reklamodawcy);
		string nazwa_planu = "<nie podano>";
		blok_reklamowy blok[48][7];
		cout << "Podaj nazwe planu" << endl << endl;
		cin >> nazwa_planu;
		system("cls");
		int z[48][7] = { 0 };
		for (unsigned int i = 0; i < reklamodawcy.size(); i++)//dla kazdego reklamodawcy
		{
			bool end = 0;
			string nazwa;
			double budzet;
			bool dlg_spotow[4];
			bool dni_tygodnia[2];
			bool godziny[8];
			reklamodawcy[i].podaj_dane(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
			while (!end)//dopoki nie skonczymy dodawac mu reklam
			{
				cout << "Nazwa planu: " << nazwa_planu << endl << endl;
				cout << "Liczba reklamodawcow: " << reklamodawcy.size() << endl << endl;
				cout << "Reklamodawca " << i + 1 << " z " << reklamodawcy.size() << endl << endl;
				string dni[7] = { "Poniedzialek", "Wtorek", "Sroda", "Czwartek", "Piatek", "Sobota", "Niedziela" };
				reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
				cout << "Czy chesz dodac reklame temu reklamodawcy?\nWpisz nr odpowiedzi\n\n1. Tak\n2. Nie, przejdz do kolejnego/Wyjdz" << endl << endl;
				int nr; 
				nr = inty.check(1, 2);
				if (nr == 1) end = false; else end = true;
				system("cls");
				if (!end)
				{
					
						reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
						cout << "Wybierz dzien reklamy, wpisz odpowiedni numer" << endl << endl;
						if (dni_tygodnia[0])
						{
							for (int i = 0; i < 4; i++)
							{
								cout << i + 1 << ". " << dni[i] << endl;
							}
						}
						if (dni_tygodnia[1])						//WYBIERANIE DNI TYGODNIA DOSTEPNYCH DLA DANEGO REKLAMODAWCY
						{
							for (int i = 4; i < 7; i++)
							{
								cout << i + 1 << ". " << dni[i] << endl;
							}
						}
						cout << endl;
						int dzien;
						dzien = inty.check(1, 7);
						system("cls");
						reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
						cout << "Wybierz pasmo reklamy" << endl << endl;
						int godzina = 0;
						string minuta = "00";
						int j = 0;
						for (int i = 0; i < 48; i++)
						{
							if (i % 6 == 0 && i > 0)
							{
								j++;
							}
							if (godziny[j])
							{
								cout << i + 1 << ". ";
								cout << godzina << ":" << minuta << " - ";
							}
							if (minuta == "00")
							{
								minuta = "30";
							}
							else
							{											//WYBIERANIE PASM DOSTEPNYCH DLA DANEGO REKLAMODAWCY
								godzina++;
								minuta = "00";
							}
							if (godziny[j])
							{
								cout << godzina << ":" << minuta << endl;
							}
						}
						cout << endl;
						int pasmo;
						pasmo = inty.check(1, 48);
						system("cls");
						reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
						cout << "Wybierz dlugosc reklamy, wpisz odpowiedni numer" << endl << endl;
						for (int i = 0; i < 4; i++)
						{
							if (dlg_spotow[i])
							{				//WYBIERANIE DLUGOSCI SPOTOW DLA DANEGO REKLAMODAWCY
								cout << i + 1 << ". " << (i + 1) * 10 << " sekundowe" << endl;
							}
						}
						cout << endl;
						int dlugosc;
						dlugosc = inty.check(1, 4);
						system("cls");
						reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
						cout << "Dzien: " << dzien << endl;
						cout << "Pasmo: " << pasmo << endl;
						cout << "Dlugosc spotu: " << dlugosc << endl << endl;
						double cena = cenniki[0].aktualna_cena(dzien, pasmo, dlugosc);//okreslenie ceny
						cout << "Cena: " << cena << endl;
						system("cls");
					
					if (cena > budzet)//brak srodkow na tę reklame, powrot
					{
						cout << "Brak srodkow" << endl << endl;
						cout << "Za chwile nastapi powrot";
						Sleep(2000);
						system("cls");
					}
					else
					{
						if (blok[pasmo - 1][dzien - 1].czas - dlugosc * 10 < 0)//brak czasu w tym pasmie, powrot
						{
							cout << "Brak czasu" << endl << endl;
							cout << "Za chwile nastapi powrot";
							Sleep(2000);
							system("cls");
						}
						else
						{
							budzet -= cena; //zmnijesz budzet reklamodawcy
							blok[pasmo - 1][dzien - 1].czas -= dlugosc * 10;
							blok[pasmo - 1][dzien - 1].reklamodawca[z[pasmo - 1][dzien - 1]] = nazwa;	//wpisywanie danych do bloku (czyli do planu)
							blok[pasmo - 1][dzien - 1].dlugosc[z[pasmo - 1][dzien - 1]] = dlugosc * 10;
							z[pasmo - 1][dzien - 1]++;	//ikrementacja liczby reklam w danym pasmie
						}
					}		
				}

			}
		}
		plan nowy(nazwa_planu, blok);
		plany.clear();
		plany.push_back(nowy);
		cout << "Wszystkie zmiany zostaly zapisane\nZa chwile nastapi powrot do menu planu";
		plany[0].zapis(plik_plany);	
		Sleep(2500);
		system("cls");
		plik_plany.close();
		plan_menu();

	}break;
	case 2:
	{ //proces analogiczny do dodawania recznego, pasma, dni i czasy reklam wybierane są losowo 
		//oczywiscie sposrod dostepnych dla danego reklamodawcy
		system("cls");
		plik_plany.open("plany.txt");
		cenniki_odswiez(plik_cenniki);
		reklamodawcy_odswiez(plik_reklamodawcy);
		string nazwa_planu = "<nie podano>";
		blok_reklamowy blok[48][7];
		cout << "Podaj nazwe planu" << endl << endl;
		cin >> nazwa_planu;
		system("cls");
		int z[48][7] = { 0 };
		for (unsigned int i = 0; i < reklamodawcy.size(); i++)
		{
			bool end = 0;
			string nazwa;
			double budzet;
			bool dlg_spotow[4];
			bool dni_tygodnia[2];
			bool godziny[8];
			reklamodawcy[i].podaj_dane(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
			while (!end)
			{
				vector <int> dni;
				if (dni_tygodnia[0])
				{
					for (int i = 0; i < 4; i++)
						dni.push_back(i + 1);
				}
				if (dni_tygodnia[1])
				{
					for (int i = 4; i < 7; i++)
						dni.push_back(i + 1);
				}
				int dzien = dni[rand() % dni.size()];//losowy wybor dnia z zakresu dni zgodnych
				dni.clear();
				vector <int> pasma;
				int j = 0;
				for (int i = 0; i < 48; i++)
				{
					if (i % 6 == 0 && i > 0)
					{
						j++;
					}
					if (godziny[j])
					{
						pasma.push_back(i + 1);
					}
				}
				int pasmo = pasma[rand() % pasma.size()];//losowy wybor pasma z zakresu pasm zgodnych
				pasma.clear();
				vector <int> dlugosci;
				for (int i = 0; i < 4; i++)
				{
					if (dlg_spotow[i])
					{
						dlugosci.push_back(i + 1);
					}
				}
				int dlugosc = dlugosci[rand() % dlugosci.size()];//losowy wybor dlugosci reklamy z zakresu dlugosci zgodnych
				dlugosci.clear();
				double cena = cenniki[0].aktualna_cena(dzien, pasmo, dlugosc);
				if (cena > budzet)
				{
					end = true;
				}
				else
				{
					if (blok[pasmo - 1][dzien - 1].czas - dlugosc * 10 < 0)
					{
						end = true;
					}
					else
					{
							budzet -= cena;
							blok[pasmo - 1][dzien - 1].czas -= dlugosc * 10;
							blok[pasmo - 1][dzien - 1].reklamodawca[z[pasmo - 1][dzien - 1]] = nazwa;
							blok[pasmo - 1][dzien - 1].dlugosc[z[pasmo - 1][dzien - 1]] = dlugosc * 10;
							z[pasmo - 1][dzien - 1]++;
					}
				}
			}
		}
		plan nowy(nazwa_planu, blok);
		plany.clear();
		plany.push_back(nowy);
		cout << "Stworzono nowy plan\nza chwile nastapi powrot do menu planu";
		plany[0].zapis(plik_plany);
		Sleep(1500);
		system("cls");
		plik_plany.close();
		plan_menu();
	}break;
	case 3:
	{//druga metoda automatyczna, dla kazdego reklamowacy mozna ustalic, jaka czesc jego budzeta moze zostac rozdysponowana
		system("cls");
		plik_plany.open("plany.txt");
		cenniki_odswiez(plik_cenniki);
		reklamodawcy_odswiez(plik_reklamodawcy);
		string nazwa_planu = "<nie podano>";
		blok_reklamowy blok[48][7];
		cout << "Podaj nazwe planu" << endl << endl;
		cin >> nazwa_planu;
		system("cls");
		int z[48][7] = { 0 };
		for (unsigned int i = 0; i < reklamodawcy.size(); i++)
		{
			bool end = 0;
			string nazwa;
			double budzet;
			bool dlg_spotow[4];
			bool dni_tygodnia[2];
			bool godziny[8];
			reklamodawcy[i].podaj_dane(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
			cout << "Nazwa planu: " << nazwa_planu << endl << endl;
			cout << "Liczba reklamodawcow: " << reklamodawcy.size() << endl << endl;
			cout << "Reklamodawca " << i + 1 << " z " << reklamodawcy.size() << endl << endl;
			cout << "Podaj jaka czesc budzetu tego reklamodawcy chcesz wydac" << endl << endl;
			double czesc_budzetu;
			cout << "1/";
			czesc_budzetu = doubly.check(0, 10000000);
			system("cls");
			budzet = budzet / czesc_budzetu;
			while (!end)
			{
				vector <int> dni;
				if (dni_tygodnia[0])
				{
					for (int i = 0; i < 4; i++)
						dni.push_back(i + 1);
				}
				if (dni_tygodnia[1])
				{
					for (int i = 4; i < 7; i++)
						dni.push_back(i + 1);
				}
				int dzien = dni[rand() % dni.size()];//losowy wybor dnia z zakresu dni zgodnych
				dni.clear();
				vector <int> pasma;
				int j = 0;
				for (int i = 0; i < 48; i++)
				{
					if (i % 6 == 0 && i > 0)
					{
						j++;
					}
					if (godziny[j])
					{
						pasma.push_back(i + 1);
					}
				}
				int pasmo = pasma[rand() % pasma.size()];//losowy wybor pasma z zakresu pasm zgodnych
				pasma.clear();
				vector <int> dlugosci;
				for (int i = 0; i < 4; i++)
				{
					if (dlg_spotow[i])
					{
						dlugosci.push_back(i + 1);
					}
				}
				int dlugosc = dlugosci[rand() % dlugosci.size()];//losowy wybor dlugosci reklamy z zakresu dlugosci zgodnych
				dlugosci.clear();
				double cena = cenniki[0].aktualna_cena(dzien, pasmo, dlugosc);
				if (cena > budzet)
				{
					end = true;
				}
				else
				{
					if (blok[pasmo - 1][dzien - 1].czas - dlugosc * 10 < 0)
					{
						end = true;
					}
					else
					{
						budzet -= cena;
						blok[pasmo - 1][dzien - 1].czas -= dlugosc * 10;
						blok[pasmo - 1][dzien - 1].reklamodawca[z[pasmo - 1][dzien - 1]] = nazwa;
						blok[pasmo - 1][dzien - 1].dlugosc[z[pasmo - 1][dzien - 1]] = dlugosc * 10;
						z[pasmo - 1][dzien - 1]++;
					}
				}
			}
			cout << "Dodano plan tego reklamodawcy";
			Sleep(1000);
			system("cls");
		}
		plan nowy(nazwa_planu, blok);
		plany.clear();
		plany.push_back(nowy);
		cout << "Stworzono nowy plan\nza chwile nastapi powrot do menu planu";
		plany[0].zapis(plik_plany);
		Sleep(2500);
		system("cls");
		plik_plany.close();
		plan_menu();
	}break;
	case 4:
	{//pokazywanie pozostalych czasow w planie
		system("cls");
		plany_odswiez(plik_plany);
		plany[0].pokaz();
		cout << endl << endl;
		system("pause");
		system("cls");
		plan_menu();
	}break;
	case 5:
	{//dokladne pokazanie jednego pasma
		system("cls");
		plany_odswiez(plik_plany);
		plany[0].pokaz_dokladnie();
		cout << endl << endl;
		system("pause");
		system("cls");
		plan_menu();
	}break;
	case 6:
	{
		system("cls");
		glowne_menu();
	}break;
	default:
	{
		system("cls");
		plan_menu();
	}break;
	}
}

void reklamodawcy_menu()
{
	int menu;
	fstream plik_reklamodawcy;
	cout << "----MENU REKLAMODAWCY----" << endl << endl;
	cout << "1. Lista reklamodawcow" << endl << endl;
	cout << "2. Dodaj reklamodawce" << endl << endl;
	cout << "3. Modyfikuj reklamodawce" << endl << endl;
	cout << "4. Usun reklamodawce" << endl << endl;
	cout << "5. Lista reklamodawcow wraz z aktualnym planem" << endl << endl;
	cout << "6. Laczny budzet reklamodawcow" << endl << endl;
	cout << "7. Powrot do menu glownego" << endl << endl;
	menu = inty.check(1, 7);
	switch (menu)
	{
	case 1:
	{
		//W TYM CASE'IE WYSWIETLANA JEST LISTA REKLAMODAWCOW ZAPISANYCH W PAMIECI
		///////////////////////////
		system("cls");
		//1. ODSWIEŻANIE TABLICY - USUWANIE DOTYCHCZASOWEJ ZAWARTOŚCI TABLICY I WCZYTYWANIE NOWEJ Z PLIKU TEKSTOWEGO
		reklamodawcy_odswiez(plik_reklamodawcy);
		//2. WYSWIETLANIE REKLAMODAWCOW, WYKORZYSTANIE PUBLICZNEJ METODY Z KLASY "REKLAMODAWCA"
		cout << "Lista reklamodawcow" << endl << endl;
		if (reklamodawcy.size() > 0)
		{
			for (unsigned int i = 0; i < reklamodawcy.size(); i++)
			{
				cout << "Nr " << i + 1 << endl;
				reklamodawcy[i].pokaz();
			}
		}
		else
			cout << "Brak reklamodawcow" << endl << endl;
		system("pause");
		system("cls");
		reklamodawcy_menu();
	}break;
	case 2:
	{
		//TEN CASE REALIZUJE OPERACJE DODAWANIA NOWEGO REKLAMODAWCY NA OSTATNNIE MIEJSCE TABLICY
		///////////////////////////////
		//1. OTWORZENIE PLIKU TEKSTOWEGO, ZCZYTYWANIE DANYCH Z KLAWIATURY I WPISYWANIE ICH DO PLIKU
		//		ROWNOCZESNE WYSWIETLANIE DANYCH NA EKRANIE (PROCEDURA REKLAMODAWCY_POKAZ) DLA PRZEJRZYSTOSCI WYKONYWANYCH DZIAŁAŃ 
		plik_reklamodawcy.open("reklamodawcy.txt", fstream::app);
		system("cls");
		string nazwa = "<nie podano>";
		double budzet = 0;
		bool dlg_spotow[4] = { 0, 0, 0, 0 };		//10, 20, 30, 40 sekund
		bool dni_tygodnia[2] = { 0, 0 }; // pon-czw i pt-niedz
		bool godziny[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };	//doba podzielona na 8 przedziałów po 3h każdy
		reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
		cout << "Podaj nazwe reklamodawcy" << endl << endl;
		cin >> nazwa;
		plik_reklamodawcy << nazwa << endl;
		system("cls");
		reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
		cout << "Podaj budzet reklamodawcy" << endl << endl;
		budzet = inty.check(0, 10000000);
		plik_reklamodawcy << budzet << endl;
		system("cls");
		for (int i = 0; i < 4; i++)
		{
			reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
			cout << "Wybierz dlugosci spotow, wcisnij 1 aby wybrac lub 0 aby odrzucic:" << endl << endl;
			cout << i + 1 << " z 4: Reklamy " << (i + 1) * 10 << " sekundowe (1/0): ";
			dlg_spotow[i] = booly.check();
			plik_reklamodawcy << dlg_spotow[i] << endl;
			system("cls");
		}
		reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
		cout << "Wybierz dni, w ktorych beda nadawane reklamy, wcisnij 1 aby wybrac lub 0 aby odrzucic:" << endl << endl;
		cout << "1 z 2: Poniedzialek - Czwartek: (1/0): ";
		dni_tygodnia[0] = booly.check();
		plik_reklamodawcy << dni_tygodnia[0] << endl;
		system("cls");
		reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
		cout << "Wybierz dni, w ktorych beda nadawane reklamy, wcisnij 1 aby wybrac lub 0 aby odrzucic:" << endl << endl;
		cout << "2 z 2: Piatek - Niedziela: (1/0): ";
		dni_tygodnia[1] = booly.check();
		plik_reklamodawcy << dni_tygodnia[1] << endl;
		system("cls");
		reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
		for (int i = 0; i < 8; i++)
		{
			cout << "Wybierz godziny, w ktorych beda nadawane reklamy, wcisnij 1 aby wybrac lub 0 aby odrzucic:" << endl << endl;
			cout << i + 1 << " z 8: Godziny: od " << (i + 1) * 3 - 3 << " do " << (i + 1) * 3 << " (1/0): ";
			godziny[i] = booly.check();
			plik_reklamodawcy << godziny[i] << endl;
			system("cls");
			reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
		}
		//2. DODANIE NOWEGO ELEMENTU DO TABLICY, ORAZ ZAMKNIECIE PLIKU TEKSTOWEGO
		reklamodawca nowy(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
		reklamodawcy.push_back(nowy);
		plik_reklamodawcy.close();
		cout << "Wszystkie dane zostaly wprowadzone i zapisane, za chwile nastapi powrot do menu reklamodawcow";
		Sleep(2000);
		system("cls");
		reklamodawcy_menu();
	}break;
	case 3:
	{
		//TEN CASE UMOŻLIWIA MODYFIKACJE WSZYstKICH DANYCH JEDNEGO, WYBRANEGO Z LISTY REKLAMODAWCY
		///////////////////////////////////
		system("cls");
		//1. ODSWIEŻANIE TABLICY - USUWANIE DOTYCHCZASOWEJ ZAWARTOŚCI TABLICY I WCZYTYWANIE NOWEJ Z PLIKU TEKSTOWEGO	
		reklamodawcy_odswiez(plik_reklamodawcy);
		//2. WYBOR REKLAMODAWCY, KTORY BEDZIE MODYFIKOWANY
		if (reklamodawcy.size() > 0)
		{
			for (unsigned int i = 0; i < reklamodawcy.size(); i++)
			{
				cout << "Nr " << i + 1 << endl;
				reklamodawcy[i].pokaz();
			}
			cout << endl << "Wpisz nr reklamodawcy, ktorego chcesz modyfikowac" << endl << endl;
			int nr;
			nr = inty.check(1, reklamodawcy.size());
			system("cls");
		//3. WPISYWANIE NOWYCH DANYCH MODYFIKOWANEGO REKLAMODAWCY
		//		PROCEDURA PODOBNA DO DODAWANIA NOWEGO REKLAMODAWCY, ALE BEZ ZAPISYWANIA NA TYM ETAPIE DANYCH DO PLIKU TEKSTOWEGO
			string nazwa = "<nie podano>";
			double budzet = 0;
			bool dlg_spotow[4] = { 0, 0, 0, 0 };		//10, 20, 30, 40 sekund
			bool dni_tygodnia[2] = { 0, 0 }; // pon-czw i pt-niedz
			bool godziny[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };	//doba podzielona na 8 przedziałów po 3h każdy
			reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
			cout <<  "Podaj nowa nazwe reklamodawcy" << endl << endl;
			cin >> nazwa;
			system("cls");
			reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
			cout << "Podaj nowy budzet reklamodawcy" << endl << endl;
			budzet = doubly.check(0, 10000000);
			system("cls");
			for (int i = 0; i < 4; i++)
			{
				reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
				cout << "Wybierz nowe dlugosci spotow, wcisnij 1 aby wybrac lub 0 aby odrzucic:" << endl << endl;
				cout << i + 1 << " z 4: Reklamy " << (i + 1) * 10 << " sekundowe (1/0): ";
				dlg_spotow[i] = booly.check();
				system("cls");
			}
			reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
			cout << "Wybierz nowe dni, w ktorych beda nadawane reklamy, wcisnij 1 aby wybrac lub 0 aby odrzucic:" << endl << endl;
			cout << "1 z 2: Poniedzialek - Piatek: (1/0): ";
			dni_tygodnia[0] = booly.check();
			system("cls");
			reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
			cout << "Wybierz nowe dni, w ktorych beda nadawane reklamy, wcisnij 1 aby wybrac lub 0 aby odrzucic:" << endl << endl;
			cout << "2 z 2: Sobota - Niedziela: (1/0): ";
			dni_tygodnia[1] = booly.check();
			system("cls");
			for (int i = 0; i < 8; i++)
			{
				reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
				cout << "Wybierz nowe godziny, w ktorych beda nadawane reklamy, wcisnij 1 aby wybrac lub 0 aby odrzucic:" << endl << endl;
				cout << i + 1 << " z 8: Godziny: od " << (i + 1) * 3 - 3 << " do " << (i + 1) * 3 << " (1/0): ";
				godziny[i] = booly.check();
				system("cls");
			}
		//4. WŁAŚCIWA MODYFIKACJA REKLAMODAWCY Z WCZEŚNIEJ POBRANYMI WARTOŚCIAMI
			reklamodawcy[nr - 1].modyfikuj(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
			reklamodawcy_pokaz(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
			cout << "Wybrany reklamodawca zostal zmodyfikowany, za chwile nastapi powrot do menu reklamodawcow" << endl << endl;
		//5. USUWANIE DOTYCHCZASOWEJ ZAWARTOSCI PLIKU TEKSTOWEGO
			FILE *plik;
			fopen_s(&plik, "reklamodawcy.txt", "w");
			fclose(plik);
		//6. AKTUALIZOWANIE PLIKU TEKSTOWEGO ZMODYFIKOWANYMY DANYMI
			plik_reklamodawcy.open("reklamodawcy.txt");
			for (unsigned int i = 0; i < reklamodawcy.size(); i++)
			{
				reklamodawcy[i].zapis_do_pliku(plik_reklamodawcy);
			}
			plik_reklamodawcy.close();
		}
		else
			cout << "Brak reklamodawcow, nie mozna nic modyfikowac, za chwile nastapi powrot do menu reklamodawcow" << endl << endl;
		Sleep(2000);
		system("cls");
		reklamodawcy_menu();
	}break;
	case 4:
	{	//W TYM CASE'IE USUWA SIE WYBRANEGO Z LISTY REKLAMODAWCE 
		//////////////////////////////////
		system("cls");
		//1. ODSWIEŻANIE TABLICY - USUWANIE DOTYCHCZASOWEJ ZAWARTOŚCI TABLICY I WCZYTYWANIE NOWEJ Z PLIKU TEKSTOWEGO
		reklamodawcy_odswiez(plik_reklamodawcy);
		if (reklamodawcy.size() > 0)
		{
		//2. WYSWIETLANIE LISTY REKLAMODAWCÓW
			for (unsigned int i = 0; i < reklamodawcy.size(); i++)
			{
				cout << "Nr " << i + 1 << endl;
				reklamodawcy[i].pokaz();
			}
		//3. WYBÓR Z LISTY REKLAMODAWCY DO USUNIĘCIA
			cout << endl << "Wpisz nr reklamodawcy, ktorego chcesz usunac" << endl << endl;
			int nr;
			nr = inty.check(1, reklamodawcy.size());
			reklamodawcy.erase(reklamodawcy.begin() + nr - 1);
			system("cls");
			cout <<  "Wybrany reklamodawca zostal usuniety, za chwile nastapi powrot do menu reklamodawcow" << endl << endl;
		//4. USUWANIE DOTYCHCZASOWAEJ ZAWARTOŚCI PLIKU TEKSTOWEGO
			FILE *plik;
			fopen_s(&plik, "reklamodawcy.txt", "w");
			fclose(plik);
		//5. ODŚWIEŻANIE ZAWARTOŚCI PLIKU TEKSTOWEGO ZMODYFIKOWANYMI DANYMI
			plik_reklamodawcy.open("reklamodawcy.txt");
				for (unsigned int i = 0; i < reklamodawcy.size(); i++)
				{
					reklamodawcy[i].zapis_do_pliku(plik_reklamodawcy);
				}
			plik_reklamodawcy.close();			
		}
		else
			cout << "Brak reklamodawcow, nie mozna nic usunac, za chwile nastapi powrot do menu reklamodawcow" << endl << endl;
		Sleep(2000);
		system("cls");
		reklamodawcy_menu();
	}break;
	case 5:
	{//wyswietlanie reklam kolejncyh reklamodawcow
		system("cls");
		fstream plik_plany;
		plany_odswiez(plik_plany);
		reklamodawcy_odswiez(plik_reklamodawcy);
		for (unsigned int i = 0; i < reklamodawcy.size(); i++)
		{
			cout << "Reklamodawca " << i + 1 << " z " << reklamodawcy.size();
			reklamodawcy[i].pokaz();
			cout << "Reklamy tego reklamodawcy" << endl << endl;
			string nazwa;
			reklamodawcy[i].podaj_nazwe(nazwa);
			plany[0].reklamy(nazwa);
			cout << endl <<"Reklamy tego reklamodawcy" << endl << endl;
			cout << "Reklamodawca " << i + 1 << " z " << reklamodawcy.size();
			reklamodawcy[i].pokaz();
			cout << endl << endl;
			system("pause");
			system("cls");
		}
		system("cls");
		reklamodawcy_menu();
	}break;
	case 6:
	{
		system("cls");
		reklamodawcy_odswiez(plik_reklamodawcy);
		for (unsigned int i = 1; i < reklamodawcy.size(); i++)
		{
			reklamodawcy[0] = reklamodawcy[0] + reklamodawcy[i];
		}
		cout << "Laczny budzet reklamodawcow wynosi: ";
		reklamodawcy[0].pokaz_budzet();
		system("pause");
		system("cls");
		reklamodawcy_menu();
	}break;
	case 7:
	{
		system("cls");
		glowne_menu();
	}break;
	default:
	{
		system("cls");
		reklamodawcy_menu();
	}break;
	}
}

//DEFINICJE POZOSTAŁYCH PROCEDUR

void reklamodawcy_pokaz(string nazwa, double budzet, bool dlg_spotow[4], bool dni_tygodnia[2], bool godziny[8])
{//TEN VOID WYSWIETLA ŁADNIE SFORMATOWANE DANE PODANE JAKO ARGUMENTY TEGO VOIDA
	cout << "Nazwa reklamodawcy: " << nazwa << endl << "Budzet reklamodawcy: " << budzet << endl;
	cout << "Wybrane dlugosci spotow (w sekundach):\t";
	for (int i = 0; i < 4; i++)
		if (dlg_spotow[i])
			cout << (i + 1) * 10 << "  ";
	cout << endl << "Wybrane dni:\t";
	for (int i = 0; i < 2; i++)
		if (dni_tygodnia[i])
			if (!i)
				cout << "Poniedzialek - Czwartek  ";
			else
				cout << "Piatek - Niedziela  ";
	cout << endl << "Wybrane godziny nadawania:\t";
	for (int i = 0; i < 8; i++)
		if (godziny[i])
			cout << (i + 1) * 3 - 3 << "-" << (i + 1) * 3 << "  ";
	cout << endl << endl;
}

void reklamodawcy_odswiez(fstream& plik_reklamodawcy)
{//wczytuje dane z pliku do obiektów klasy reklamodawcy, wczcesniej usuwa poprzednie obikety
	plik_reklamodawcy.open("reklamodawcy.txt");
	string nazwa;
	double budzet;
	bool dlg_spotow[4];		//10, 20, 30, 40 sekund
	bool dni_tygodnia[2]; // pon-czw i pt-niedz
	bool godziny[8];	//doba podzielona na 8 przedziałów po 3h każdy
	reklamodawcy.clear();
	while (!plik_reklamodawcy.eof())
	{
		plik_reklamodawcy >> nazwa;
		plik_reklamodawcy >> budzet;
		for (int i = 0; i < 4; i++)
			plik_reklamodawcy >> dlg_spotow[i];
		for (int i = 0; i < 2; i++)
			plik_reklamodawcy >> dni_tygodnia[i];
		for (int i = 0; i < 8; i++)
			plik_reklamodawcy >> godziny[i];
		reklamodawca nowy(nazwa, budzet, dlg_spotow, dni_tygodnia, godziny);
		reklamodawcy.push_back(nowy);
	}
	reklamodawcy.pop_back();
	plik_reklamodawcy.close();
}

void cenniki_pokaz(string nazwa, double cena[48][2], double mnozniki[4])
{//TEN VOID WYSWIETLA ŁADNIE SFORMATOWANE DANE PODANE JAKO ARGUMENTY TEGO VOIDA
	cout << "Nazwa cennika: " << nazwa << endl << endl;
	cout << "Pasmo\t\t\t";
	cout << "Pon-Czw" << "\t\t" << "Pt-Niedz" << endl;
	int godzina = 0;
	string minuta = "00";
	for (int i = 0; i < 48; i++)//48 przedziałów po 30 minut
	{
		cout << godzina << ":" << minuta << " - ";
		if (minuta == "00")
		{
			minuta = "30";
		}
		else
		{
			godzina++;
			minuta = "00";
		}
		cout << godzina << ":" << minuta << "\t\t";
		for (int j = 0; j < 2; j++)//tydzien podzielony na 2 grupy dni
		{
			cout << cena[i][j] << "\t\t";
		}
		cout << endl;
	}
	cout << "Mnozniki" << endl;
	for (int i = 0; i < 4; i++)
	{
		cout << (i + 1) * 10 << " sekund - ";
		cout << mnozniki[i] << endl;
	}
	cout << endl << endl;
}

void cenniki_odswiez(fstream& plik_cenniki)
{//wczytuje dane z pliku do obiektów klasy cenniki, wczcesniej usuwa poprzednie obiekty
	plik_cenniki.open("cenniki.txt");
	string nazwa;
	double cena[48][2];
	double mnozniki[4];
	cenniki.clear();
	while (!plik_cenniki.eof())
	{
		plik_cenniki >> nazwa;
		for (int i = 0; i < 48; i++)
			for (int j = 0; j < 2; j++)
				plik_cenniki >> cena[i][j];
		for (int i = 0; i < 4; i++)
			plik_cenniki >> mnozniki[i];
		cennik nowy(nazwa, cena, mnozniki);
		cenniki.push_back(nowy);
	}
	cenniki.pop_back();
	plik_cenniki.close();
}
void plany_odswiez(fstream& plik_plany)
{//wczytuje dane z pliku do obiektu klasy cenniki, wczcesniej usuwa poprzedni obiekt
	plik_plany.open("plany.txt");
	
	string nazwa;
	blok_reklamowy blok[48][7];
	
	plany.clear();
	plik_plany >> nazwa;
	for (int i = 0; i < 48; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			blok[i][j].czysc();
			plik_plany>>blok[i][j].czas;
			for (int k = 0; k < 36; k++)
			{
				plik_plany >> blok[i][j].reklamodawca[k];
				plik_plany >> blok[i][j].dlugosc[k];
			}
		}
	}
	plan nowy(nazwa, blok);
	plany.push_back(nowy);
	plik_plany.close();
}
int main()
{
	srand((unsigned int)time(NULL));
	koniec = false;
	while(!koniec)	glowne_menu();
	return 0;
}