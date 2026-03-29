#pragma once
#include <iostream>
#include <string>
#include <exception>
#include <cstring>
#include <stdexcept>
using namespace std;

class Coloana {
private:
    char* nume;
    char** date;
    int size;
    static int marimeDefault;
public:
    Coloana();
    Coloana(const char*, char**, int);
    Coloana(const Coloana&);
    ~Coloana();
    char* getNume();
    void setNume(const char*);
    Coloana& operator=(const Coloana&);
    Coloana operator+(const char*);
    explicit operator int();
    bool operator!();
    bool operator==(const Coloana&);
    bool operator>(const Coloana&);
    char*& operator[](int);
    friend ostream& operator<<(ostream&, const Coloana&);
    friend istream& operator>>(istream&, Coloana&);
};