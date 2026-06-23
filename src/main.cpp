#include <iostream>
#include "Coloana.h"
#include "Tabela.h"
#include "Rand.h"
#include "BazaDeDate.h"
#include "Interogare.h"




int main(int argc, char* argv[]) {
    vector<Tabela> tabele;
    BazaDeDate baza("mydb", tabele , 1);
    if (std::filesystem::exists("mydb")) {
        if (std::filesystem::is_directory("mydb")) {
            std::filesystem::current_path("mydb");
        }
        else {
            cout << "A fost gasit fisier, nu director.\n";
        }
    }
    else {
        cout << "Directorul nu exista, se creeaza unul.\n";
        std::filesystem::create_directory("mydb");
    }
    
    Interogare i(argv, argc);
    if (!i) {
        cout << "Comanda invalida!" << "\n";
        cout << i;
    }
    else i.executa(baza);
    
    
	return 0;
}