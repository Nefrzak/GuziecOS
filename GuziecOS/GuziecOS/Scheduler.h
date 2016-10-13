//Piotr Siwiñski & Mateusz Korolow

#pragma once
#include <iostream>
#include <map>
#include <list>
#include <vector>
#include "guziec_pamiec.h"

using namespace std;

class runqueue;

/*POLE PRZECHOWUJ¥CE W SOBIE OSTATNIO U¯YTY PID - NA JEGO PODSTAWIE WYZNACZA SIÊ NOWY PID DLA TWORZONEGO PROCESU*/
int last_pid = 1;

/*LISTA STRUKTUR ZAWIERAJ¥CYCH PIDY PROCESÓW KTÓRE JU¯ SIÊ WYKONALY ORAZ REZULTAT ICH WYKONANIA*/
struct exiting
{
	int kto, rezultat;
};
vector<exiting> exits;

/*LISTA STRUKTUR ZAWIERAJ¥CYCH PIDY PROCESÓW CZEKAJ¥CYCH ORAZ PIDY PROCESÓW NA KTÓRE CZEKAJ¥*/
struct waiting
{
	int kto, na;
};
vector<waiting> waits;

/*DEKLARACJA FUNKCJI KILL NAD STRUKTURA PCB ZE WZGLEDU NA TO ZE METODY TEJ STRUKTURY TYPU EIT CZY WAITPID JA WYWOLUJA, SAMA METODA ZNAJDUJE SIE POD STRUKTURA PONIEWAZ KORZYSTA ZE STATYCZNIE UTWORZONEGO PRZEZ TA STRUKTURE PROCESU INIT_TASK */
bool kill(string nazwa);

struct struct_task
{
	/*ZDEFINIOWANIE STANÓW PROCESU*/
	enum class stanProcesu { Nowy, Gotowy, Aktywny, Czekaj¹cy, Zakoñczony, Zombie, Idle };  

																					  /*POWI¥ZANIA Z PROCESAMI POKREWNYMI*/
	struct_task  *pptr, *cptr, *osptr, *ysptr;

	/*NAZWA PROCESU*/
	string nazwa;

	/*IDENTYFIKATORY*/
	int PID, PPID;

	/*PARAMETRY SZEREGOWANIA*/
	stanProcesu state;
	int rt_priority, priority, credit, old_credit, time_waiting;

	/*WSKANIK DO ELEMENTU NASTÊPNEGO ORAZ POPRZEDZAJ¥CEGO OBECNY PROCES NA LIŒCIE CYKLICZNEJ WSZYSTKICH PROCESÓW*/
	struct_task *prev_task, *next_task;

	/*KONTEKST POZIOMU REJESTRU*/
	short A, B, C, counter;

	/*TWORZY NOWY PROCES NA PODSTAWIE PROCESU MACIERZYSTEGO I DODAJE GO DO TABLICY PROCESÓW*/

	int fork(string nazwa)
	{
		// TWORZE NOWY PROCES

		auto nowy_proces = new struct_task();

		pamiec.zarezerwujPamiec(nazwa); // proc alokujemy pamiêæ dlaesu o konkretnej nazwie wywo³uj¹c funkcjê Szymona	
		

		// USTAWIAM STAN PROCESU NA NOWY

		nowy_proces->state = stanProcesu::Nowy;
		nowy_proces->nazwa = nazwa;

		// DODAJE NOWY PROCES DO CYKLICZNEJ LISTY PROCESOW
		auto wsk = this->next_task;

		this->next_task->prev_task = nowy_proces;
		nowy_proces->next_task = this->next_task;

		this->next_task = nowy_proces;
		nowy_proces->prev_task = this;

		// NADAJE PID NOWEMU PROCESSOWI

		nowy_proces->PID = getFreePID();
		nowy_proces->PPID = this->PID;

		// USTAWIA WIÊZI RODZINNE

		if (!this->cptr)
		{
			this->cptr = nowy_proces;
			this->cptr->pptr = this;
		}
		else
		{
			wsk = this->cptr;
			while (wsk->ysptr) wsk = wsk->ysptr;

			wsk->ysptr = nowy_proces;
			nowy_proces->osptr = wsk;

			nowy_proces->pptr = this;
		}

		// ZWRACAMY PID NOWEGO PROCESU

		return nowy_proces->PID;
	}

	// ZWRACA PIERWSZY MO¯LIWY DO WYKORZYSTANIA PID DLA NOWEGO PROCESU
	int getFreePID()
	{
		while (true)
		{
			if ((++last_pid) & 0xffff8000)
				last_pid = 1;

			auto wsk = this;
			auto busy = false;
			while (wsk->next_task != this)
			{
				wsk = wsk->next_task;
				if (wsk->PID == last_pid)
				{
					busy = true;
					break;
				}
			}

			if (!busy) return last_pid;
		}
	}

	// WYKONUJE WAIT DLA PROCESU NA KONKRETNYM PID PROCESU POTOMNEGO
	void waitpid(int PID);

