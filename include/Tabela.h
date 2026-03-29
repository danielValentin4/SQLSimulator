#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include <exception>
#include <cstring>
#include "Coloana.h"
#include "Rand.h"
using namespace std;

class Tabela {
private:
    char* numeTabela;
    Coloana* coloane;
    int nrColoane;
    const int idTabela;
    static int nrTabele;
public:
    Tabela();
    Tabela(const char*, Coloana*, int, int);
    Tabela(const Tabela&);
    ~Tabela();
    Tabela& operator=(const Tabela&);
    Coloana& operator[](int);
    Tabela operator+(const Coloana&);
    bool operator==(const Tabela&);
    bool operator!();
    bool operator>(const Tabela&);
    char* getNumeTabela();
    Coloana* getColoana(const char*);
    Rand getRand(int);
    int getNrColoane();
    void setNumeTabela(const char*);
    void addColumn(const char*);
    void removeColumn(const char*);
    void insertRand(const Rand&);
    friend ostream& operator<<(ostream&, Tabela&);
    friend istream& operator>>(istream&, Tabela&);
};