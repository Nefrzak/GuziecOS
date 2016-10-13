/* Daniel Hildebrand */

//Komunikacja miedzy procesami GuziecOS
/*Otwarcie lacza za pomoca PIPE pipe()
Dostepne argumenty:
w - writer
r - reader
Zapis do lacza:
pipe.open("DANE", argument typu char)
pipe.open(r);
Argumenty jak wy¿ej
DANE typu string, maksymalna iloœæ przesylanych danych wynosi 3, dlugoœæ FIFO = 4*/

#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include "guziec_sp.h"
#include "guziec_pamiec.h"
#include "Scheduler.h"

using namespace std;


const int buf_size = 16;
const int data_size = 3;

bool flaga = true;
bool flaga2 = true;


DYSK* dyskDaniel;

struct pipe_inode_info {
	/*wskaznik do bufora z danymi*/
	char *base;
	/*dlugosc fifo*/
	unsigned int fifo;
	/*ilosc danych*/
	unsigned int len;
	/*zmienna sluzaca do synchronizacji dostepu do stuktur przechowujacych dane.
	Rozna od zera oznacza, ze istnieja procesy, ktore pisza lub czytaja z lacza*/
	unsigned int lock;
	/*ilosc procesow czytajacych z lacza*/
	unsigned int readers;
	/*ilosc procesow piszacych do lacza*/
	unsigned int writers;
	int pipe_init_write;
	int pipe_exit_write;
}inode;

void fifo_init()
{
	
	//string Wstrzymany = Runqueue.curr->nazwa;
	//Runqueue.curr->wait(true);
	//find_by_name(Wstrzymany)->wait(false);
	inode.base = nullptr;
	inode.fifo = 0;
	inode.len = 0;
	inode.lock = 0;
	inode.readers = 0;
	inode.writers = 0;
	inode.pipe_init_write = 0;
	inode.pipe_exit_write = 1;
}

void inode_write(DYSK *dyskDaniel);


