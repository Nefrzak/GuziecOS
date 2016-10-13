//Leszek Stencel

/* Za ca³oœæ projektu odpowiadaj¹:
Leszek Stencel
Piotr Siwiñski
Tomasz Braczyñski
Mateusz Korolow
Szymon Nowak
Daniel Hildebrand*/

// GuziecOS.cpp : Defines the entry point for the console application.
#pragma once
#include "stdafx.h"
#include "guziec_sp.h"
#include "Scheduler.h"
#include "guziec_pamiec.h"
#include "guziec_kmp.h"

using namespace std;

DYSK dysk = DYSK();

void witam_guzce()
{
	system("cls");
	cout << "      @@@@@@                              @@                               @@@@@           @@@@@    " << endl;
	cout << "    @@@@@@@@@@                            @@                             @@@@@@@@@       @@@@@@@@@  " << endl;
	cout << "   @@@@     @@@                           @@                            @@@     @@@     @@@     @@@ " << endl;
	cout << "   @@        @@                                                        @@@       @@@    @@       @@ " << endl;
	cout << "  @@@        @@@                                  @@@          @@      @@         @@@   @@       @@ " << endl;
	cout << "  @@               @@     @@  @@@@@@@@@@  @@    @@@@@@@      @@@@@@   @@@         @@@   @@          " << endl;
	cout << " @@@               @@     @@         @@@  @@   @@@   @@@    @@@  @@@  @@@          @@   @@@@@       " << endl;
	cout << " @@@               @@     @@        @@@   @@   @@     @@@  @@@    @@@ @@           @@    @@@@@@@@   " << endl;
	cout << " @@@     @@@@@@@   @@     @@       @@@    @@   @@      @@  @@         @@           @@      @@@@@@@  " << endl;
	cout << " @@@     @@@@@@@   @@     @@      @@@     @@  @@@@@@@@@@@  @@         @@           @@          @@@@ " << endl;
	cout << " @@@          @@   @@     @@      @@      @@  @@@@@@@@@@@  @@         @@@          @@            @@@" << endl;
	cout << "  @@          @@   @@     @@     @@       @@  @@@          @@         @@@         @@@  @@         @@" << endl;
	cout << "  @@@         @@   @@     @@    @@        @@   @@      @@  @@      @@  @@@        @@   @@@       @@@" << endl;
	cout << "   @@@       @@@   @@    @@@   @@@        @@   @@     @@@  @@@    @@@   @@@      @@@    @@@      @@ " << endl;
	cout << "    @@@@@@@@@@@    @@@@ @@@@  @@@@@@@@@@  @@    @@@  @@@    @@@  @@@    @@@@@@@@@@@     @@@@@@@@@@@ " << endl;
	cout << "     @@@@@@@@@      @@@@@ @@  @@@@@@@@@@  @@     @@@@@@      @@@@@@       @@@@@@@         @@@@@@@@  " << endl<<endl;
}

void init();
PIPE pipe = PIPE(&dysk);

bool kill(string nazwa);

class Interpreter
{
public:
	string nazwa;
	short rejestrA;
	short rejestrB;
	short rejestrC;
	short licznikRozkazow;
	Interpreter()
	{
		rejestrA = 0;
		rejestrB = 0;
		rejestrC = 0;
		licznikRozkazow = 0;
	}

	//pobieramy rejestry z aktualnie wykonywanego procesu
	void pobierzRejestry()
	{
		if (Runqueue.curr != nullptr)
		{
			rejestrA = Runqueue.curr->A;
			rejestrB = Runqueue.curr->B;
			rejestrC = Runqueue.curr->C;
			licznikRozkazow = Runqueue.curr->counter;
			nazwa = Runqueue.curr->nazwa;
		}
	}

	void wyslijRejestry() // zapisujemy zamiany w rejestrach w procesie
	{
		if (Runqueue.curr!=nullptr)
		{
			Runqueue.curr->A = rejestrA;
			Runqueue.curr->B = rejestrB;
			Runqueue.curr->C = rejestrC;
			Runqueue.curr->counter = licznikRozkazow;
		}
		rejestrA = 0;
		rejestrB = 0;
		rejestrC = 0;
		licznikRozkazow = 0;
//		else
//			cout << "Aktualnie zaden proces nie jest w stanie Aktywny";
		
	}
	void wyswietlStan() const
	{
		cout << "nazwa procesu: \t" << nazwa << endl;
		cout << "rej A: \t" <<rejestrA << endl;
		cout << "rej B: \t" << rejestrB<< endl;
		cout << "rej C: \t" << rejestrC << endl;
		cout << "PC: \t" << licznikRozkazow<< endl;
	}

