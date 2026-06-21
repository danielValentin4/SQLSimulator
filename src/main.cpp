#include <iostream>
#include "Coloana.h"
#include "Tabela.h"
#include "Rand.h"
#include "BazaDeDate.h"
#include "Interogare.h"



//g++ - I include src / main.cpp src / Coloana.cpp src / Tabela.cpp src / BazaDeDate.cpp src / Interogare.cpp src / Rand.cpp - o test.exe

int main(int argc, char* argv[]) {
    vector<Tabela> tabele;
    BazaDeDate baza("mydb", tabele , 1);
    baza.incarca();
    Interogare i(argv, argc);
    if (!i) {
        cout << "Comanda invalida!" << "\n";
        cout << i;
    }
    else i.executa(baza);
    baza.salveaza();
    
	return 0;
}