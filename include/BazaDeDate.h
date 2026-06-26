#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <cstring>
#include <stdexcept>
#include "Tabela.h"
//using namespace std;
using std::ofstream, std::ifstream, std::ios;
class BazaDeDate {
private:
    //Tabela* tabele;
    //int nrTabele;
    vector<Tabela> tabele;
    char* denumireBaza;
    const int idBaza;
    static int bazeDeDateCreate;
public:
    BazaDeDate();
    BazaDeDate(const char*, vector<Tabela> , int);
    BazaDeDate(const BazaDeDate&);
    ~BazaDeDate();
    BazaDeDate& operator=(const BazaDeDate&);
    Tabela& operator[](int);
    BazaDeDate operator+(const Tabela&);
    bool operator==(const BazaDeDate&) const;
    bool operator!();
    bool operator<(const BazaDeDate&);
    Tabela* getTabela(const char*);
    void showLoadedTables();
    void createTable(const char*);
    bool dropTable(const char*);
    void salveaza(Tabela*);
    void incarca(char*);
    friend ostream& operator<<(ostream&, const BazaDeDate&);
    friend istream& operator>>(istream&, BazaDeDate&);
};