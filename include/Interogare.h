#pragma once
#include <iostream>
#include <string>
#include <exception>
#include <cstring>
#include <stdexcept>
#include "BazaDeDate.h"
using namespace std;

#define TIP_INVALID  0
#define TIP_CREATE   1
#define TIP_ADD      2
#define TIP_REMOVE   3
#define TIP_INSERT   4
#define TIP_SELECT   5
#define TIP_DROP     6
#define TIP_HELP     7


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