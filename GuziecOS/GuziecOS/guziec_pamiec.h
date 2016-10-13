//Szymon Nowak

#pragma once;
#include<iostream>
#include<cstdlib>
#include<string>

using namespace std;

DYSK *Dysk;

void setDysk(DYSK *Tomka){
	Dysk = Tomka;
} // DO SPRAWDZENIA

string zwrocPustyString() {
	string pomocniczy = "";
	for (int i = 0; i < 16; i++) {
		pomocniczy = pomocniczy + char(0);
	}
	return pomocniczy;
}


string stronaNaString(char * poczatekStrony) {
	string pomocniczy = "";
	int i = 0;
	while (poczatekStrony[i] != 0)
		pomocniczy += poczatekStrony[i];
	return pomocniczy;
}

class Strona {
private:
	char dane[16];
public:
	Strona() {
		for (int i = 0; i < 16; i++)
			dane[i] = 0;
	}
	Strona(string n) {
		int rozmiar;
		if (n.length() > 16)
			rozmiar = 16;
		else
			rozmiar = (int)n.length();
		for (int i = 0; i < 16; i++) {
			if (i < rozmiar)
				dane[i] = n[i];
			else
				dane[i] = 0;

		}
	}
	void setDane(char pomocniczy[]) {
		for (int i = 0; i < 16; i++)
			dane[i] = pomocniczy[i];
	}
	void setDaneString(string n) {
		int rozmiar = (int)n.length();
		if (rozmiar >= 16)
			rozmiar = 16;

		for (int i = 0; i < 16; i++) {
			if (i < rozmiar)
				dane[i] = n[i];
			else
				dane[i] = 0;
		}
	}
	void setDaneById(char pomocniczy, int idPomocniczy) {
		dane[idPomocniczy] = pomocniczy;
	}
	char * getPoleRamkiById(int i) {
		return &dane[i];
	}
	char * getDane() {
		return dane;
	}
	string getDaneString() {
		string pomoc = "";
		for (int i = 0; i < 16; i++)
			pomoc += dane[i];
		return pomoc;
	}
	string getDaneJakoString() {
		string pomoc = "";
		for (int i = 0; i < 16; i++)
			pomoc = pomoc + dane[i];
		return pomoc;
	}
	void wypiszStroneZnakowo() {
		cout << "  ";
		for (int i = 0; i < 16; i++) {
			cout << dane[i] << "   ";
		}
		cout << endl;
	}
	void wypiszStroneLiczbowo() {
		cout << "  ";
		for (int i = 0; i < 16; i++) {
			if (dane[i] < 10)
				cout << int(dane[i]) << "   ";
			else if (dane[i] < 100)
				cout << int(dane[i]) << "  ";
			else
				cout << int(dane[i]) << " ";
		}
		cout << endl;
	}
	void wyzerujeStrone() {
		for (int i = 0; i < 16; i++)
			dane[i] = 0;
	}
	void wypiszNumerki() {
		cout << "  ";
		for (int i = 0; i < 16; i++) {
			if (i < 10)
				cout << i << "   ";
			else
				cout << i << "  ";
		}
		cout << endl;
		cout << "__";
		for (int i = 0; i < 16; i++)
			cout << "____";
		cout << endl;
	}
};

class MapaStrona {
private:
	Strona * strona;
	bool zajety;
public:
	MapaStrona() : strona{ nullptr }, zajety{ false } {
	}
	void setZajety(bool i) { zajety = i; }
	void setStrona(Strona * i) { strona = i; }
	bool getZajety() { return zajety; }
	Strona * getStrona() { return strona; }
};

//Strona * globalnaTablicaStron[32];

//Metoda Tomkowa
/*string przekaz_fragment_pliku(string nazwa, int numer_fragmentu = 0) {

if (nazwa == "test")
{
if (numer_fragmentu == 0)
return "sektor 0 de    d";
else if (numer_fragmentu == 1)
return "sektor 1 opjgs s";
else if (numer_fragmentu == 2)
return "sektor 2   swa 4";
else if (numer_fragmentu == 3)
return "sektor 3 niamnia";
}
if (nazwa == "test2")
{
if (numer_fragmentu == 0)
return "adkiwnhmskohksuj";
else if (numer_fragmentu == 1)
return "85sdeju526258542";
else if (numer_fragmentu == 2)
return "IUJLIDLSw3939dkk";
else if (numer_fragmentu == 3)
return "852    jjj jjj u";
}
}*/
/*string przekaz_fragment_pliku2(string nazwa, int numer_fragmentu = 0) {
if (numer_fragmentu == 0)
return "adkiwnhmskohksuj";
else if (numer_fragmentu == 1)
return "85sdeju526258542";
else if (numer_fragmentu == 2)
return "IUJLIDLSw3939dkk";
else if (numer_fragmentu == 3)
return "852    jjj jjj u";
}*/
/*
//Druga metoda Tomkowa
int przekaz_ilosc_sektorow(string nazwa) {
return 4;
}
*/

