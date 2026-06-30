#include <iostream>
#include "Coloana.h"
#include "Tabela.h"
#include "Rand.h"
#include "BazaDeDate.h"
#include "Interogare.h"
#include "Server.h"




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
    
    /*Interogare i(argv, argc);
    if (!i) {
        cout << "Comanda invalida!" << "\n";
        cout << i;
    }
    else i.executa(baza);*/
    
    asio::io_context io;
    asio::thread_pool dbPool(4);

    Server server(io, dbPool, 5000, baza);
    unsigned int n = std::thread::hardware_concurrency();
    vector<std::thread> threads;
    threads.reserve(n);
    for (unsigned int i = 0; i < n; i++) {
        threads.emplace_back([&io] { io.run();});
    }
    
    for (auto& t : threads) {
        t.join();
    }
    dbPool.join();
	return 0;
}