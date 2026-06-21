#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include <exception>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <memory>
#include <format>
#include "Coloana.h"
#include "Conditie.h"
#include "Rand.h"
//using namespace std;
using std::stof, std::unordered_map, std::unique_ptr;
class Tabela {
private:
    char* numeTabela;
    vector<Coloana> coloane;
    const int idTabela;
    static int nrTabele;
    unordered_map<string, int> mapID;
    vector<bool> deleted;
    
public:
    Tabela();
    Tabela(const char*, vector<Coloana>, int, vector<bool>);
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
    void addColumn(const char*, const char*);
    void removeColumn(const char*);
    void insertRand(const Rand&);
    //void selectRand(const char*, const char*, const char*);
    void selectRand(const NodConditie*);
    //int updateRand(const char*, const char*, const char*, const char*, const char*);
    int updateRand(const vector<std::pair<string, unique_ptr<ExpresieUpdate>>>&, const NodConditie*);
    void purgeTable(int);
    void addMap(string, int);
    int getIndex(string);
    int findSlot();
    int getNrRanduri();
    bool isDeleted(int);
    bool isDeleted(int) const;
    void setDeleted(int index);
    void setMap(unordered_map<string, int>);
    void afisareMap();
    void describeTable();
    static Tabela joinTables(Tabela*, Tabela*, char* , char*);
    friend ostream& operator<<(ostream&,const Tabela&);
    friend istream& operator>>(istream&, Tabela&);
    friend std::vector<size_t> widthColoane(const Tabela&);
};