class Segment {
private:
	Strona * strona[32];
	int rozmiarSegmentu;
public:
	Segment() : rozmiarSegmentu{ 0 } {
		for (int i = 0; i < 32; i++) {
			strona[i] = nullptr;
		}
	}
	//przekazuje wskaznik do tablicy wskaznikow na przydzielone n stron
	Segment(Strona ** stronaPomocniczy, int n) : Segment() {
		rozmiarSegmentu = n;
		for (int i = 0; i < n; i++)
			strona[i] = stronaPomocniczy[i];
	}
	Segment(int n) : Segment() {
		for (int i = 0; i < n; i++)
			strona[i] = &Strona();
	}
	void setRozmiarSegmentu(int i) {
		rozmiarSegmentu = i;
	}
	int getRozmiarSegmentu() {
		return rozmiarSegmentu;
	}
	Strona * getStrona(int i);
	void setStronaById(Strona * stronaPomocniczy, int i) {
		strona[i] = stronaPomocniczy;
	}
	void zapiszZPliku(string nazwaPliku);
	void wypiszNumerki() {
		cout << "  ";
		for (int i = 0; i < 16; i++) {
			if (i < 10)
				cout << i << "   ";
			else
				cout << i << "  ";
		}
		cout << endl;
		cout << "__";
		for (int i = 0; i < 16; i++)
			cout << "____";
		cout << endl;
	}
	void wypiszStronyZnakowo() {
		wypiszNumerki();
		if (rozmiarSegmentu == 0) {
			cout << "Brak ramek w segmencie";
			return;
		}
		for (int i = 0; i < rozmiarSegmentu; i++)
			strona[i]->wypiszStroneZnakowo();
	}
	void wypiszStronyLiczbowo() {
		wypiszNumerki();
		if (rozmiarSegmentu == 0) {
			cout << "Brak ramek w segmencie";
			return;
		}
		for (int i = 0; i < rozmiarSegmentu; i++) {
			strona[i]->wypiszStroneLiczbowo();
		}
	}
	bool czyZawieraStrone(Strona * stronaPomoc) {
		for (int i = 0; i < 32; i++) {
			if (strona[i] == stronaPomoc)
				return true;
		}
		return false;
	}
	int getIdStrony(Strona * stronaPomoc) {
		for (int i = 0; i < rozmiarSegmentu; i++) {
			if (strona[i] == stronaPomoc)
				return i;
		}
		cout << "Blad segmentu" << endl;
		return 0;
	}
};

class MapaSegmentu {
private:
	Segment * segment;
	string nazwaProcesu;
	bool zajety;
public:
	MapaSegmentu() : nazwaProcesu(""), zajety(false) {
		segment = new Segment();
	}
	MapaSegmentu(string nazwaPliku) : MapaSegmentu() {
		nazwaProcesu = nazwaPliku;
		zajety = true;
		segment->setRozmiarSegmentu(Dysk->przekaz_ilosc_sektorow(nazwaPliku));
		for (int i = 0; i < segment->getRozmiarSegmentu(); i++)
			segment->getStrona(i)->setDaneString(Dysk->przekaz_fragment_pliku(nazwaPliku, i));
	}
	void setSegment(Segment * i) { segment = i; }
	void setNazwaProcesu(string i) { nazwaProcesu = i; }
	void setZajety(bool i) { zajety = i; }
	Segment * getSegment() { return segment; }
	string getNazwaProcesu() { return nazwaProcesu; }
	bool getZajety() { return zajety; }
	bool zwolnij();
};