	// WYKONUJE EXIT DLA PROCESU POTOMNEGO W CELU ZASYGNALIZOWANIA MU ¯E DOBIEGL KONCA: 0 - PROCES POTOMNY ZAKONCZONY POMYSLNIE, 1 - PROCES POTOMNY ZAKONCZONY BLEDEM
	bool exit(int result);
	// DEKLARACJA KONSTRUKTORA
	struct_task();

	//METODA DLA DANIELA
	void wait(bool stan);
	

}IDLE, INIT_TASK;

// ZABIJA PROCES POPRZEZ ZWOLNIENIE ZASOBÓW I USUNIÊCIE PROCESU Z LISTY CYKLICZNEJ, W PRZYPADKU POSIADANIA POTOMSTWA ZOSTAJE ONO ZAADOPTOWANE PRZEZ PROCES INIT
bool kill(string nazwa);

//ZNAJDOWANIE PROCESU PO NAZWIE NA LIŒCIE CYKLICZNEJ
struct_task* find_by_name(string nazwa)
{
	if (nazwa == "IDLE") return &IDLE;
	else if (nazwa == "INIT") return &INIT_TASK;
	else
	{
		auto wsk = &IDLE;

		while (wsk->next_task->nazwa != "IDLE")
		{
			wsk = wsk->next_task;
			if (wsk->nazwa == nazwa) return wsk;
		}
	}
	return nullptr;
}

// KONSTRUKTOR NOWO TWORZONEGO PROCESU
struct_task::struct_task() : pptr(nullptr), cptr(nullptr), osptr(nullptr), ysptr(nullptr), PID(0), PPID(0), state(stanProcesu::Nowy), rt_priority(0), priority(4), credit(4), old_credit(4), time_waiting(0), prev_task(nullptr), next_task(nullptr), A(0), B(0), C(0), counter(0)
{

}
// PRZECI¥¯AMY OPERATOR STRUMIENIA WYJŒCIOWEGO W CELU MO¯LIWOŒCI WYŒWIETLENIA STANÓW PROCESU
ostream& operator<<(ostream& strumien, struct_task::stanProcesu stan)
{
	switch (stan)
	{
	case struct_task::stanProcesu::Aktywny:
		strumien << "Aktywny";
		break;
	case struct_task::stanProcesu::Nowy:
		strumien << "Nowy";
		break;
	case struct_task::stanProcesu::Gotowy:
		strumien << "Gotowy";
		break;
	case struct_task::stanProcesu::Czekaj¹cy:
		strumien << "Czekaj¹cy";
		break;
	case struct_task::stanProcesu::Zakoñczony:
		strumien << "Zakoñczony";
		break;
	case struct_task::stanProcesu::Zombie:
		strumien << "Zombie";
		break;

	case struct_task::stanProcesu::Idle:
		strumien << "Idle";
		break;
	default: break;
	}

	return strumien;
}

class prio_array
{
public:
	map <int, list<struct_task*>> Map;
	prio_array()
	{
		Map = map<int, list<struct_task*>>();
		Map[0] = list<struct_task*>();
		Map[1] = list<struct_task*>();
		Map[2] = list<struct_task*>();
		Map[3] = list<struct_task*>();
		Map[4] = list<struct_task*>();
		Map[5] = list<struct_task*>();
		Map[6] = list<struct_task*>();
		Map[7] = list<struct_task*>();
	}

	virtual ~prio_array()
	{
		
	}
};

class runqueue
{
	unsigned int nr_process;
	unsigned int expired_timestamp; /* czas ostatniej podmiany wzkaŸników active i expired */
	const int static_priority;
	int nice;

public:
	struct_task *curr, *idle;	/* aktualny process i process idle*/
	bool need_resched;
	prio_array active;
	prio_array expired;

	runqueue() : nr_process(0), expired_timestamp{ 0 }, static_priority(3), nice(0),curr{ nullptr }, idle{ nullptr }, need_resched(false)
	{
		active = prio_array();
		expired = prio_array();
	}

	virtual ~runqueue()
	{
		
	}

	void add_to_runqueue(struct_task * p)
	{
		if (p->nazwa != "INIT")
		active.Map[p->priority].push_back(p);
		if (curr == nullptr || p->priority < curr->priority) //|| curr->nazwa == "IDLE")// ? Trzeci warunek mnie zastanawia - chodzi o wyw³aszczenie IDLE przy dodawaniu
			need_resched = true;

		if (p->nazwa == "INIT")
			need_resched = true;


		if (need_resched)
			schedule();
	}

	void add_to_expired_queue(struct_task * p)
	{
		if(p->nazwa !="IDLE")
			expired.Map[p->priority].push_back(p);
	}