class PIPE
{
protected:
	char x;
	string Wstrzymany = "";
	//DYSK dyskDaniel;
	//zmienna x sprawdza kto otworzyl lacze w-pisarz, r-writer
public:
	PIPE()
	{

	}
	explicit PIPE(DYSK *d): x(0)
	{
		dyskDaniel = d;
		//this->dyskDaniel = dyskDaniel;
		//this->x = x;
		///Jeœli jest to pierwsze otwarcie iwezla przez ktorykolwiek z procesow
		if (flaga == true)
		{
			dyskDaniel->wypelnij_plik_danymi("KOM", "................");
			fifo_init();
			inode.base = pamiec.getZarezerwowanaStrona();
			inode_write(dyskDaniel);
			for (int i = 0; i < 16;i++)
			{
				inode.base[i] = '.';
			}
		}
		/*if (dyskDaniel->sektory_na_dysku[1].dane[10] == '0' && dyskDaniel->sektory_na_dysku[1].dane[11] == '1')
		{
			inode.pipe_init_write = 1;
			dyskDaniel->sektory_na_dysku[1].dane[10] = (inode.pipe_init_write + '0');
			inode.pipe_exit_write = 0;
			dyskDaniel->sektory_na_dysku[1].dane[11] = (inode.pipe_exit_write + '0');
			inode.lock = 1;
			dyskDaniel->sektory_na_dysku[1].dane[7] = (inode.lock + '0');
			inode.writers = 1;
			dyskDaniel->sektory_na_dysku[1].dane[9] = (inode.writers + '0');
			cout << "Lacze zostalo utworzone" << endl;
		}*/
		else
		{
			//Jeœli proces piszacy nie zaczal zapisu to usypia proces czytajacy
			if (dyskDaniel->sektory_na_dysku[1].dane[10] == '0' && dyskDaniel->sektory_na_dysku[1].dane[11] == '1')
			{
				Wstrzymany = Runqueue.curr->nazwa;
				Runqueue.curr->wait(true);
				cout << "Poroces zostal uspany." << endl;
			}
			//Jeœli proces piszacy zaczal zapis ale nie skoñczyl i nie wyszedl z pliku to usypia proces czytajacy
			else if (dyskDaniel->sektory_na_dysku[1].dane[10] == '1' && dyskDaniel->sektory_na_dysku[1].dane[11] == '0')
			{
				Wstrzymany = Runqueue.curr->nazwa;
				Runqueue.curr->wait(true);
				cout << "Poroces zostal uspany." << endl;
			}
			else if (dyskDaniel->sektory_na_dysku[1].dane[10] == '1' && dyskDaniel->sektory_na_dysku[1].dane[11] == '1' && dyskDaniel->sektory_na_dysku[1].dane[7] == '0')
			{
				inode.readers = +1;
				inode.lock = 1;
				dyskDaniel->sektory_na_dysku[1].dane[8] = (inode.readers + '0');
				dyskDaniel->sektory_na_dysku[1].dane[8] = (inode.lock + '0');
				cout << "Proces mo¿e zaczaæ odczyt z lacza." << endl;
			}
		}
	}
	/*Read*/
	string read()
	{
		string zmienna = dyskDaniel->przekaz_fragment_pliku("KOM");
		string zmienna2;
		string dane;
		int i = 0;
		for (i = 0; i < 3; i++)
		{
			if (zmienna[i] != '.' && zmienna[i] != ',')
				dane += zmienna[i];
			else
				break;
		}
		for (int j = i + 1; j < buf_size; j++)
		{
			zmienna2 += zmienna[j];
		}
		for (int k = (int) zmienna2.length(); k < 16; k++)
		{
			zmienna2 += '.';
		}
		dyskDaniel->wypelnij_plik_danymi("KOM", zmienna2);
		return dane;
	}
	void open(string znaki, char operacja)
	{
		if (dyskDaniel->sektory_na_dysku[1].dane[10] == '0' && dyskDaniel->sektory_na_dysku[1].dane[11] == '1')
		{
			inode.pipe_init_write = 1;
			dyskDaniel->sektory_na_dysku[1].dane[10] = (inode.pipe_init_write + '0');
			inode.pipe_exit_write = 0;
			dyskDaniel->sektory_na_dysku[1].dane[11] = (inode.pipe_exit_write + '0');
			inode.lock = 1;
			dyskDaniel->sektory_na_dysku[1].dane[7] = (inode.lock + '0');
			inode.writers = 1;
			dyskDaniel->sektory_na_dysku[1].dane[9] = (inode.writers + '0');
			cout << "Lacze zostalo utworzone" << endl;
		}
		int x = (int)znaki.length() - 1;
		int y = (int)znaki.length() + dyskDaniel->sektory_na_dysku[1].dane[6] - 49;
		if (operacja == 'w' && znaki.length()-1<data_size && (znaki.length() + dyskDaniel->sektory_na_dysku[1].dane[6] - 49) < buf_size)
		{
			flaga2 = true;
				pamiec.setDane(znaki);
				pamiec.zapiszBuforNaDysku();
				if (dyskDaniel->sektory_na_dysku[1].dane[6] > 0)
				{
					dyskDaniel->sektory_na_dysku[1].dane[6] += 4;
				}
				else
				{
					dyskDaniel->sektory_na_dysku[1].dane[6] = +3;
				}
		}
		else if (operacja != 'w')
		{
			cout << "Bledna operacja!" << endl;
		}
		if (znaki.length() > data_size || znaki.length() + dyskDaniel->sektory_na_dysku[1].dane[6] - 50 > buf_size)
		{
			cout << "Zbyt duza ilosc danych. Operacja nie powiodla sie." << endl;
		}

	}
	string open(char operacja)
	{
		if (operacja == 'r')
		{
			//Jeœli proces piszacy nie zaczal zapisu to usypia proces czytajacy // 
			if (dyskDaniel->sektory_na_dysku[1].dane[10] == '0' && dyskDaniel->sektory_na_dysku[1].dane[11] == '1')
			{
				Wstrzymany = Runqueue.curr->nazwa;
				Runqueue.curr->wait(true);
				cout << "Proces zostal uspiony." << endl;
			}
			//Jeœli proces piszacy zaczal zapis ale nie skoñczyl i nie wyszedl z pliku to usypia proces czytajacy
			else if (dyskDaniel->sektory_na_dysku[1].dane[10] == '1' && dyskDaniel->sektory_na_dysku[1].dane[11] == '0')
			{
				Wstrzymany = Runqueue.curr->nazwa;
				Runqueue.curr->wait(true);
				cout << "Proces zostal uspiony." << endl;
			}
		}
		if (operacja == 'r')
		{
			if (dyskDaniel->sektory_na_dysku[1].dane[10] == '1' && dyskDaniel->sektory_na_dysku[1].dane[11] == '1' && dyskDaniel->sektory_na_dysku[1].dane[7] == '0')
			{
				if (dyskDaniel->sektory_na_dysku[1].dane[8] == '0')
				{
					cout << "Proces moze zaczac odczyt z lacza." << endl;
				}
				inode.readers = 1;
				inode.lock = 1;
				dyskDaniel->sektory_na_dysku[1].dane[8] = (inode.readers + '0');
				dyskDaniel->sektory_na_dysku[1].dane[8] = (inode.lock + '0');
			}
			flaga2 = false;
			return read();
		}
		else
			return "Blad operacji.";
			//cout << "Blad operacji." << endl;
	}