class BuddySystem {
private:
	MapaSegmentu mapaSegmentu[32];
public:
	BuddySystem() {
		for (int i = 0; i < 32; i++) {
			mapaSegmentu[i] = MapaSegmentu();
			mapaSegmentu[i].setZajety(false);
			mapaSegmentu[i].setSegment(new Segment());
		}
	}
	MapaSegmentu * znajdzPierwszyWolny() {
		for (int i = 0; i < 32; i++) {
			if (mapaSegmentu[i].getZajety() == false)
				return &mapaSegmentu[i];
		}
		return nullptr;
	}
	bool utworzSegment(string nazwa);
	Segment * znajdzPoNazwieProcesu(string nazwa) {
		for (int i = 0; i < 32; i++) {
			if (mapaSegmentu[i].getNazwaProcesu() == nazwa)
				return mapaSegmentu[i].getSegment();
		}
		cout << "Nie znaleziono";
		return nullptr;
	}
	Segment * znajdzPoStronie(Strona * strona) {
		for (int i = 0; i < 32; i++) {
			if (mapaSegmentu[i].getSegment()->czyZawieraStrone(strona))
				return mapaSegmentu[i].getSegment();
		}
		cout << "Dana strona jest wolna"; //Bo nie jest przypisana do zadnego procesu
		return nullptr;
	}
	MapaSegmentu * znajdzMapeSegmentuPoNazwieProcesu(string nazwa) {
		for (int i = 0; i < 32; i++) {
			if (mapaSegmentu[i].getNazwaProcesu() == nazwa)
				return &mapaSegmentu[i];
		}
		cout << "Nie znaleziono ";
		return nullptr;
	}
	MapaSegmentu * getMapaSegmentu(int i) {
		return &mapaSegmentu[i];
	}
	MapaSegmentu * znajdzMapaSegmentuPoStronie(Strona * strona) {
		for (int i = 0; i < 32; i++) {
			if (mapaSegmentu[i].getSegment()->czyZawieraStrone(strona))
				return &mapaSegmentu[i];
		}
		//cout << "Dana strona jest wolna"; //Bo nie jest przypisana do zadnego procesu
		return nullptr;
	}
	int getIdSegmentu(MapaSegmentu * pomocniczy) {
		for (int i = 0; i < 32; i++) {
			if (&mapaSegmentu[i] == pomocniczy)
				return i;
		}
		return -1; //nigdy nie dojdzie
	}
};

//FIFO
class Kolejka {
private:
	Kolejka * nastepny;
	MapaStrona * strona;

	Kolejka() : nastepny(nullptr), strona(nullptr) {
	}
public:
	//konstruktor instancji kolejki w pamieci
	//musialem uzyc innej opcji tworzenia, poniewaz potrzebuje kolejke, ktora bedzie posiadala glowe, czyli element kolejki 
	//ktory nie zawiera w sobie odnosnika do strony, a jedynie wskaznik na nastepny element, ktory strone juz posiada.
	static Kolejka * init() {
		Kolejka * pomocniczy = new Kolejka();
		pomocniczy->nastepny = new Kolejka();
		return pomocniczy;
	}
	void dodajNaKoncu(MapaStrona * stronaPomocniczy) {
		Kolejka * pomocniczy = zwrocOstatniaStroneWKolejce();
		pomocniczy->strona = stronaPomocniczy;
		pomocniczy->nastepny = new Kolejka();
	}
	Kolejka * zwrocOstatniaStroneWKolejce() {
		Kolejka * pomocniczy = nastepny;
		while (true) {
			if (pomocniczy->nastepny != nullptr)
				pomocniczy = pomocniczy->nastepny;
			else //(pomocniczy->nastepny == nullptr)
				return pomocniczy;
		}
	}
	//zwraca juz pusta strone
	//dac na koniec i odkomentowac to co jest z plikiem
	Strona * zwolnijNajstarszaStrone();
	void usunZKolejki(Kolejka * kolejka) {
		Kolejka * pomocniczy = this;
		while (pomocniczy != nullptr) {
			if (pomocniczy->nastepny != kolejka)
				pomocniczy = pomocniczy->nastepny;
			else {
				//nastepna strona to ta do zwolnienia
				Kolejka * pomocniczy2 = pomocniczy->nastepny;
				pomocniczy->nastepny = pomocniczy2->nastepny;
				pomocniczy2->nastepny = nullptr;
				pomocniczy2->strona->getStrona()->wyzerujeStrone();
				pomocniczy2->strona->setZajety(false);
				pomocniczy2->strona = nullptr;
				delete pomocniczy2;
				pomocniczy2 = nullptr;
				break;
			}
		}
	}
	void wypiszStruktureKolejki();
	void zwolnijPamiecProcesu(Segment * segment) {
		Kolejka * pomocniczy;
		for (int i = 0; i < segment->getRozmiarSegmentu(); i++) {
			pomocniczy = this->nastepny;
			while (pomocniczy->nastepny != nullptr) {
				if (segment->getStrona(i) == (pomocniczy->strona->getStrona())) {
					usunZKolejki(pomocniczy);
					segment->setStronaById(nullptr, i);
					break;
				}
				pomocniczy = pomocniczy->nastepny;
			}
		}
		segment->setRozmiarSegmentu(0);
	}
	void wypiszNajstarszaStrone();
};