	void remove_from_run_queue(struct_task * p)
	{
		bool isSuccessful = false;
		auto resultActive = findProcessInActive(p->nazwa);
		auto resultExpired = findProcessInExpired(p->nazwa);
		if(resultActive == nullptr && resultExpired == nullptr)
		{
			//cout << "Takiego procesu nie ma wsrod kolejki gotowych" << endl;
		}
			

		else if(resultActive!= nullptr)
		{
			auto &list = active.Map[resultActive->priority];
			auto iterator = list.begin();
			for (auto &elem : list)
			{
				if (elem->PID == p->PID)
				{
					list.erase(iterator);
					isSuccessful = true;
					break;
				}
				++iterator;

			}
		}
		else if(resultExpired!=nullptr)
		{
			auto &list = active.Map[resultActive->priority];
			auto iterator = list.begin();
			for (auto &elem : list)
			{
				if (elem->PID == p->PID)
				{
					list.erase(iterator);
					isSuccessful = true;
					break;
				}
				++iterator;

			}
		}
	
//		if(isSuccessful)
//		cout << p->nazwa << " zostal usuniety z Runqueue" << endl;
	}

	int FindFirstQueue()
	{
		bool isEmpty = false;
		int notEmpty = 0;
		for (int i = 0; i < 8; i++)
		{
			if (!active.Map[i].empty())
			{
				notEmpty = i;
				return notEmpty;
			}

		}
		return -1;
	}


	void ChangeEra()
	{
		cout << "Zamiana epoki " << endl;
		//Zamiana wskaŸników
		auto result = active;
		active = expired;
		expired = result;
		resetTimeWaiting();
		//Ponowne kredytowanie przeterminowanych procesów
		for (int i = 0; i < 8; i++)
		{
			auto &list = active.Map[i];
			for (auto &elem : list)
				countCredit(elem);
		}
	}

	void countCredit(struct_task *p)
	{
		p->credit = p->old_credit / 2 + p->priority;
		p->old_credit = p->credit;
	}

	void countPriority()
	{
		
	}

	void countPriority(struct_task * p)
	{
		if (p->nazwa == "IDLE")
			p->priority = 7;
		else
		{
		//	p->priority = static_priority + nice;
			if (p->time_waiting > 30)
				p->priority = 1;
			else if (p->time_waiting > 25)
				p->priority = 2;
			else if (p->time_waiting > 20)
				p->priority = 3;
			else if (p->time_waiting > 15)
				p->priority = 4;
			else if (p->time_waiting > 10)
				p->priority = 5;
			else if (p->time_waiting > 5)
				p->priority = 6;

		}
			
	}

	void setPriority(struct_task * p)
	{
		if (p->nazwa == "IDLE")
			p->priority = 7;
		else
		{
			p->priority = static_priority + nice;
		}
	}

	void showAllReady()
	{
		showActive();
		showExpired();
	}
	void showActive()
	{
		bool isSth = false;
		cout << "Kolejki Active" << endl;
		for (int i = 0; i < 8; i++)
		{
			auto list = active.Map[i];
			for (auto elem : list)
			{
				cout << "Czekajacy proces to: " << elem->nazwa << " PID: " << elem->PID << " PPID: " << elem->PPID << " Priorytet: " << elem->priority << " Kredyt: " << elem->credit << " Stan: " << elem->state << endl;
				isSth = true;
			}
				
		}
		if (!isSth)
			cout << "Kolejki Active - kolejki procesow gotowych sa puste " << endl;
	}

	void showExpired()
	{
		bool isSth = false;
		cout << "Kolejki Expired" << endl;
		for (int i = 0; i < 8; i++)
		{
			auto list = expired.Map[i];
			for (auto elem : list)
			{
				cout << "Przeterminowany proces to: " << elem->nazwa << " PID: " << elem->PID << " PPID: " << elem->PPID << " Priorytet: " << elem->priority << " Kredyt: " << elem->credit << " Stan: " << elem->state << endl;
				isSth = true;
			}
		}

		if (!isSth)
			cout << "Kolejki Expired - kolejki procesow przeterminowanych sa puste " << endl;
	}
	/*METODY DO SZEREGOWANIA*/
	struct_task* schedule()
	{
		auto firstNotEmptyQueue = FindFirstQueue(); // Zwracamy numer pierwszej nie pustej kolejki z kolejki Active

		if (firstNotEmptyQueue == -1) //Je¿eli kolejki Active s¹ puste
		{
			ChangeEra(); // Zmieniamy epokê
			firstNotEmptyQueue = FindFirstQueue(); // Po zmianie epoki ponownie szukamy pierwszej nie pustej kolejki
			if (firstNotEmptyQueue == -1) // Obie kolejki s¹ puste prze³¹czamy na proces IDLE
			{
				curr = &IDLE;
				IDLE.state = struct_task::stanProcesu::Aktywny;
				return curr;
			}
		}
		
		auto maxPriority = active.Map[firstNotEmptyQueue].front(); // Zwracamy referencjê do procesu o najwy¿szym priorytecie z pierwszej nie pustej kolejki
		active.Map[firstNotEmptyQueue].pop_front(); // Usuwamy proces o najwy¿szym priorytecie z kolejki Aktywnych

		if (curr == nullptr) // Je¿eli ¿aden proces nie jest w stanie Active - np. przy starcie systemu
		{
			curr = maxPriority;
			maxPriority->state = struct_task::stanProcesu::Aktywny;
		}
		else //Wyw³aszczenie procesu
		{
			countPriority(curr);
			curr->state = struct_task::stanProcesu::Gotowy;


			if (curr->nazwa == "IDLE") // naprawa idla
				curr->state = struct_task::stanProcesu::Czekaj¹cy;



			if (curr->credit > 0 && curr->nazwa != "IDLE") //Jeœli aktualny proces mia³ kredyt wiêkszy od 0 do dodawany jest ponownie do Active 
				active.Map[curr->priority].push_back(curr);
			else  //Je¿eli kredyt == 0, to dodajemy do kolejki przeterminowamych
				add_to_expired_queue(curr);
			curr = nullptr;
			curr = maxPriority;
			curr->state = struct_task::stanProcesu::Aktywny;
		}

		need_resched = false;
		return curr;
	}

