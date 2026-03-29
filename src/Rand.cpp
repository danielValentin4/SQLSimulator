#include "Rand.h"

int Rand::nrRanduri = 0;

Rand::Rand() : ID(0) {
    date = nullptr;
    numeColoane = nullptr;
    nrValori = 0;
    Rand::nrRanduri++;
}

Rand::Rand(int ID, char** date, int nrValori, char** numeColoane) : ID(ID) {
    this->nrValori = nrValori;
    if (nrValori > 0 && date != nullptr && numeColoane != nullptr) {
        this->date = new char* [nrValori];
        this->numeColoane = new char* [nrValori];
        for (int i = 0; i < nrValori; i++) {
            this->date[i] = new char[strlen(date[i]) + 1];
            strcpy_s(this->date[i], strlen(date[i]) + 1, date[i]);
            this->numeColoane[i] = new char[strlen(numeColoane[i]) + 1];
            strcpy_s(this->numeColoane[i], strlen(numeColoane[i]) + 1, numeColoane[i]);
        }
    }
    else {
        this->date = nullptr;
        this->numeColoane = nullptr;
        this->nrValori = 0;
    }
    Rand::nrRanduri++;
}

Rand::Rand(const Rand& r) : ID(r.ID) {
    this->nrValori = r.nrValori;
    if (r.nrValori > 0 && r.date != nullptr) {
        this->date = new char* [r.nrValori];
        this->numeColoane = new char* [r.nrValori];
        for (int i = 0; i < r.nrValori; i++) {
            this->date[i] = new char[strlen(r.date[i]) + 1];
            strcpy_s(this->date[i], strlen(r.date[i]) + 1, r.date[i]);
            this->numeColoane[i] = new char[strlen(r.numeColoane[i]) + 1];
            strcpy_s(this->numeColoane[i], strlen(r.numeColoane[i]) + 1, r.numeColoane[i]);
        }
    }
    else {
        this->date = nullptr;
        this->numeColoane = nullptr;
    }
    Rand::nrRanduri++;
}

Rand::~Rand() {
    if (date != nullptr) {
        for (int i = 0; i < nrValori; i++) delete[] date[i];
        delete[] date;
    }
    if (numeColoane != nullptr) {
        for (int i = 0; i < nrValori; i++) delete[] numeColoane[i];
        delete[] numeColoane;
    }
}

Rand& Rand::operator=(const Rand& r) {
    if (this != &r) {
        if (date != nullptr) {
            for (int i = 0; i < nrValori; i++) delete[] date[i];
            delete[] date;
        }
        if (numeColoane != nullptr) {
            for (int i = 0; i < nrValori; i++) delete[] numeColoane[i];
            delete[] numeColoane;
        }
        this->nrValori = r.nrValori;
        if (r.nrValori > 0 && r.date != nullptr) {
            this->date = new char* [r.nrValori];
            this->numeColoane = new char* [r.nrValori];
            for (int i = 0; i < r.nrValori; i++) {
                this->date[i] = new char[strlen(r.date[i]) + 1];
                strcpy_s(this->date[i], strlen(r.date[i]) + 1, r.date[i]);
                this->numeColoane[i] = new char[strlen(r.numeColoane[i]) + 1];
                strcpy_s(this->numeColoane[i], strlen(r.numeColoane[i]) + 1, r.numeColoane[i]);
            }
        }
        else {
            this->date = nullptr;
            this->numeColoane = nullptr;
        }
    }
    return *this;
}

char*& Rand::operator[](int index) {
    if (index >= 0 && index < nrValori) return date[index];
    throw runtime_error("Index invalid");
}

char*& Rand::operator[](const char* numeColoana) const {
    for (int i = 0; i < nrValori; i++)
        if (strcmp(numeColoane[i], numeColoana) == 0) return date[i];
    throw runtime_error("Coloana inexistenta");
}

Rand::operator int() {
    return nrValori;
}

bool Rand::operator>(const Rand& r) {
    return nrValori > r.nrValori;
}

bool Rand::operator==(const Rand& r) {
    if (nrValori != r.nrValori) return false;
    for (int i = 0; i < nrValori; i++) {
        if (strcmp(numeColoane[i], r.numeColoane[i]) != 0) return false;
        if (strcmp(date[i], r.date[i]) != 0) return false;
    }
    return true;
}

bool Rand::operator!() {
    return nrValori == 0;
}

Rand Rand::operator+(const Rand& r) {
    if (*this == r) return *this;
    Rand copie;
    copie.nrValori = nrValori + r.nrValori;
    copie.date = new char* [copie.nrValori];
    copie.numeColoane = new char* [copie.nrValori];
    int i = 0;
    for (i = 0; i < nrValori; i++) {
        copie.date[i] = new char[strlen(date[i]) + 1];
        strcpy_s(copie.date[i], strlen(date[i]) + 1, date[i]);
        copie.numeColoane[i] = new char[strlen(numeColoane[i]) + 1];
        strcpy_s(copie.numeColoane[i], strlen(numeColoane[i]) + 1, numeColoane[i]);
    }
    for (int j = 0; j < r.nrValori; j++) {
        copie.date[i] = new char[strlen(r.date[j]) + 1];
        strcpy_s(copie.date[i], strlen(r.date[j]) + 1, r.date[j]);
        copie.numeColoane[i] = new char[strlen(r.numeColoane[j]) + 1];
        strcpy_s(copie.numeColoane[i], strlen(r.numeColoane[j]) + 1, r.numeColoane[j]);
        i++;
    }
    return copie;
}

ostream& operator<<(ostream& out, const Rand& r) {
    

    for (int i = 0;i < r.nrValori;i++) {
        out << r.numeColoane[i] << "\t";
    }
    out << endl;
    for (int i = 0;i < r.nrValori;i++) {
        out << r.date[i] << "\t";
    }
    out << endl;
    return out;
}

istream& operator>>(istream& in, Rand& r) {
    if (r.date != nullptr) {
        for (int i = 0; i < r.nrValori; i++) delete[] r.date[i];
        delete[] r.date;
    }
    if (r.numeColoane != nullptr) {
        for (int i = 0; i < r.nrValori; i++) delete[] r.numeColoane[i];
        delete[] r.numeColoane;
    }
    cout << "Nr valori: ";
    in >> r.nrValori;
    if (r.nrValori > 0) {
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
    }
    return in;
}