class Pamiec {
private:
	MapaStrona mapaStrona[32];
	BuddySystem buddySystem;
	Kolejka kolejka;
	Strona * zarezerwowanaStrona;
public:
	BuddySystem * getBuddySystem() { return &buddySystem; }
	Kolejka * getKolejka() { return &kolejka; }
	Pamiec() : buddySystem(), kolejka(*Kolejka::init()) {
		for (int i = 0; i < 32; i++) {
			mapaStrona[i].setStrona(new Strona());
			mapaStrona[i].setZajety(false);
		}
		zarezerwowanaStrona = new Strona();
	}
	Strona * getWolnaStrona() {
		for (int i = 0; i < 32; i++) {
			if (mapaStrona[i].getZajety() == false) {
				mapaStrona[i].setZajety(true);
				Strona * strona = mapaStrona[i].getStrona();
				return strona;
			}
		}
		kolejka.zwolnijNajstarszaStrone();
		return getWolnaStrona();
	}
	MapaStrona * zwrocMapaStronaPoStrona(Strona * strona) {
		for (int i = 0; i < 32; i++) {
			if (mapaStrona[i].getStrona() == strona)
				return &mapaStrona[i];
		}
		return nullptr;
	}
	int zwrocPozycjeStrony(Strona * strona) {
		for (int i = 0; i < 32; i++) {
			if (strona == mapaStrona[i].getStrona())
				return i;
		}
		return -1;
	}

	//Dla Daniela 
	//Zwraca wskaznik na pierwszy element 16-sto bajtowego obszaru
	char * getZarezerwowanaStrona() {
		return zarezerwowanaStrona->getDane();
	}
	//Ustawia w buforze Daniela znak na i-tej pozycji (i = 0 - 15)
	bool setDane(string pomocniczy) {
		int pozycjaOstatniegoPrzecinka = 0;
		string temp = zarezerwowanaStrona->getDane();
		int rozmiar = (int)pomocniczy.length() + 1;

		for (int i = 15; i >= 0; i--) {
			if (temp[i] == ',') {
				pozycjaOstatniegoPrzecinka = i + 1;
				break;
			}
		}

		if (rozmiar + pozycjaOstatniegoPrzecinka > 15) {
			cout << "Nie udalo sie zapisac. Bufor przepelniony" << endl;
			return false;
		}
		else {
			string doWpisania = temp.substr(0, pozycjaOstatniegoPrzecinka) + pomocniczy + ',' + "..........................................";
			for (int i = 0; i < 16; i++) {
				zarezerwowanaStrona->setDaneById(doWpisania[i], i);
			}
			return true;
		}
	}
	//Zapis bufora do Tomka i czyszczenie
	void zapiszBuforNaDysku();
	//Dla Mateusza
	//Ustala odpowiednia ilosc stron, zapisuje te dane z dysku i zwraca true jesli udalo sie
	bool zarezerwujPamiec(string nazwaProcesu) {
		return buddySystem.utworzSegment(nazwaProcesu);
	}
	//Zwalnia pamiec 
	bool zwolnijPamiec(string nazwaProcesu) {
		return buddySystem.znajdzMapeSegmentuPoNazwieProcesu(nazwaProcesu)->zwolnij();
	}
	//Leszek
	//pobiera jeden rozkaz
	string pobierzRozkaz(string nazwaProcesu, int licznik) {
		MapaSegmentu * segment = buddySystem.znajdzMapeSegmentuPoNazwieProcesu(nazwaProcesu);
		Strona * strona = segment->getSegment()->getStrona(licznik / 2);
		if (strona == nullptr) {
			cout << "Strona skojarzona z potrzebnym rozkazem zostala wymieniona na inna." << endl;
			cout << "Nastepuje ponowne pobranie strony" << endl;
			segment->getSegment()->setStronaById(getWolnaStrona(), licznik / 2);
			strona = segment->getSegment()->getStrona(licznik / 2);
			strona->setDaneString(Dysk->przekaz_fragment_pliku(nazwaProcesu, (licznik / 2)));
		}
		string rozkazy = strona->getDaneString(); //podpiac wymiane stron, jesli nie znaleziono

		if (licznik % 2 == 0)
			return rozkazy.substr(0, 8);
		else
			return rozkazy.substr(8, 8);
	}