	void sched_yield()
	{
		if (curr->nazwa != "IDLE")
		{
			countPriority(curr);
			curr->state = struct_task::stanProcesu::Gotowy;

			if (curr->credit == 0)
				add_to_expired_queue(curr);
			else
				active.Map[curr->priority].push_back(curr);
			curr = nullptr;
		}
		else
		{
			curr->state = struct_task::stanProcesu::Czekaj¹cy;
			curr->priority = 7;
			curr->credit = 3;
			curr = nullptr;
		}
			
		schedule();
	}

	/*METODY DO OBLICZANIA PRIORYTETU*/
	/*Ustawienie parametru nice*/
	int get_nice() const
	{
		return nice;
	}

	void set_nice(int inc)
	{
		if (inc >= -3 && inc <= 4)
		{
			nice = inc;
			cout << "Nowa wartosc nice to :" << nice << endl;
		}
		else
			cout << "Zla wartosc nice: musi byæ z zakresu -3 do 4" << endl;
	}
	void set_priority(int p)
	{
		if (p >= 0 && p <= 7)
		{
			if (curr != nullptr)
			{
				curr->priority = p;
				cout << "Zmieniono priorytet na " << p << endl;
			}
				
			else
				cout << "Nacisnij C - trzeba poszeregowac" << endl;
		}
		else
			cout << "Zla wartosc priorytetu. Moze byc z zakresu od 0 do 7" << endl;
		
	}

	struct_task* findProcessInActive(string nazwa)
	{
		bool isPresent{ false };
		struct_task* szukany{ nullptr };
		//
		for (int i = 0; i < 8; i++)
		{
			auto &list = active.Map[i];
			for (auto &elem : list)
			{
				if (elem->nazwa == nazwa)
				{
					isPresent = true;
					szukany = elem;
					break;
				}

			}

		}
		/*if (isPresent)
			cout << "Proces o nazwie: " << szukany->nazwa << " jest obecny w Active" << endl;
		else
			cout << "Takiego procesu nie ma w Active" << endl;*/
		return szukany;
	}

	struct_task* findProcessInExpired(string nazwa)
	{
		bool isPresent{ false };
		struct_task* szukany{ nullptr };
		//
		for (int i = 0; i < 8; i++)
		{
			auto &list = expired.Map[i];
			for (auto &elem : list)
			{
				if (elem->nazwa == nazwa)
				{
					isPresent = true;
					szukany = elem;
					break;
				}

			}

		}
		/*if (isPresent)
			cout << "Proces o nazwie: " << szukany->nazwa << " jest obecny w Expired" << endl;
		else
			cout << "Takiego procesu nie ma w Expired" << endl;*/
		return szukany;
	}

	struct_task* findProcessInRunqueue(string nazwa) // Sprawdzamy, czy proces jest obecny w Kolejce Active lub Expired
	{
		auto  szukany = findProcessInActive(nazwa);
		if(szukany == nullptr)
			szukany = findProcessInExpired(nazwa);
		return szukany;
	}

	void setTimeWaiting() //Przeliczamy ile ka¿dy proces czeka na procesor // Metoda do liczenia priorytetów // Wywo³ana przy wykonaniu procesu
	{
		for (int i = 0; i < 8; i++)
		{
			auto &list = active.Map[i];
			for (auto &elem : list)
				elem->time_waiting++;
		}
	}

	void resetTimeWaiting() //zerujemy czas oczekiwania przy zmianie epoki - jedyne u¿ycie funkcji
	{
		for (int i = 0; i < 8; i++)
		{
			auto &list = active.Map[i];
			for (auto &elem : list)
				elem->time_waiting = 0;
		}
	}
};

runqueue Runqueue = runqueue();

