#pragma once
#include <iostream>
#include <exception>
#include <stdexcept>
#include <cstring>
using namespace std;

class Rand {
private:
    char** date;
    int nrValori;
    const int ID;
    char** numeColoane;
    static int nrRanduri;
public:
    Rand();
    Rand(int, char**, int, char**);
    Rand(const Rand&);
    ~Rand();
    Rand& operator=(const Rand&);
    char*& operator[](int);
    char*& operator[](const char*) const;
    explicit operator int();
    bool operator>(const Rand&);
    bool operator==(const Rand&);
    bool operator!();
    Rand operator+(const Rand&);
    friend ostream& operator<<(ostream&, const Rand&);
    friend istream& operator>>(istream&, Rand&);
};