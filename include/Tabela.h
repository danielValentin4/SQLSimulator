#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include <exception>
#include <cstring>
#include <vector>
#include "Coloana.h"
#include "Rand.h"
//using namespace std;

class Tabela {
private:
    char* numeTabela;
    vector<Coloana> coloane;
    const int idTabela;
    static int nrTabele;
public:
    Tabela();
    Tabela(const char*, vector<Coloana>, int);
    Tabela(const Tabela&);
    ~Tabela();
    Tabela& operator=(const Tabela&);
    Coloana& operator[](int);
    Tabela operator+(const Coloana&);
    bool operator==(const Tabela&) const;
    bool operator!();
    bool operator>(const Tabela&);
    char* getNumeTabela() const;
    Coloana* getColoana(const char*);
    Rand getRand(int) const;
    int getNrColoane() const;
    void setNumeTabela(const char*);
    void addColumn(const char*);
    void removeColumn(const char*);
    void insertRand(const Rand&);
    friend ostream& operator<<(ostream&,const Tabela&);
    friend istream& operator>>(istream&, Tabela&);
};