void struct_task::waitpid(int PID)
{
	{
		// SPRAWDZAMY CZY PROCES NA KTÓRYM CHCEMY WYWO£AÆ WAITPIDA WYKONA£ EXITA ( ZNAJDUJE SIÊ NA LIŒCIE PRZECHOWUJ¥CEJ PROCESY KTÓRE SIÊ WYKONA£Y ( WYKONA£Y PROCEDURÊ EXIT() )
		int index = -1;
		for (int i = 0; i < exits.size(); i++)
			if (exits.at(i).kto == PID) { index = i; break; }

		// PROCES POTOMNY NA KTÓRYM WYKONUJEMY WAITPIDA WYKONA£ WCZEŒNIEJ EXIT
		if (index >= 0)
		{
			// ZNAJDUJEMY NA LISCIE PROCESOW POTOMNYCH PROCESU AKTUALNIE WYKONYWUJACEGO WAITPIDA PROCES O ZDEFINIOWANYM PIDZIE
			struct_task* wsk = this->cptr;
			while (wsk && wsk->PID != PID) wsk = wsk->ysptr;

			// DZIALAMY TYLKO JESLI PROCES KTORY WYKONUJE WAITPIDA MA PROCES POTOMNY O PIDZIE KTORY PODAJEMY JAKO ARGUMENT WAITPIDA
			if (wsk)
			{
				// ZMIENIAMY STAN PROCESU NA ZAKOÑCZONY
				wsk->state = stanProcesu::Zakoñczony;

				// ZABIJAMY PROCES CZYLI: ZNAJDUJEMY GO NA LIŒCIE CYKLICZNEJ WSZYSTKICH PROCESÓW, ZWALNIAMY ZAJMOWAN¥ PRZEZ NIEGO PAMIÊÆ, USUWAMY GO Z LISTY CYKLICZNEJ WSZYSTKICH PROCESÓW, INIT_TASK ADOPTUJE WSZYSTKIE JEGO POTOMNE
				kill(wsk->nazwa);

				// USUWAMY INFORMACJÊ O ZAKOÑCZENIU SIÊ PROCESU Z LISTY EXITS PO USUNIÊCIU PROCESU
				exits.erase((exits.begin() + index));
			}
		}
		// PROCES POTOMNY NA KTÓRYM WYWO£UJEMY WAITPIDA NIE WYWO£A£ WCZEŒNIEJ METODY EXIT()
		else
		{
			// STAN PROCESU MACIERZYSTEGO KTÓRY WYKONA£ WAITPIDA NA PROCESIE POTOMNYM KTÓRY NIE WYKONA£ EXITA ZMIENIAMY NA CZEKAJ¥CY
			this->state = stanProcesu::Czekaj¹cy;
			
			if(this!= Runqueue.curr) // Je¿eli nie robimy WaitPid na aktualnym
			{
				///SPRAWDZIÆ
				Runqueue.remove_from_run_queue(this);
			}
			if (Runqueue.curr->nazwa == "IDLE") // Je¿eli Aktualny jest IDLE to oddaje procesor
				Runqueue.sched_yield();
			else
			{
				Runqueue.sched_yield();
				Runqueue.remove_from_run_queue(this); 
			}
			// TWORZYMY SYGNAL TEGO ZE OBECNY PROCES CZEKA NA WYKONANIE SIÊ PROCESU O PIDZIE PODANYM JAKO ARGUMENT WAITPIDA I ZGLASZAMY TAKIE OCZEKIWANIE DODAJAC JE DO ODPOWIEDNIEJ LISTY
			waiting zglaszaneOczekiwanie = { this->PID, PID };
			waits.push_back(zglaszaneOczekiwanie);
		}
	}
}

