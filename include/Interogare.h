#pragma once
#include <iostream>
#include <string>
#include <exception>
#include <cstring>
#include <stdexcept>
#include "BazaDeDate.h"
//using namespace std;
using std::vector, std::string, std::ostream, std::istream, std::runtime_error, std::cout;
constexpr auto TIP_INVALID = 0;
constexpr auto TIP_CREATE = 1;
constexpr auto TIP_ADD = 2;
constexpr auto TIP_REMOVE = 3;
constexpr auto TIP_INSERT = 4;
constexpr auto TIP_SELECT = 5;
constexpr auto TIP_DROP = 6;
constexpr auto TIP_HELP = 7;
constexpr auto TIP_WHERE = 8;
constexpr auto TIP_SHOW_TABLES = 9;
constexpr auto TIP_PURGE_TABLE = 10;
constexpr auto TIP_DELETE = 11;
constexpr auto TIP_UPDATE = 12;
constexpr auto TIP_DESCRIBE = 13;
constexpr auto TIP_JOIN = 14;

class Interogare {
private:
    char** parametrii;
    int nrParametrii;
    int tipInterogare;
    static int nrInterogari;

    int detectTip();
public:
    Interogare();
    Interogare(char**, int);
    Interogare(const Interogare&);
    ~Interogare();
    Interogare& operator=(const Interogare&);
    char* operator[](int);
    bool operator!();
    bool operator==(const Interogare&);
    void executa(BazaDeDate&);
    friend ostream& operator<<(ostream&, const Interogare&);
};