#include "Rand.h"

int Rand::nrRanduri = 0;

Rand::Rand() : ID(0) {
    date.reserve(5);
    Rand::nrRanduri++;
}

Rand::Rand(int ID, vector<string> date) : ID(ID), date(date) {
    Rand::nrRanduri++;
}

Rand::Rand(const Rand& r) : ID(r.ID), date(r.date) {
    Rand::nrRanduri++;
}

Rand::~Rand() {
    Rand::nrRanduri--;
}

Rand& Rand::operator=(const Rand& r) {
    if (this != &r) {
        date = r.date;
    }
    return *this;
}

string& Rand::operator[](int index) {
    if (index >= 0 && index < date.size()) return date.at(index);
    throw runtime_error("Index invalid");
}

const string& Rand::operator[](int index) const {
    if (index >= 0 && index < date.size()) return date.at(index);
    throw runtime_error("Index invalid");
}

//char*& Rand::operator[](const char* numeColoana) const {
//    for (int i = 0; i < nrValori; i++)
//        if (strcmp(numeColoane[i], numeColoana) == 0) return date[i];
//    throw runtime_error("Coloana inexistenta");
//}

Rand::operator int() const {
    return date.size();
}

bool Rand::operator>(const Rand& r) {
    return date.size() > r.date.size();
}

bool Rand::operator==(const Rand& r) {
    return date == r.date;
}

bool Rand::operator!() {
    return date.empty();
}

Rand Rand::operator+(const Rand& r) {
    if (*this == r) return *this;
    Rand copie = *this;
    copie.date.insert(copie.date.end(), r.date.begin(), r.date.end());
    return copie;
}

ostream& operator<<(ostream& out, const Rand& r) {
    
    for (size_t i = 0; i < r.date.size();i++) {
        out << r.date[i] << "\t";
    }
    out << '\n';
        
    
    return out;
}

istream& operator>>(istream& in, Rand& r) {
    /*if (r.date != nullptr) {
        for (int i = 0; i < r.nrValori; i++) delete[] r.date[i];
        delete[] r.date;
    }
    if (r.numeColoane != nullptr) {
        for (int i = 0; i < r.nrValori; i++) delete[] r.numeColoane[i];
        delete[] r.numeColoane;
    }*/
    cout << "Nr valori: ";
    int sz;
    in >> sz;
    r.date.clear();
    if (sz > 0) {
        string buffer;
        for (int i = 0; i < sz; i++) {
            in >> buffer;
            r.date.emplace_back(buffer);
        }
    }
    /*if (r.nrValori > 0) {
        r.date = new char* [r.nrValori];
        r.numeColoane = new char* [r.nrValori];
        char buffer[200];
        for (int i = 0; i < r.nrValori; i++) {
            cout << "Nume coloana " << i << ": ";
            in >> buffer;
            r.numeColoane[i] = new char[strlen(buffer) + 1];
            strcpy_s(r.numeColoane[i], strlen(buffer) + 1, buffer);
            cout << "Valoare: ";
            in >> buffer;
            r.date[i] = new char[strlen(buffer) + 1];
            strcpy_s(r.date[i], strlen(buffer) + 1, buffer);
        }
    }
    else {
        r.date = nullptr;
        r.numeColoane = nullptr;
    }*/
    return in;
}