	/*Zamykanie lacza*/
	void close()
	{
		if (dyskDaniel->sektory_na_dysku[1].dane[10] == '1' && dyskDaniel->sektory_na_dysku[1].dane[11] == '0' && dyskDaniel->sektory_na_dysku[1].dane[7] == '1')
		{
			inode.pipe_init_write = 1;
			inode.pipe_exit_write = 1;
			inode.lock = 0;
			inode.writers--;
			inode_write(dyskDaniel);
			if(Wstrzymany != "")
			find_by_name(Wstrzymany)->wait(false);
			cout << "Zamykanie lacza ze strony pisarza powiodlo sie. Dane czekaja na odczyt. \nCzytelnicy zostali wybudzeni. Liczba pisarzy: " << dyskDaniel->sektory_na_dysku[1].dane[9] << endl;
		}
		else if (dyskDaniel->sektory_na_dysku[1].dane[8] == '0' && dyskDaniel->sektory_na_dysku[1].dane[9] == '1')
		{
			inode.readers--;
			inode.lock = 0;
			inode_write(dyskDaniel);
			cout << "Zamykanie lacza ze strony czytelnika powiodlo sie. Liczba czytelnikow: " << dyskDaniel->sektory_na_dysku[1].dane[8] << endl;
			if (dyskDaniel->sektory_na_dysku[1].dane[8] == '0')
			{
				unlink();
			}
		}
		else if (dyskDaniel->sektory_na_dysku[1].dane[5] == '0')
		{
			cout << "Plik wymiany zamkniety. Dlugosc kolejki = 0." << endl;
			unlink();
		}
	}
	/*Jeœli nie ma ju¿ ani czytelnikow, ani pisarzy lacze zostanie usuniete*/
	static void unlink()
	{
		fifo_init();
		inode_write(dyskDaniel);
		flaga = true;
		flaga2 = true;
		if (inode.base == nullptr)
		{
			dyskDaniel->wypelnij_plik_danymi("KOM", "................");
			cout << "Lacze zostalo usuniete poprawnie" << endl;
		}
		else
		{
			cout << "Usuwanie lacza nie powiodlo sie." << endl;
		}
	}
};

void inode_write(DYSK *dyskDaniel)
{
	dyskDaniel->sektory_na_dysku[1].dane[5] = (inode.fifo + '0');
	dyskDaniel->sektory_na_dysku[1].dane[6] = (inode.len + '0');
	dyskDaniel->sektory_na_dysku[1].dane[7] = (inode.lock + '0');
	dyskDaniel->sektory_na_dysku[1].dane[8] = (inode.readers + '0');
	dyskDaniel->sektory_na_dysku[1].dane[9] = (inode.writers + '0');
	dyskDaniel->sektory_na_dysku[1].dane[10] = (inode.pipe_init_write + '0');
	dyskDaniel->sektory_na_dysku[1].dane[11] = (inode.pipe_exit_write + '0');
	//
	dyskDaniel->sektory_na_dysku[1].dane[12] = ('0');
	dyskDaniel->sektory_na_dysku[1].dane[13] = ('1');
	dyskDaniel->sektory_na_dysku[1].dane[14] = ('2');
	dyskDaniel->sektory_na_dysku[1].dane[15] = ('3');
	if (flaga == true)
	{
		inode.base = pamiec.getZarezerwowanaStrona();
		flaga = false;
	}
}