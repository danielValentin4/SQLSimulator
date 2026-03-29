#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <cstring>
#include <stdexcept>
#include "Tabela.h"
using namespace std;

class BazaDeDate {
private:
    Tabela* tabele;
    int nrTabele;
    char* denumireBaza;
    const int idBaza;
    static int bazeDeDateCreate;
public:
    BazaDeDate();
    BazaDeDate(const char*, Tabela*, int, int);
    BazaDeDate(const BazaDeDate&);
    ~BazaDeDate();
    BazaDeDate& operator=(const BazaDeDate&);
    Tabela& operator[](int);
    BazaDeDate operator+(const Tabela&);
    bool operator==(const BazaDeDate&);
    bool operator!();
    bool operator<(const BazaDeDate&);
    Tabela* getTabela(const char*);
    void createTable(const char*);
    bool dropTable(const char*);
    void salveaza();
    void incarca();
    friend ostream& operator<<(ostream&, const BazaDeDate&);
    friend istream& operator>>(istream&, BazaDeDate&);
};