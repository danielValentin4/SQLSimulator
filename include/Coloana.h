#pragma once
#include <iostream>
#include <string>
#include <exception>
#include <cstring>
#include <vector>
#include <stdexcept>
//using namespace std;
using std::vector, std::string, std::ostream, std::istream, std::runtime_error, std::cout;
class Coloana {
private:
    char* nume;
    //char** date;
    vector<string> date;
    //int size;
    static int marimeDefault;
public:
    Coloana();
    Coloana(const char*, vector<string>);
    Coloana(const Coloana&);
    ~Coloana();
    char* getNume() const;
    void setNume(const char*);
    Coloana& operator=(const Coloana&);
    Coloana operator+(const char*);
    Coloana& operator+=(string);
    explicit operator int() const;
    bool operator!() const;
    bool operator==(const Coloana&) const;
    bool operator>(const Coloana&);
    string& operator[](int);
    const string& operator[](int) const;
    friend ostream& operator<<(ostream&, const Coloana&);
    friend istream& operator>>(istream&, Coloana&);
};