bool struct_task :: exit(int result)
{
	// FLAGA BEDZIE INFORMOWAC O TYM CZY PROCES ZOSTAL JUZ USUNIETY CZY STAL SIE ZOMBIE POPRZEZ ZWROCENIE WYNIKU TYPU BOOL; TRUE - USUNIETY
	bool Usuniety = false;

	// SPRAWDZAMY CZY PROCES MACIERZYSTY WYWOLAL JU¯ WAITPIDA DLA PROCESU POTOMNEGO KTORY AKTUALNIE KONCZYMY
	int index = -1;
	for (int i = 0; i < waits.size(); i++)
		if (waits.at(i).na == this->PID) { index = i; break; }

	// PROCES MACIERZYSTY WYWO£A£ WAITPIDA NA AKTUALNIE KONCZONYM PROCESIE POTOMNYM
	if (index >= 0)
	{
		// SPRAWDZAMY CZY PROCES MACIERZYSTY KONCZACEGO SIE PROCESU WYWOLAL NA NIM WAITPIDA ORAZ JESLI TAK TO CZY TYLKO NA NIEGO CZY MOZE JESZCZE NA INNE SWOJE PROCESY POTOMNE  - LICZYMY WSZYSTKIE SYGNALY OCZEKIWANIA
		int naIlePotomnychCzeka = 0; bool czyCzekaNaNiego = false; int indexSygnalu = 0;
		for (int i = 0; i < waits.size(); i++)
		{
			if (waits.at(i).kto == this->PPID)
			{
				++naIlePotomnychCzeka;
				if (waits.at(i).na == this->PID) { czyCzekaNaNiego = true; indexSygnalu = i; }
			}
		}

		// JESLI PROCES MACIERZYSTY CZEKAL TYLKO NA PROCES KTORY AKTUALNIE ZABIJAMY TO MOZEMY ZMIENIC JEGO STAN SPOWROTEM NA GOTOWY O ILE NIE JEST TO INIT
		if (czyCzekaNaNiego && naIlePotomnychCzeka == 1)
		{
			if(this->pptr->nazwa!="INIT") // INITA NIE DODAJEMY DO GOTOWYCH
			{
				this->pptr->state = stanProcesu::Gotowy;
				Runqueue.add_to_runqueue(this->pptr);
			}
			
			// Z LISTY SYGNALOW USUWAMY TEN PROCESU MACIERZYSTEGO PROCESU USUWANEGO W KTORYM INFORMUJE ON ¯E CZEKA NA PROCES USUWANY
			waits.erase((waits.begin() + indexSygnalu));

			

		}
		// PROCES MACIERZYSTY ZGLOSIL WAITPIDA W OCZEKIWANIU NA TEN PROCES POTOMNY ALE ZGLOSIL ROWNIEZ WAITPIDA NA INNE PROCESY POTOMNE
		else if (czyCzekaNaNiego)
		{
			// Z LISTY SYGNALOW USUWAMY TEN PROCESU MACIERZYSTEGO PROCESU USUWANEGO W KTORYM INFORMUJE ON ¯E CZEKA NA PROCES USUWANY
			waits.erase((waits.begin() + indexSygnalu));
		}

		// PROCES USUNIETY - ZMIENIAMY INFORMUJACA O TYM FLAGE NA TRUE
		Usuniety = true;

		// ZABIJAMY PROCES CZYLI: ZNAJDUJEMY GO NA LIŒCIE CYKLICZNEJ WSZYSTKICH PROCESÓW, ZWALNIAMY ZAJMOWAN¥ PRZEZ NIEGO PAMIÊÆ, USUWAMY GO Z LISTY CYKLICZNEJ WSZYSTKICH PROCESÓW, INIT_TASK ADOPTUJE WSZYSTKIE JEGO POTOMNE
		kill(this->nazwa);
	}
	// PROCES MACIERZYSTY NIE WYWO£A£ DO TEJ PORY WAITPIDA NA AKTUALNIE KONCZONYM PROCESIE POTOMNYM
	else
	{
		// STAN KONCZONEGO PROCESU ZMIENIAMY NA ZOMBIE, NIE ZOSTANIE ON USUNIETY Z LISTY CYKLICZNEJ PROCESOW DO CZASU WYKONANIA PRZEZ JEGO PROCES MACIERZYSTY NA NIM WAITPIDA
		if(this->nazwa!="IDLE")
		this->state = stanProcesu::Zombie;
	

		// TWORZYMY SYGNAL TEGO ZE OBECNY PROCES SIE WYKONAL ORAZ CZEKA W STANIE ZOMBIE NA WYKONANIE PRZEZ JEGO PROCES MACIERZYSTY NA NIM WAITPIDA I ZGLASZAMY TAKI SYGNAL DO ODPOWIEDNIEJ LISTY
		exiting zglaszaneZakonczenie = { this->PID, result };
		exits.push_back(zglaszaneZakonczenie);
	}

	return Usuniety;
}

