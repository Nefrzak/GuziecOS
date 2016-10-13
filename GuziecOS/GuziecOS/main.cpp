#include "guziec_sp.h"

int main()
{
	DYSK dysk_systemowy = DYSK();
	dysk_systemowy.stworz_folder("dcb");
	dysk_systemowy.stworz_plik("abc", "dcb");
	dysk_systemowy.wypelnij_plik_danymi("abc", "c://asd.txt");
	dysk_systemowy.wyswietl_pliki_i_foldery();
	dysk_systemowy.otworz_plik("abc");
	//dysk_systemowy.wyswietl_dysk_znakowo();
	//dysk_systemowy.usun_plik("abc");
	dysk_systemowy.zakoncz_prace_dysku();
	system("pause");
	return 0;
}