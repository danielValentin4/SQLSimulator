#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
//using namespace std;
using std::vector, std::string, std::ostream, std::istream, std::cout, std::runtime_error;
class Rand {
private:
    vector<string> date;
    const int ID;
    static int nrRanduri;
public:
    Rand();
    Rand(int, vector<string>);
    Rand(const Rand&);
    ~Rand();
    Rand& operator=(const Rand&);
    string& operator[](int);
    const string& operator[](int) const;
    /*char*& operator[](const char*) const;*/
    explicit operator int() const;
    bool operator>(const Rand&);
    bool operator==(const Rand&);
    bool operator!();
    Rand operator+(const Rand&);
    friend ostream& operator<<(ostream&, const Rand&);
    friend istream& operator>>(istream&, Rand&);
};