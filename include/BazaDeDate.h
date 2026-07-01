#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <cstring>
#include <stdexcept>
#include <list>
#include "Tabela.h"

constexpr auto MAX_CAPACITY = 20;
//using namespace std;
using std::ofstream, std::ifstream, std::ios;
class BazaDeDate {
private:
    //Tabela* tabele;
    //int nrTabele;
    std::list<Tabela> tabele;
    unordered_map<string, std::list<Tabela>::iterator> cacheTables;
    char* denumireBaza;
    const int idBaza;
    static int bazeDeDateCreate;
public:
    BazaDeDate();
    BazaDeDate(const char*, std::list<Tabela>, int);
    BazaDeDate(const BazaDeDate&);
    ~BazaDeDate();
    BazaDeDate& operator=(const BazaDeDate&);
    bool operator==(const BazaDeDate&) const;
    bool operator!();
    bool operator<(const BazaDeDate&);
    Tabela* getTabela(const char*);
    string showLoadedTables();
    void createTable(const char*);
    bool dropTable(const char*);
    void salveaza(Tabela*);
    void incarca(char*);
    bool doesTableExist(const char*);
};