bool kill(string nazwa)
{
	/*USTAWIAMY FLAGÊ, KTÓRA INFORMOWAÆ BÊDZIE CZY USUNELIŒMY JAKIŒ PROCES - ZMIENI SIÊ GDY ZNAJDZIEMY PROCES O WSKAZANEJ NAZWIE NA LIŒCIE CYKLICZNEJ WSZYSTKICH PROCESÓW*/
	bool Usuniety = false;

	/*TWORZYMY WSKANIK NA POCZ¥TEK LISTY CYKLICZNEJ WSZYSTKICH PROCESÓW - CZYLI INITa*/
	struct_task *wsk = &INIT_TASK;

	/*ROBIMY POJEDYÑCZ¥ PÊTLE PO LIŒCIE WSZYSTKICH PROCESÓW ( DO KOLEJNEGO NAPOTKANIA INITA ) BY ZNALEÆ PROCES KTÓRY CHCEMY ZABIÆ*/
	while (wsk->next_task != &INIT_TASK)
	{
		/*NA LIŒCIE WSZYSTKICH PROCESÓW ZNALELIŒMY PROCES O NAZWIE KTÓR¥ PODANO JAKO NAZWA PROCESU DO ZABICIA*/
		if (wsk->next_task->nazwa == nazwa)
		{
//			
			// SPRAWDZAMY CZY PROCES MACIERZYSTY USUWANEGO PROCESU WYWOLAL NA NIM WAITPIDA ORAZ JESLI TAK TO CZY TYLKO NA NIEGO CZY MOZE JESZCZE NA INNE SWOJE PROCESY POTOMNE  - LICZYMY WSZYSTKIE SYGNALY OCZEKIWANIA
			int naIlePotomnychCzeka = 0; bool czyCzekaNaNiego = false; int indexSygnalu = 0;
			for (int i = 0; i < waits.size(); i++)
			{
				if (waits.at(i).kto == wsk->next_task->PPID)
				{
					++naIlePotomnychCzeka;
					if (waits.at(i).na == wsk->next_task->PID) { czyCzekaNaNiego = true; indexSygnalu = i; }
				}
			}

			// JESLI PROCES MACIERZYSTY CZEKAL TYLKO NA PROCES KTORY AKTUALNIE ZABIJAMY TO MOZEMY ZMIENIC JEGO STAN SPOWROTEM NA GOTOWY
			if (czyCzekaNaNiego && naIlePotomnychCzeka == 1)
			{
				///TU POTENCJALNIE MO¯EMY DODAWAÆ INITA DO RUNQUEUE - SPRAWDZIÆ - JE¯ELI BY£ WAITPID PRZED EXITEM DZIECKA
				wsk->next_task->pptr->state = struct_task::stanProcesu::Gotowy;
				Runqueue.add_to_runqueue(wsk->next_task->pptr);

				// Z LISTY SYGNALOW USUWAMY TEN PROCESU MACIERZYSTEGO PROCESU USUWANEGO W KTORYM INFORMUJE ON ¯E CZEKA NA PROCES USUWANY
				waits.erase((waits.begin() + indexSygnalu));
			}
			// PROCES MACIERZYSTY ZGLOSIL WAITPIDA W OCZEKIWANIU NA TEN PROCES POTOMNY ALE ZGLOSIL ROWNIEZ WAITPIDA NA INNE PROCESY POTOMNE
			else if (czyCzekaNaNiego)
			{
				// Z LISTY SYGNALOW USUWAMY TEN PROCESU MACIERZYSTEGO PROCESU USUWANEGO W KTORYM INFORMUJE ON ¯E CZEKA NA PROCES USUWANY
				waits.erase((waits.begin() + indexSygnalu));
			}

			/*ZAPISUJEMY USUWANY PROCES W POMOCNICZNYM WSKANIKU*/
			struct_task* usuwany = wsk->next_task;

			/*JE¯ELI USUWANY PROCES JEST PIERWSZYM Z PROCESÓW POTOMNYCH PROCESU INIT_TASK NALE¯Y JEGO WSKANIK NA PIERWSZE DZIECKO USTAWIÆ NA M£ODSZE RODZEÑSTWO USUWANEGO PROCESU*/
			if (usuwany == INIT_TASK.cptr) INIT_TASK.cptr = usuwany->ysptr;

			/*OD£¥CZAMY USUWANY PROCES OD WSZELKICH POWI¥ZAÑ Z LIST¥ CYKLICZN¥ WSZYSTKICH PROCESÓW*/
			wsk->next_task = wsk->next_task->next_task;
			wsk->next_task->prev_task = wsk;

			/*OD£¥CZAMY USUWANY PROCS OD WSZELKICH POWI¥ZAÑ RODZINNYCH*/

			if (usuwany->osptr) usuwany->osptr->ysptr = usuwany->ysptr;
			if (usuwany->pptr->cptr == usuwany) usuwany->pptr->cptr = usuwany->ysptr;

			/*SPRAWDZAMY CZY USUWANY PROCES MA PROCESY POTOMNE - JEŒLI TAK, ZOSTAN¥ ONE ZAADOPTOWANE PRZEZ PROCES INIT_TASK*/

			/*PROCES USUWANY MA PROCESY POTOMNE, KTÓRE NALE¯Y ZAADOPTOWAÆ*/
			if (usuwany->cptr)
			{
				struct_task *wsk = usuwany->cptr;
				while (wsk)
				{
					wsk->PPID = 1;
					wsk->pptr = &INIT_TASK;
					wsk = wsk->ysptr;
				}

				// INIT NIE MA POTOMNYCH
				if (!INIT_TASK.cptr)
					INIT_TASK.cptr = usuwany->cptr;
				// INIT MA POTOMNE, ADOPTOWANE ZOSTAN¥ DOPISANE
				else
				{
					struct_task *ostatni = INIT_TASK.cptr;
					while (ostatni->ysptr) ostatni = ostatni->ysptr;

					ostatni->ysptr = usuwany->cptr;
					usuwany->cptr->osptr = ostatni;
				}
			}
			/*ZWALNIAMY PAMIÊÆ RAM ZAJMOWAN¥ PRZEZ PROCES */
			pamiec.zwolnijPamiec(usuwany->nazwa);	// OK 
			if(Runqueue.curr == usuwany) 
				Runqueue.curr = nullptr;
			/*DEALOKUJEMY PAMIÊÆ ZE WSKANIKA NA USUWANY PROCES*/
			delete usuwany;

			/*ZMIENIAMY STAN FLAGI NA POTWIERDZAJ¥CY USUNIÊCIE PROCSU*/
			Usuniety = true;

			/*SKORO ZNALELIŒMY SZUKANY W CELU USUNIÊCIA PROCES NIE MA SENSU SZUKAÆ GO NA LIŒCIE DALEJ DLATEGO KOÑCZYMY*/
			break;
		}

		/*NIE ZNALELIŒMY JESZCZE SZUKANEGO PRZEZ NAS PROCESU DO ZABICIA DLATEGO TE¯ PRZESUWAMY WSKANIK NA NASTÊPNY ELEMENT LISTY CYKLICZNEJ WSZYSTKICH PROCESÓW*/
		wsk = wsk->next_task;
	}

	/*ZWRACAMY FLAGÊ KTÓRA INFORMUJE O TYM CZY ZNALEZIONO PROCES O WSKAZANEJ NAZWIE I GO USUNIÊTO*/
	return Usuniety;
}