	void wypiszPamiecFizyczna()	{
		for (int i = 0; i < 32; i++) {
			if (i % 8 == 0)
				cout << endl;
			if (i < 10)
				cout << "Strona[" << i << "]:  ";
			else
				cout << "Strona[" << i << "]: ";
			mapaStrona[i].getStrona()->wypiszStroneZnakowo();
		}
		cout << endl;
	}
	void wypiszPamiecWirtualna() {
		bool flaga;
		Segment * segment;
		for (int i = 0; i < 32; i++) {
			flaga = false;
			if (buddySystem.getMapaSegmentu(i)->getZajety()) {
				segment = buddySystem.getMapaSegmentu(i)->getSegment();
				if (i < 10)
					cout << "Segment " << i << ":  ";
				else
					cout << "Segment " << i << ": ";
				for (int j = 0; j < segment->getRozmiarSegmentu(); j++) {
					if (segment->getStrona(j) == nullptr)
						continue;
					else {
						if (flaga == true) {
							cout << "            ";
							segment->getStrona(j)->wypiszStroneZnakowo();
						}
						else {
							segment->getStrona(j)->wypiszStroneZnakowo();
							flaga = true;
						}
					}
					if (j != 0)
						cout << "            ";
					segment->getStrona(j)->wypiszStroneZnakowo();
				}
				cout << endl;
			}
		}
	}
	void wypiszPamiec() {
		MapaSegmentu * mapaSegmentu;
		for (int i = 0; i < 32; i++) {
			mapaSegmentu = buddySystem.znajdzMapaSegmentuPoStronie(mapaStrona[i].getStrona());
			if (mapaSegmentu != nullptr) {
				if (i < 10)
					cout << "Strona " << i << ":   ";
				else
					cout << "Strona " << i << ":  ";
				//MapaSegmentu * mapaSegmentu = buddySystem.znajdzMapaSegmentuPoStronie(mapaStrona[i].getStrona());
				cout << "Segment " << buddySystem.getIdSegmentu(mapaSegmentu) << ": Procesu: " << mapaSegmentu->getNazwaProcesu() << endl;
			}
		}
	}
	void wypiszBufor() {
		cout << "\nBufor wymiany miedzy procesowej" << endl;
		for (int i = 0; i < 16; i++)
			cout << i << ". ";

		cout << endl;
		for (int i = 0; i < 16; i++)
		if (i < 10)
			cout << "___";
		else
			cout << "____";

		cout << endl;

		for (int i = 0; i < 16; i++) {
			if (i < 10)
				cout << zarezerwowanaStrona->getDane()[i] << "  ";
			else
				cout << zarezerwowanaStrona->getDane()[i] << "   ";
		}
		cout << endl;

	}
	void wypiszNajstarszaStrone() {
		kolejka.wypiszNajstarszaStrone();
	}
};

Pamiec pamiec;

