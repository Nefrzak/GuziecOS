/* Tomasz Braczyñski */

//System plików GuziecOS
#pragma once
#include <iostream>
#include <cstdlib>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
using namespace std;
/*
W superbloku (pierwszy sektor):
1-8 bajt przechowuja dane o wolnych sektorach, 1 sektor zawsze zajety
*/
void dectobin(int liczba, int bity[])
{
	int wsk = 7;
	while (liczba)
	{
		bity[wsk] = liczba % 2;
		liczba /= 2;
		wsk--;
	}

}
int bintodec(int bity[8])
{
	int suma = 0;
	for (int i = 7, mnoznik = 1; i >= 0; i--, mnoznik *= 2)
	{
		suma += bity[i] * mnoznik;
	}
	return suma;
}

class SEKTOR
{
public:
	unsigned char dane[16];
	void wyswietl_liczbowo()
	{
		for (int i = 0; i < 16; i++)
		{
			cout << setw(5) << (int)dane[i] << " ";
		}
		cout << endl;
	}
	void wyswietl_znakowo()
	{
		for (int i = 0; i < 16; i++)
		{
			cout << setw(5) << dane[i] << " ";
		}
		cout << endl;
	}
	void wyczysc()
	{
		for (int i = 0; i < 16; i++)
		{
			dane[i] = 0;
		}
	}
	void wpisz_plik_do_folderu(int numer_iwezla)
	{
		for (int i = 4; i < 16; i++)
		{
			if (dane[i] == 0)
			{
				dane[i] = (char)numer_iwezla;
				break;
			}
		}
	}
	bool mozna_wpisac_plik()
	{
		for (int i = 4; i < 16; i++)
		{
			if (dane[i] == 0)
			{
				return true;
			}
		}
		return false;
	}
	void wypisz_plik_z_folderu(int numer_iwezla)
	{
		for (int i = 4; i < 16; i++)
		{
			if (dane[i] == (char)numer_iwezla)
			{
				dane[i] = (char)0;
				break;
			}
		}
	}
	void wyswietl_wlasciwosci()
	{
		if (dane[0] == 255)//plik
		{
			cout << (char)192 << (char)196 << (char)196 << (char)196 << "Plik \"" << dane[1] << dane[2] << dane[3] << "\", rozmiar " << (int)dane[4] << "B" << endl;
		}
		else if (dane[0] == 254)//folder
		{
			cout << (char)195 << (char)196 << (char)196 << (char)196 << "Folder \"" << dane[1] << dane[2] << dane[3] << "\"" << endl;
		}
		else if (dane[0] == 253)//folder
		{
			cout << (char)195 << (char)196 << (char)196 << (char)196 << "Plik komunikacyjny \"" << dane[1] << dane[2] << dane[3] << "\" o stalym rozmiarze 16B" << endl;
		}
		else//dane lub pusty sektor
		{
			if (dane[0] == 0)
			{
				cout << "Sektor jest pusty" << endl;
			}
			else
			{
				cout << "Dane zapisane w tym sektorze : ";
				for (int i = 0; i < 16; i++, cout << dane[i]);
				cout << endl;
			}
		}
	}
	void wypelnij(string tresc)
	{
		for (int i = 0; i < tresc.size(); i++)
		{
			dane[i] = tresc[i];
		}
	}
};
class DYSK
{
public:
	SEKTOR sektory_na_dysku[64];
	int wolne_miejsce;
	DYSK()
	{
		for (int i = 0; i < 64; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				if (i == 0 && j == 0)
				{
					//zaznaczenie zajetosci pierwszego sektora - superbloku, drugiego - plik komunikacji, trzeciego - sektor na dane pliku komunikacji
					sektory_na_dysku[i].dane[j] = (char)224;
				}
				else if (i == 1)
				{
					char wlasciwosci_pliku[16] = { (char)253,'K','O','M', 2,0,0,0,0,0,0,0,0,0,0,0 };
					sektory_na_dysku[i].dane[j] = wlasciwosci_pliku[j];
					//wlasciwosci_pliku={typ, nazwa[3], blok posredni=2}
					//typ 253 to plik komunikacyjny
				}
				else
				{
					sektory_na_dysku[i].dane[j] = (char)0;
				}
			}
		}
		przelicz_wolne_miejsce();
	}
	void przelicz_wolne_miejsce()
	{
		int licznik = 0;
		for (int i = 0; i < 64; i++)
		{
			if (dostepnosc_sektora(i))
			{
				licznik++;
			}
		}
		wolne_miejsce = 16 * licznik;
	}
	bool dostepnosc_sektora(int numer_sektora)
	{
		int do_sprawdzenia = sektory_na_dysku[0].dane[numer_sektora / 8];
		int bity[8] = {};
		dectobin(do_sprawdzenia, bity);
		if (bity[numer_sektora % 8] == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	int szukaj_wolnego_sektora()
	{
		int wolny = -1;
		for (int i = 0; i < 64; i++)
		{
			if (dostepnosc_sektora(i))
			{
				wolny = i;
				break;
			}
		}
		return wolny;
	}
	void wyswietl_sektor_znakowo(int numer)
	{
		if (dostepnosc_sektora(numer))
		{
			cout << "[W]";
		}
		else
		{
			cout << "[Z]";
		}
		cout << "sektor nr" << setw(2) << numer << ": ";
		sektory_na_dysku[numer].wyswietl_znakowo();
	}
	void wyswietl_sektor_liczbowo(int numer)
	{
		if (dostepnosc_sektora(numer))
		{
			cout << "[W]";
		}
		else
		{
			cout << "[Z]";
		}
		cout << "sektor nr" << setw(2) << numer << ":\t";
		sektory_na_dysku[numer].wyswietl_liczbowo();
	}
	void zmien_dostepnosc_sektora(int numer, bool zajety)
	{
		int temp = sektory_na_dysku[0].dane[numer / 8], bity[8] = {};
		dectobin(temp, bity);
		if (zajety)
		{
			bity[numer % 8] = 1;
		}
		else
		{
			bity[numer % 8] = 0;
		}
		temp = bintodec(bity);
		sektory_na_dysku[0].dane[numer / 8] = temp;
	}
	int szukaj_sektora_po_nazwie(string nazwa)
	{
		int sektor = 0;
		for (int i = 1; i <= 64; i++)
		{
			if (sektory_na_dysku[i].dane[1] == nazwa[0] && sektory_na_dysku[i].dane[2] == nazwa[1] && sektory_na_dysku[i].dane[3] == nazwa[2])
			{
				if (sektory_na_dysku[i].dane[0] == 255 || sektory_na_dysku[i].dane[0] == 254 || sektory_na_dysku[i].dane[0] == 253)
				{
					sektor = i;
					break;
				}
			}
		}
		return sektor;//moze zwrocic 0 gdy nie znaleziono pliku/folderu
	}
	void stworz_plik(string nazwa, string sciezka = "")
	{
		if (szukaj_sektora_po_nazwie(nazwa) != 0)
		{
			cout << "Podana nazwa juz istnieje. Plik nie zostal utworzony" << endl;
		}
		else if (nazwa.size() == 3)
		{
			int sektor_na_plik = szukaj_wolnego_sektora(), sciezka_sektor = 0;
			if (sektor_na_plik == -1)
			{
				cout << "Brak wolnych sektorow na dysku. Plik nie zostal stworzony." << endl;
			}
			else
			{
				if (sciezka.size() != 0)
				{
					sciezka_sektor = szukaj_sektora_po_nazwie(sciezka);
					if (sciezka_sektor == 0)
					{
						cout << "Nie znaleziono folderu. Plik zostanie utworzony w glownym katalogu." << endl;
					}
					if (sektory_na_dysku[sciezka_sektor].mozna_wpisac_plik() == false)
					{
						cout << "W folderze znajduje sie maksymalna ilosc plikow. Plik zostanie utworzony w glownym katalogu" << endl;
						sciezka_sektor = 0;
					}
				}
				char wlasciwosci_pliku[16] = { (char)255,nazwa[0],nazwa[1],nazwa[2], 0,(char)sciezka_sektor };
				//wlasciwosci_pliku={typ, nazwa, rozmiar, sciezka, 4xbloki bezposrednie, blok posredni}
				//typ 255 to plik
				//sciezka = 0 kalatog glowny
				//odwolanie do nieistniejacego bloku np.64 = nieuzyty blok
				memcpy(sektory_na_dysku[sektor_na_plik].dane, wlasciwosci_pliku, 11);
				zmien_dostepnosc_sektora(sektor_na_plik, true);
				cout << "Utworzono i-wezel pliku \"" << nazwa << "\" w sektorze " << sektor_na_plik << " w katalogu ";
				if (sciezka_sektor == 0)
				{
					cout << "glownym." << endl;
				}
				else
				{
					cout << "o nazwie \"" << sektory_na_dysku[sciezka_sektor].dane[1] << sektory_na_dysku[sciezka_sektor].dane[2] << sektory_na_dysku[sciezka_sektor].dane[3] << "\"." << endl;
					sektory_na_dysku[sciezka_sektor].wpisz_plik_do_folderu(sektor_na_plik);
				}
			}
		}
		else
		{
			cout << "Nazwa zbyt dluga lub zbyt krotka. Plik nie zostal utworzony." << endl;
		}
		przelicz_wolne_miejsce();
	}
	void wypelnij_plik_danymi(string nazwa, string sciezka_na_windowsie)//w sciezce pliku nalezy uzywac podwojnych / np.: C://asd.txt
	{
		int sektor_pliku = szukaj_sektora_po_nazwie(nazwa);
		ifstream wejscie(sciezka_na_windowsie);
		string dane_z_pliku;
		getline(wejscie, dane_z_pliku);
		int rozmiar = (int)dane_z_pliku.size();
		if (sektor_pliku == 1 || nazwa == "KOM")
		{
			if (sciezka_na_windowsie.size() > 16)
			{
				cout << "Zbyt wiele informacji na plik komunikacyjny." << endl;
			}
			else
			{
				for (int i = 0; i < sciezka_na_windowsie.size(); i++)
				{
					sektory_na_dysku[2].dane[i] = sciezka_na_windowsie[i];
				}
			}
		}
		else if (rozmiar == 0)
		{
			cout << "Nie znaleziono pliku lub pliku plik jest pusty." << endl;
		}
		else if (rozmiar > 240)
		{
			cout << "Rozmiar pliku zbyt duzy. GuziecOS przyjmuje pliki do 240 bajtow." << endl;
		}
		else if ((sektor_pliku == 0)&&(sektory_na_dysku[sektor_pliku].dane[0] != 253|| sektory_na_dysku[sektor_pliku].dane[0]!=255))
		{
			cout << "Nie mozna zaladowac informacji, bo plik o podanej nazwie nie istnieje." << endl;
		}
		else
		{
			przelicz_wolne_miejsce();
			if (sektory_na_dysku[sektor_pliku].dane[4] != 0)
			{
				cout << "Plik jest juz wypelniony danymi." << endl;
			}
			else if (wolne_miejsce > rozmiar)
			{
				int start, temp;
				sektory_na_dysku[sektor_pliku].dane[4] = rozmiar;
				cout << "Zaladowno dane do pliku \"" << sektory_na_dysku[sektor_pliku].dane[1] << sektory_na_dysku[sektor_pliku].dane[2] << sektory_na_dysku[sektor_pliku].dane[3] << "\"." << endl;
				if (rozmiar < 64)
				{
					start = 6;
					for (int i = 0; i < ceil((double)rozmiar / 16); i++)
					{
						temp = szukaj_wolnego_sektora();
						string czesc_danych = dane_z_pliku.substr(i * 16, 16);
						sektory_na_dysku[temp].wypelnij(czesc_danych);
						zmien_dostepnosc_sektora(temp, true);
						sektory_na_dysku[sektor_pliku].dane[start] = temp;
						start++;
					}
				}
				else
				{
					start = 6;
					for (int i = 0; i < ceil((double)rozmiar / 16) && i < 4; i++)
					{
						temp = szukaj_wolnego_sektora();
						string czesc_danych = dane_z_pliku.substr(i * 16, 16);
						sektory_na_dysku[temp].wypelnij(czesc_danych);
						zmien_dostepnosc_sektora(temp, true);
						sektory_na_dysku[sektor_pliku].dane[start++] = temp;
					}
					sektory_na_dysku[sektor_pliku].dane[10] = szukaj_wolnego_sektora();
					sektor_pliku = sektory_na_dysku[sektor_pliku].dane[10];
					start = 0;
					zmien_dostepnosc_sektora(sektor_pliku, true);
					for (int i = 0; i < ceil((double)rozmiar / 16) && i < 11; i++)
					{
						int temp = szukaj_wolnego_sektora();
						string czesc_danych = dane_z_pliku.substr((4 + i) * 16, 16);
						sektory_na_dysku[temp].wypelnij(czesc_danych);
						zmien_dostepnosc_sektora(temp, true);
						sektory_na_dysku[sektor_pliku].dane[start++] = temp;
						if ((4 + i + 1) * 16 >= rozmiar)
						{
							break;
						}
					}
				}
			}
			else
			{
				cout << "Na dysku nie ma wystarczajacej ilosci wolnej pamieci do wczytania danych." << endl;
			}
		}
		przelicz_wolne_miejsce();
	}
	void stworz_folder(string nazwa)
	{
		if (szukaj_sektora_po_nazwie(nazwa) != 0)
		{
			cout << "Podana nazwa juz istnieje. Folder nie zostal utworzony." << endl;
		}
		else if (nazwa.size() == 3)
		{
			int sektor_na_folder = szukaj_wolnego_sektora();
			if (sektor_na_folder == -1)
			{
				cout << "Brak wolnych sektorow na dysku. Folder nie zostal utworzony." << endl;
			}
			else
			{
				char wlasciwosci_folderu[16] = { (char)254,nazwa[0],nazwa[1],nazwa[2] };
				//typ 254 oznacza folder
				memcpy(sektory_na_dysku[sektor_na_folder].dane, wlasciwosci_folderu, 4);
				zmien_dostepnosc_sektora(sektor_na_folder, true);
				cout << "Utworzono i-wezel folderu \"" << nazwa << "\" w sektorze: " << sektor_na_folder << endl;
			}
		}
		else
		{
			cout << "Nazwa zbyt dluga lub zbyt krotka. Folder nie zostal utworzony." << endl;
		}
		przelicz_wolne_miejsce();
	}
	void usun(string nazwa)
	{
		int sektor_do_usuniecia = szukaj_sektora_po_nazwie(nazwa);
		if (sektory_na_dysku[sektor_do_usuniecia].dane[0] == 253||nazwa == "KOM")
		{
			cout << "Nie mozna usunac pliku komunikacyjnego." << endl;
		}
		else if (sektory_na_dysku[sektor_do_usuniecia].dane[0] == 254)
		{
			usun_folder(sektor_do_usuniecia);
		}
		else if (sektory_na_dysku[sektor_do_usuniecia].dane[0] == 255)
		{
			usun_plik(sektor_do_usuniecia);
		}
	}
	void usun_plik(int sektor_do_usuniecia)//numer sektora, w ktorym znajduje sie iwezel pliku
	{
		if (sektor_do_usuniecia == 0 || sektor_do_usuniecia == 1 || sektor_do_usuniecia == 2 || sektory_na_dysku[sektor_do_usuniecia].dane[0] != 255)
		{
			cout << "Blad usuwania pliku." << endl;//nie ma pliku o takiej nazwie (stad 0) lub sektor nie jest plikiem
		}
		else
		{
			cout << "Plik \"" << sektory_na_dysku[sektor_do_usuniecia].dane[1] << sektory_na_dysku[sektor_do_usuniecia].dane[2] << sektory_na_dysku[sektor_do_usuniecia].dane[3] << "\" zostal usuniety." << endl;
			int temp = sektory_na_dysku[sektor_do_usuniecia].dane[10];//odnosnik do bloku posredniego
			if (temp != 0)
			{
				for (int i = 0; i < 16; i++)
				{
					if (sektory_na_dysku[temp].dane[i] != 0)
					{
						sektory_na_dysku[sektory_na_dysku[temp].dane[i]].wyczysc();//czyszczenie blokow posrednich z danymi
						zmien_dostepnosc_sektora(sektory_na_dysku[temp].dane[i], false);
					}
				}
				sektory_na_dysku[temp].wyczysc();
				zmien_dostepnosc_sektora(temp, false);
			}
			for (int i = 6; i < 10; i++)
			{
				if (sektory_na_dysku[sektor_do_usuniecia].dane[i] != 0)
				{
					sektory_na_dysku[sektory_na_dysku[sektor_do_usuniecia].dane[i]].wyczysc();//czyszczenie blokow bezposrednich z danymi
					zmien_dostepnosc_sektora(sektory_na_dysku[sektor_do_usuniecia].dane[i], false);
				}
			}
			if (sektory_na_dysku[sektor_do_usuniecia].dane[5] != 0)//sprawdzenie czy plik jest w folderze czy w katalogu glownym
			{
				sektory_na_dysku[sektory_na_dysku[sektor_do_usuniecia].dane[5]].wypisz_plik_z_folderu(sektor_do_usuniecia);
			}
			sektory_na_dysku[sektor_do_usuniecia].wyczysc();//czyszczenie iwezla pliku
			zmien_dostepnosc_sektora(sektor_do_usuniecia, false);
		}
		przelicz_wolne_miejsce();
	}
	void usun_folder(int sektor_do_usuniecia)
	{
		if (sektor_do_usuniecia == 0 || sektor_do_usuniecia == 1 || sektor_do_usuniecia == 2 || sektory_na_dysku[sektor_do_usuniecia].dane[0] != 254)
		{
			cout << "Blad usuwania folderu." << endl;//nie ma folderu o takiej nazwie (stad 0) lub sektor nie jest folderem
		}
		else
		{
			for (int j = 4; j < 16; j++)
			{
				if (sektory_na_dysku[sektor_do_usuniecia].dane[j] != 0)
				{
					usun_plik(sektory_na_dysku[sektor_do_usuniecia].dane[j]);
				}
			}
			cout << "Folder \"" << sektory_na_dysku[sektor_do_usuniecia].dane[1] << sektory_na_dysku[sektor_do_usuniecia].dane[2] << sektory_na_dysku[sektor_do_usuniecia].dane[3] << "\" zostal usuniety." << endl;
			sektory_na_dysku[sektor_do_usuniecia].wyczysc();
			zmien_dostepnosc_sektora(sektor_do_usuniecia, false);
		}
		przelicz_wolne_miejsce();
	}
	void wyswietl_pliki_i_foldery()
	{
		cout << "Zawartosc dysku:" << endl;
		for (int i = 1; i < 64; i++)
		{
			if (sektory_na_dysku[i].dane[0] == 254)
			{
				sektory_na_dysku[i].wyswietl_wlasciwosci();
				for (int j = 4; j < 16; j++)
				{
					if (sektory_na_dysku[i].dane[j] != 0)
					{
						cout << (char)179 << "  ";
						sektory_na_dysku[sektory_na_dysku[i].dane[j]].wyswietl_wlasciwosci();
					}
				}
			}
			else if (sektory_na_dysku[i].dane[0] == 253)
			{
				sektory_na_dysku[i].wyswietl_wlasciwosci();
			}
		}
		for (int i = 1; i < 64; i++)
		{
			if (sektory_na_dysku[i].dane[0] == 255 && sektory_na_dysku[i].dane[5] == 0)
			{
				sektory_na_dysku[i].wyswietl_wlasciwosci();
			}
		}
		cout << (char)192 << (char)196 << (char)196 << (char)196 << "Pozostale miejsce: " << wolne_miejsce << "B" << endl;
	}
	void wyswietl_dysk_liczbowo()
	{
		for (int i = 0; i < 64; i++)
		{
			wyswietl_sektor_liczbowo(i);
			if (i % 16 == 0 && i != 0)
			{
				system("pause");
			}
		}
	}
	void wyswietl_dysk_znakowo()
	{
		for (int i = 0; i < 64; i++)
		{
			wyswietl_sektor_znakowo(i);
			if (i % 16 == 0 && i != 0)
			{
				system("pause");
			}
		}
	}
	void otworz_plik(string nazwa)
	{
		int sektor_pliku = szukaj_sektora_po_nazwie(nazwa);
		if ((sektor_pliku == 0)|| (sektory_na_dysku[sektor_pliku].dane[0] != 255&& sektory_na_dysku[sektor_pliku].dane[0] != 253))
		{
			cout << "Nazwa pliku nieprawidlowa/plik nie istnieje." << endl;
		}
		else if (sektor_pliku == 1)//plik komunikacyjny
		{
			cout << przekaz_fragment_pliku("KOM", 0) << endl;
		}
		else
		{
			for (int i = 0; i < ceil((double)sektory_na_dysku[sektor_pliku].dane[4] / 16); i++)
			{
				cout << przekaz_fragment_pliku(nazwa, i) << endl;
			}
		}
	}
	//DLA SZYMONA I DANIELA
	string przekaz_fragment_pliku(string nazwa, int numer_fragmentu = 0)
	{
		string wynik = "";
		int sektor_pliku = szukaj_sektora_po_nazwie(nazwa);
		if (sektor_pliku == 0)
		{
			cout << "Nazwa pliku nieprawidlowa/plik nie istnieje." << endl;
		}
		else if (sektor_pliku == 1)//plik komunikacyjny na stale
		{
			for (int i = 0; i < 16; i++)
			{
				wynik += sektory_na_dysku[2].dane[i];
			}
		}
		else if (numer_fragmentu > 14 || numer_fragmentu < 0)
		{
			cout << "Zazadano nieprawidlowego fragmentu pliku." << endl;
		}
		else
		{
			int temp;
			if (numer_fragmentu <4)
			{
				numer_fragmentu += 6;
				temp = sektory_na_dysku[sektor_pliku].dane[numer_fragmentu];
			}
			else
			{
				numer_fragmentu -= 4;
				temp = sektory_na_dysku[sektory_na_dysku[sektor_pliku].dane[10]].dane[numer_fragmentu];
			}
			for (int i = 0; i < 16; i++)
			{
				wynik += sektory_na_dysku[temp].dane[i];
			}
		}
		return wynik;
	}
	int przekaz_ilosc_sektorow(string nazwa)
	{
		int sektor = szukaj_sektora_po_nazwie(nazwa);
		if (sektor != 0)
		{
			if (sektory_na_dysku[sektor].dane[4] % 16 == 0)
			{
				return (int)ceil(sektory_na_dysku[sektor].dane[4] / 16);
			}
			else
			{
				return (int)ceil(sektory_na_dysku[sektor].dane[4] / 16)+1;
			}
		}
		else
		{
			cout << "Plik nie zostal znaleziony lub nie istnieje." << endl;
			return 0;
		}
	}
};