// METODA ZAREZERWOWANA DLA PROCESU IDLE I PROCESU INIT_TASK - PODSTAWOWA INICJALIZACJA
void init()
{
	IDLE.state = struct_task::stanProcesu::Aktywny;
	IDLE.nazwa = "IDLE"; 
	IDLE.PID = 0; INIT_TASK.PID = 1; INIT_TASK.PPID = 0;
	IDLE.cptr = &INIT_TASK;
	Runqueue.idle = &IDLE;

	INIT_TASK.nazwa = "INIT";
	INIT_TASK.state = struct_task::stanProcesu::Czekaj¹cy;
	INIT_TASK.next_task = &IDLE; IDLE.prev_task = &INIT_TASK; IDLE.next_task = &INIT_TASK;
	INIT_TASK.prev_task = &IDLE;
	INIT_TASK.pptr = &IDLE;
	
	Runqueue.curr = &IDLE;
	Runqueue.curr->priority = 7;
	
}

// PRZECI¥¯AMY OPERATOR STRUMIENIA WYJŒCIOWEGO W CELU MO¯LIWOŒCI WYŒWIETLENIA STANÓW PROCESU
/*ostream& operator<<(ostream& strumien, struct_task::stanProcesu stan)
{
	switch (stan)
	{
	case struct_task::stanProcesu::Aktywny:
		strumien << "Aktywny";
		break;
	case struct_task::stanProcesu::Nowy:
		strumien << "Nowy";
		break;
	case struct_task::stanProcesu::Gotowy:
		strumien << "Gotowy";
		break;
	case struct_task::stanProcesu::Czekaj¹cy:
		strumien << "Czekaj¹cy";
		break;
	case struct_task::stanProcesu::Zakoñczony:
		strumien << "Zakoñczony";
		break;
	case struct_task::stanProcesu::Zombie:
		strumien << "Zombie";
		break;
	
	case struct_task::stanProcesu::Idle: 
		strumien << "Zombie";
		break;
	default: break;
	}

	return strumien;
}*/

// METODA PRZEZNACZONA DO UKAZANIA NA EKRANIE WSZYSTKICH PROCESÓW ZNAJDUJ¥CYCH SIÊ NA LIŒCIE CYKLICZNEJ B¥D KONKRETNEGO PROCESU JESLI JAKO PARAMETR PODAMY JEGO NAZWE
void showProcess(string nazwa = "all")
{
	if (nazwa == "all")
	{
		showProcess("IDLE");

		auto wsk = &IDLE;

		while (wsk->next_task->nazwa != "IDLE")
		{
			wsk = wsk->next_task;

			cout << "Proces [" << wsk->nazwa << "]:" << endl << endl;

			cout << "  Stan -> " << wsk->state << endl << endl;

			cout << "  PID = " << wsk->PID << endl;
			cout << "  PPID = " << wsk->PPID << endl << endl;

			cout << "  A = " << wsk->A << endl;
			cout << "  B = " << wsk->B << endl;
			cout << "  C = " << wsk->C << endl << endl;
			cout << "  counter = " << wsk->counter << endl << endl;

			cout << "  credit = " << wsk->credit << endl;
			cout << "  priority = " << wsk->priority << endl;

			cout << endl;
		}
	}
	else
	{
		auto wsk = find_by_name(nazwa);
		if (wsk)
		{
			cout << "Proces [" << wsk->nazwa << "]:" << endl << endl;
			cout << "  Stan -> " << wsk->state << endl << endl;
			cout << "  PID = " << wsk->PID << endl;
			cout << "  PPID = " << wsk->PPID << endl << endl;

			cout << "  A = " << wsk->A << endl;
			cout << "  B = " << wsk->B << endl;
			cout << "  C = " << wsk->C << endl << endl;
			cout << "  counter = " << wsk->counter << endl << endl;

			cout << "  credit = " << wsk->credit << endl;
			cout << "  priority = " << wsk->priority << endl;

			cout << endl;
		}
	}
}

//METODA DLA DANIELA - WSTRZYMYWANIE PROCESU 08-12-15
void struct_task::wait(bool stan)
{
	if (stan) // Zmiaiamy na czekaj¹cy
	{
		state = stanProcesu::Czekaj¹cy;
		Runqueue.remove_from_run_queue(this);
		Runqueue.curr = nullptr;
	}
	else
	{
		state = stanProcesu::Gotowy;
		//this->counter--;
		Runqueue.add_to_runqueue(this);
	}
}