bool BuddySystem::utworzSegment(string nazwa) {
	MapaSegmentu * mapaSegmentu = znajdzPierwszyWolny();
	if (mapaSegmentu == nullptr) {
		cout << "Blad pamieci wirtualnej." << endl;
		cout << "W pamieci wirtualnej nie ma juz miejsca na umieszczenie kolejnego procesu." << endl;
		return false;
	}
	mapaSegmentu->setNazwaProcesu(nazwa);
	mapaSegmentu->setZajety(true);
	Segment * segment = mapaSegmentu->getSegment();
	segment->setRozmiarSegmentu(Dysk->przekaz_ilosc_sektorow(nazwa));

	for (int i = 0; i < segment->getRozmiarSegmentu(); i++) {
		segment->setStronaById(pamiec.getWolnaStrona(), i);
		segment->getStrona(i)->setDaneString(Dysk->przekaz_fragment_pliku(nazwa, i));
		pamiec.getKolejka()->dodajNaKoncu(pamiec.zwrocMapaStronaPoStrona(segment->getStrona(i)));
	}

	return true;
}
bool MapaSegmentu::zwolnij() {

	nazwaProcesu = "";
	zajety = false;
	pamiec.getKolejka()->zwolnijPamiecProcesu(segment);
	//i musi byc jeszcze wywalony ze struktury buddysystem
	return true;

	/* BY£O - CHCÊ ZAMIENIÆ DWIE PIERWSZE LINIE Z PAMIEC.GET....
	nazwaProcesu = "";
	zajety = false;
	pamiec.getKolejka()->zwolnijPamiecProcesu(segment);
	//i musi byc jeszcze wywalony ze struktury buddysystem
	return true;
	*/
}
Strona * Kolejka::zwolnijNajstarszaStrone() {
	//1. Usuwanie z kolejki
	Kolejka * pomocniczy = this->nastepny;
	if (this->nastepny == nullptr) {
		cout << "Nie ma czego zwalniac!";
		return nullptr;
	}
	this->nastepny = pomocniczy->nastepny;
	pomocniczy->nastepny = nullptr;
	//2. Usuwanie z pamieci fizycznej
	MapaStrona * mapaStrona = pomocniczy->strona;
	Strona * strona = mapaStrona->getStrona();
	strona->wyzerujeStrone();
	mapaStrona->setZajety(false);
	//3. Usuwanie z pamieci wirtualnej
	Segment * segment = pamiec.getBuddySystem()->znajdzPoStronie(strona);
	int i = segment->getIdStrony(strona);
	segment->setStronaById(nullptr, i);
	mapaStrona->setZajety(false);
	//cout << "Pozycja strony " << pamiec.zwrocPozycjeStrony(strona) << endl;
	pomocniczy->strona = nullptr;
	delete pomocniczy;
	pomocniczy = nullptr;
	return strona;
}
void Segment::zapiszZPliku(string nazwaPliku) {
	rozmiarSegmentu = Dysk->przekaz_ilosc_sektorow(nazwaPliku);
	for (int i = 0; i < rozmiarSegmentu; i++) {
		string pomoc = Dysk->przekaz_fragment_pliku(nazwaPliku, i);
		strona[i] = pamiec.getWolnaStrona();
		strona[i]->setDaneString(pomoc);
		pamiec.getKolejka()->dodajNaKoncu(pamiec.zwrocMapaStronaPoStrona(strona[i]));
	}
}
Strona * Segment::getStrona(int i) {
	/*if (strona[i] == nullptr) {
	cout << "Strona z tym rozkazem zostala wymieniona na inna strone." << endl;
	cout <<"Nastepnuje ponowne pobranie strony z dysku";
	Strona * strona = pamiec.getKolejka()->zwolnijNajstarszaStrone();
	return strona;
	}*/
	return strona[i];
}
void Pamiec::zapiszBuforNaDysku() {
	string pomocniczy = "";
	for (int i = 0; i < 16; i++) {
		pomocniczy = pomocniczy + zarezerwowanaStrona->getDane()[i];
	}
	Dysk->wypelnij_plik_danymi("KOM", pomocniczy);
}
void Kolejka::wypiszStruktureKolejki() {
	Kolejka * pomocniczy = this->nastepny;
	while (pomocniczy->nastepny != nullptr) {
		cout << "(";
		cout << pomocniczy->strona->getStrona()->getDaneJakoString();
		cout << ")";
		//		cout << "\tWPamieci fizycznej na " << pamiec.zwrocPozycjeStrony(this->strona->getStrona()) << " pozycji" << endl;
		pomocniczy = pomocniczy->nastepny;
	}
}
void Kolejka::wypiszNajstarszaStrone() {
	if (nastepny->strona == nullptr) {
		cout << "W pamieci nie ma zadnych stron." << endl;
		return;
	}
	cout << "Najstarsza strona to";
	Strona * strona = nastepny->strona->getStrona();
	cout << " strona nr " << pamiec.getBuddySystem()->znajdzMapaSegmentuPoStronie(strona)->getSegment()->getIdStrony(strona) << endl;
	cout << "Jest ona skojarzona z procesem \"" << pamiec.getBuddySystem()->znajdzMapaSegmentuPoStronie(strona)->getNazwaProcesu() << "\"" << endl;
}

/*
void testZarezerwujPamiec() {
string test = "test";
pamiec.zarezerwujPamiec(test);
pamiec.getBuddySystem()->znajdzMapeSegmentuPoNazwieProcesu(test)->getSegment()->wypiszStronyZnakowo();

test = "test2";
pamiec.zarezerwujPamiec(test);
pamiec.getBuddySystem()->znajdzMapeSegmentuPoNazwieProcesu(test)->getSegment()->wypiszStronyZnakowo();
}
void testPobierzRozkaz() {
cout << pamiec.pobierzRozkaz("test", Dysk->przekaz_ilosc_sektorow("test")) << endl;
cout << pamiec.pobierzRozkaz("test2", Dysk->przekaz_ilosc_sektorow("test2")) << endl;

}
*/