	string pobierzRozkaz()  //Pobiera rozkaz z pamiêci RAM
	{
		if (Runqueue.curr->nazwa == "IDLE" || Runqueue.curr->nazwa == "INIT")
			return ""; // return string();
		string rozkaz = pamiec.pobierzRozkaz(Runqueue.curr->nazwa, Runqueue.curr->counter);
		return rozkaz;		
	}
	
	void wykonajRozkaz()
	{
		string instrukcja = pobierzRozkaz();
		if (instrukcja != "")
		{
				string rozkaz, rejestr;
			cout << "\n aktualnie wykonywany rozkaz: " << instrukcja << endl;
			rozkaz = instrukcja.substr(0, 2);
			if (rozkaz == "AD")
			{
				rejestr = instrukcja.substr(3, 1);
				if (rejestr == "B")
					rejestrA += rejestrB;
				else
					rejestrA += rejestrC;
			}
			else if (rozkaz == "SB")
			{
				rejestr = instrukcja.substr(3, 1);
				if (rejestr == "B")
					rejestrA -= rejestrB;
				else
					rejestrA -= rejestrC;
			}
			else if (rozkaz == "MP") //Mnozenie
			{
				rejestr = instrukcja.substr(3, 1);
				if (rejestr == "B")
					rejestrA *= rejestrB;
				else
					rejestrA *= rejestrC;
			}
			else if (rozkaz == "IC") //increment
			{
				rejestr = instrukcja.substr(3, 1);
				if (rejestr == "A")
					rejestrA++;
				else if (rejestr == "B")
					rejestrB++;
				else
					rejestrC++;
			}
			else if (rozkaz == "MV") //Naprawione - PS
			{
				rejestr = instrukcja.substr(3, 1);
				string wartosc;
				wartosc = instrukcja.substr(5, 3);
				int liczba = 0;

	#pragma region Przerabianie stringa na inta
				if (wartosc[1] == 32 && wartosc[2] == 32)
					liczba = (wartosc[0] - '0');
				else if (wartosc[2] == 32)
					liczba = (wartosc[0] - '0') * 10 + wartosc[1] - '0';
				else
					liczba = (wartosc[0] - '0') * 100 + (wartosc[1] - '0') * 10 + (wartosc[2] - '0');
	#pragma endregion 
				if (rejestr == "A")
					rejestrA = liczba;
				if (rejestr == "B")
					rejestrB = liczba;
				if (rejestr == "C")
					rejestrC = liczba;
			}
			else if (rozkaz == "JW")  //jump
			{
				int liczba = 0;
				string wartosc = instrukcja.substr(3, 2);

				if (wartosc[1] == 32)
					liczba = (wartosc[0] - '0');
				else
					liczba = ((wartosc[0] - '0') * 10) + (wartosc[1] - '0');
				if (liczba > rejestrA)
				{
					licznikRozkazow = rejestrC;
				}
			}
			else if (rozkaz == "JM")  //jump
			{
				int liczba = 0;
				string wartosc = instrukcja.substr(3, 2);

				if (wartosc[1] == 32)
					liczba = (wartosc[0] - '0');
				else
					liczba = ((wartosc[0] - '0') * 10) + (wartosc[1] - '0');
				if (liczba < rejestrA)
				{
					licznikRozkazow = rejestrC;
				}
			}
			else if (rozkaz == "PT") // PT - PÊTLA
			{
				rejestrC = licznikRozkazow;
			}
			else if (rozkaz == "WW")
			{
				string wiadomosc = "";
				string msg = instrukcja.substr(3, 3);
				if (msg[1] == 32 && msg[2] == 32)
				{					
					wiadomosc = msg.substr(0,1);
					pipe.open(wiadomosc, 'w');
				}					
				else if (msg[2] == 32)
				{
					wiadomosc= msg.substr(0, 2);
					pipe.open(wiadomosc, 'w');
				}					
				else
				{
					wiadomosc = msg;
					pipe.open(wiadomosc, 'w');
				}
					
			}
			else if (rozkaz == "OW")
			{
				string wiadomosc;
				wiadomosc=pipe.open('r');
				cout << wiadomosc << endl;
			}
			else if (rozkaz == "EX")
			{
				if (Runqueue.curr->nazwa == "PRW")
				{
					pipe.close();
				}
				cout << endl << "Proces o nazwie: " << Runqueue.curr->nazwa << " sie wykonal" << endl;
				bool exitStatus = Runqueue.curr->exit(0);
				Runqueue.curr = nullptr;
			}
			licznikRozkazow++;
		}
		
	};
	void wykonajProgramUzytkownika(string rozkaz)
	{
		 if (rozkaz.substr(0, 3) == "CLS")
		{
			system("cls");
		}
		else if (rozkaz.substr(0, 1) == "C") //WYKONUJEMY KOLEJNY ROZKAZ
		{
			if (Runqueue.curr == nullptr)
				Runqueue.schedule();
			if (Runqueue.curr->credit>0)
			{
				pobierzRejestry();
				Runqueue.curr->credit--;
				Runqueue.setTimeWaiting(); //Zwiêksza o 1 czas oczekiwania w Runqueue
				wykonajRozkaz();
				wyswietlStan();
				wyslijRejestry();
			}
			if (Runqueue.curr != nullptr && Runqueue.curr->credit==0) //Je¿eli po wykonaniu rozkazu kredyt spad³ do 0 to oddaje procesor
			{
				cout << "Proces wyczerpal swoj kredyt i oddaje procesor" << endl;
				Runqueue.sched_yield();
			}
		}
		else if (rozkaz.substr(0, 2) == "FE") //ROZWIDLA AKTUALNY PROCES - FORK
		{
			string nazwaProcesu = rozkaz.substr(3, 3);
			string whoIsParent = rozkaz.substr(7, 4);
			struct_task* parent = nullptr;
			
			if (whoIsParent == "INIT")
				parent = &INIT_TASK;
			else
				parent = find_by_name(whoIsParent);

			parent->fork(nazwaProcesu);
			parent->next_task->state = struct_task::stanProcesu::Gotowy;
			//pamiec.zarezerwujPamiec(nazwaProcesu);
			
			Runqueue.add_to_runqueue(parent->next_task);
		}
		else if (rozkaz.substr(0, 2) == "WP") //PROCES MACIERZYSTY WYWO£UJE METODÊ WAITPID - MACIERZYSTY CZEKA NA DZIECKO
		{
			string nazwaProcesu = rozkaz.substr(3, 3);
			//PRZESZUKUJEMY LISTÊ CYKLICZN¥ - ZNAJDUJEMY PO NAZWIE 
			auto dziecko = find_by_name(nazwaProcesu);
			//PRZEZ WSKANIK POTOMNEGO WYWO£UJEMY WAITPID NA MACIERZYSTYM
			if (dziecko)
			{
				dziecko->pptr->waitpid(dziecko->PID);
			}
			else
				cout << "Proces o takiej nazwie nie istnieje" << endl;
		}
		else if (rozkaz.substr(0, 2) == "SP") //STWORZ PLIK  - OK
		{
			if (rozkaz.length()<7)
			{
				string nazwaPliku, sciezka;
				nazwaPliku = rozkaz.substr(3, 3);
				dysk.stworz_plik(nazwaPliku);
			}
			else
			{
				string nazwaPliku, sciezka;
				nazwaPliku = rozkaz.substr(3, 3);
				sciezka = rozkaz.substr(7, rozkaz.length() - 7);
				dysk.stworz_plik(nazwaPliku, sciezka);
			}
		}
		else if (rozkaz.substr(0, 2) == "ZP")// Za³aduj plik danymi
		{
			string nazwaPliku, sciezka;
			nazwaPliku = rozkaz.substr(3, 3);
			sciezka = rozkaz.substr(7, rozkaz.length() - 7);
			dysk.wypelnij_plik_danymi(nazwaPliku, sciezka);
		}
		else if (rozkaz.substr(0, 2) == "SF") //STWORZ FOLDER OK
		{
			string nazwaFolderu = rozkaz.substr(3, 3);
			dysk.stworz_folder(nazwaFolderu);
		}
		else if (rozkaz.substr(0, 2) == "KP") //JAWNE WYWO£ANIE kill(nazwa_procesu)
		{
			string nazwaProcesu = rozkaz.substr(3, 3);
			auto wsk = find_by_name(nazwaProcesu);
			if(wsk == Runqueue.curr)
			{
				kill(nazwaProcesu);
				Runqueue.curr = nullptr;
				Runqueue.schedule();
			}
			else
				kill(nazwaProcesu);
			
		}
		else if (rozkaz.substr(0, 2) == "UP")
		{
			string nazwaPliku;
			nazwaPliku = rozkaz.substr(3, 3);
			dysk.usun(nazwaPliku);			
		}
		else if (rozkaz.substr(0, 2) == "WD")
		{
			dysk.wyswietl_dysk_liczbowo();
		}
		else if (rozkaz.substr(0, 2) == "OP")
		{
			string nazwaPliku;
			nazwaPliku = rozkaz.substr(3, 3);
			dysk.otworz_plik(nazwaPliku);
		}
		else if (rozkaz.substr(0, 2) == "PG") // Poka¿ wszystkie gotowe
		{
			Runqueue.showAllReady();
		}
		else if (rozkaz.substr(0, 2) == "PA") // Poka¿ wszystkie w kolejce active
		{
			Runqueue.showActive();
		}
		else if (rozkaz.substr(0, 2) == "PE") // Poka¿ wszystkie w kolejce expired
		{
			Runqueue.showExpired();
		}
		else if (rozkaz.substr(0, 2) == "SN") //set Nice - zmiana wartosci nice
		{
			string wartosc = rozkaz.substr(3, 1);
			int inc = 0;
				inc = (wartosc[0] - '0');
			Runqueue.set_priority(inc);
		}
		else if (rozkaz.substr(0, 2) == "ZR") // ZnajdŸ w Runqueue
		{
			string nazwaProcesu = rozkaz.substr(3, 4);
			Runqueue.findProcessInRunqueue(nazwaProcesu);
		}
		else if (rozkaz.substr(0, 2) == "PC") // Poka¿ Current
		{
			if (Runqueue.curr != nullptr)
				cout << "Aktualny proces to: " << Runqueue.curr->nazwa << " PID: " << Runqueue.curr->PID << " PPID: " << Runqueue.curr->PPID << " Priorytet: " << Runqueue.curr->priority << " Kredyt: " << Runqueue.curr->credit << " Stan: " << Runqueue.curr->state << endl;
			else
				cout << "Nalezy poszeregowac procesy prosze wcisnac C" << endl;
		}
		else if (rozkaz.substr(0, 4) == "LS")
		{
			dysk.wyswietl_pliki_i_foldery();
		}
		else if (rozkaz.substr(0, 2) == "PP") //Poka¿ Procesy
		{
			string nazwaProcesu = rozkaz.substr(3, 4);
			showProcess(nazwaProcesu);
		}
		else if (rozkaz.substr(0, 2) == "PF")
		{
			pamiec.wypiszPamiecFizyczna();
		}
		else if (rozkaz.substr(0, 2) == "PW")
		{
			pamiec.wypiszPamiecWirtualna();
		}
		else if (rozkaz.substr(0, 2) == "PM")
		{
			pamiec.wypiszPamiec();
		}
		else if (rozkaz.substr(0, 2) == "PB")
		{
			pamiec.wypiszBufor();
		}
		else if (rozkaz.substr(0, 2) == "NS")
		{
			pamiec.wypiszNajstarszaStrone();
		}
		else if (rozkaz.substr(0, 2) == "XX")
		{
			cout << "Nie znaleziono takiej komendy" << endl;
			Runqueue.curr = nullptr;
		}
		else
		{
			if(rozkaz.length()!=0)
			cout << "Nie znaleziono takiej komendy" << endl;
		}
	}
};

Interpreter interpreter = Interpreter();

void bootOs()
{
	witam_guzce();
	init();
	setDysk(&dysk); // Dysk dla Szymona
	//pipe = PIPE(&dysk);
}

int main()
{
	bootOs();
	string rozkaz;

	while(rozkaz!="EXIT")
	{
		cout << "GuziecO$: ";
		try
		{
			getline(cin, rozkaz);
			interpreter.wykonajProgramUzytkownika(rozkaz);
		}
		catch (...)
		{
			cout << "Blad - prawdopodobnie zla komenda, zla kolejnosc lub brak parametru funkcji" << endl;
		}
		
	}
	cout << "Dziekujemy za korzystanie z GuziecOS - nacisnij enter, aby zakonczyc" << endl;
	getchar();
    return 0;
}
