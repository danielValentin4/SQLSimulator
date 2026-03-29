#include "Coloana.h"

int Coloana::marimeDefault = 1;

Coloana::Coloana(){
    nume = new char[strlen("N/A") + 1];
    strcpy_s(nume, strlen("N/A") + 1, "N/A");
    date = nullptr;
    size = 0;
}

Coloana::Coloana(const char* nume, char** date, int size)  {
    if (nume != nullptr) {
        this->nume = new char[strlen(nume) + 1];
        strcpy_s(this->nume, strlen(nume) + 1, nume);
    }
    else {
        this->nume = new char[strlen("N/A") + 1];
        strcpy_s(this->nume, strlen("N/A") + 1, "N/A");
    }
    if (size > 0 && date != nullptr) {
        this->size = size;
        this->date = new char* [size];
        for (int i = 0; i < size; i++) {
            this->date[i] = new char[strlen(date[i]) + 1];
            strcpy_s(this->date[i], strlen(date[i]) + 1, date[i]);
        }
    }
    else {
        this->size = 0;
        this->date = nullptr;
    }
}

Coloana::Coloana(const Coloana& c){
    if (c.nume != nullptr) {
        this->nume = new char[strlen(c.nume) + 1];
        strcpy_s(this->nume, strlen(c.nume) + 1, c.nume);
    }
    else {
        nume = new char[strlen("N/A") + 1];
        strcpy_s(nume, strlen("N/A") + 1, "N/A");
    }
    if (c.size > 0 && c.date != nullptr) {
        this->size = c.size;
        this->date = new char* [c.size];
        for (int i = 0; i < c.size; i++) {
            this->date[i] = new char[strlen(c.date[i]) + 1];
            strcpy_s(this->date[i], strlen(c.date[i]) + 1, c.date[i]);
        }
    }
    else {
        this->date = nullptr;
        this->size = 0;
    }
}

Coloana::~Coloana() {
    if (nume != nullptr) delete[] nume;
    if (date != nullptr) {
        for (int i = 0; i < size; i++) delete[] date[i];
        delete[] date;
    }
}

char* Coloana::getNume() {
    char* copie = new char[strlen(nume) + 1];
    strcpy_s(copie, strlen(nume) + 1, nume);
    return copie;
}

void Coloana::setNume(const char* nume) {
    if (this->nume != nullptr) delete[] this->nume;
    if (nume != nullptr) {
        this->nume = new char[strlen(nume) + 1];
        strcpy_s(this->nume, strlen(nume) + 1, nume);
    }
    else {
        this->nume = new char[strlen("N/A") + 1];
        strcpy_s(this->nume, strlen("N/A") + 1, "N/A");
    }
}

Coloana& Coloana::operator=(const Coloana& c) {
    if (this != &c) {
        if (nume != nullptr) delete[] nume;
        if (date != nullptr) {
            for (int i = 0; i < size; i++) delete[] date[i];
            delete[] date;
        }
        if (c.nume != nullptr) {
            this->nume = new char[strlen(c.nume) + 1];
            strcpy_s(this->nume, strlen(c.nume) + 1, c.nume);
        }
        else this->nume = nullptr;
        if (c.size > 0 && c.date != nullptr) {
            this->size = c.size;
            this->date = new char* [c.size];
            for (int i = 0; i < c.size; i++) {
                this->date[i] = new char[strlen(c.date[i]) + 1];
                strcpy_s(this->date[i], strlen(c.date[i]) + 1, c.date[i]);
            }
        }
        else {
            this->size = 0;
            this->date = nullptr;
        }
    }
    return *this;
}

Coloana Coloana::operator+(const char* valoare) {
    Coloana copie = *this;
    char** temp = new char* [copie.size + 1];
    for (int i = 0; i < copie.size; i++) {
        temp[i] = new char[strlen(copie.date[i]) + 1];
        strcpy_s(temp[i], strlen(copie.date[i]) + 1, copie.date[i]);
    }
    temp[copie.size] = new char[strlen(valoare) + 1];
    strcpy_s(temp[copie.size], strlen(valoare) + 1, valoare);
    for (int i = 0; i < copie.size; i++) delete[] copie.date[i];
    delete[] copie.date;
    copie.date = temp;
    copie.size++;
    return copie;
}

Coloana::operator int() {
    return size;
}

bool Coloana::operator!() {
    return size <= 0;
}

bool Coloana::operator==(const Coloana& c) {
    if (strcmp(nume, c.nume) != 0) return false;
    if (size != c.size) return false;
    for (int i = 0; i < size; i++)
        if (strcmp(date[i], c.date[i]) != 0) return false;
    return true;
}

bool Coloana::operator>(const Coloana& c) {
    return this->size > c.size;
}

char*& Coloana::operator[](int index) {
    if (index < 0 || index >= size)
        throw runtime_error("Index invalid");
    return date[index];
}

ostream& operator<<(ostream& out, const Coloana& c) {
    out << "Nume coloana: " << c.nume << endl;
    out << "Valori: ";
    if (c.date != nullptr && c.size > 0)
        for (int i = 0; i < c.size; i++)
            out << c.date[i] << " ";
    else
        out << "Nu exista valori";
    out << endl;
    return out;
}

istream& operator>>(istream& in, Coloana& c) {
    cout << "Nume coloana: ";
    string buffer;
    in >> buffer;
    if (c.nume != nullptr) delete[] c.nume;
    c.nume = new char[buffer.length() + 1];
    strcpy_s(c.nume, buffer.length() + 1, buffer.c_str());

    int oldSize = c.size;
    cout << "Size-ul vectorului de date: ";
    in >> c.size;

    if (c.date != nullptr) {
        for (int i = 0; i < oldSize; i++) delete[] c.date[i];
        delete[] c.date;
    }
    if (c.size > 0) {
        c.date = new char* [c.size];
        for (int i = 0; i < c.size; i++) {
            cout << "Valoarea " << i + 1 << ": ";
            in >> buffer;
            c.date[i] = new char[buffer.length() + 1];
            strcpy_s(c.date[i], buffer.length() + 1, buffer.c_str());
        }
    }
    else {
        c.size = 0;
        c.date = nullptr;
    }
    return in;
}