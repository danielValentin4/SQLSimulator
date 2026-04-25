#include "Coloana.h"

int Coloana::marimeDefault = 1;

Coloana::Coloana(){
    nume = new char[strlen("N/A") + 1];
    strcpy_s(nume, strlen("N/A") + 1, "N/A");
    date.reserve(5);
}

Coloana::Coloana(const char* nume, vector<string> date) : date(date)  {
    if (nume != nullptr) {
        this->nume = new char[strlen(nume) + 1];
        strcpy_s(this->nume, strlen(nume) + 1, nume);
    }
    else {
        this->nume = new char[strlen("N/A") + 1];
        strcpy_s(this->nume, strlen("N/A") + 1, "N/A");
    }
}

Coloana::Coloana(const Coloana& c) : date(c.date)  {
    if (c.nume != nullptr) {
        this->nume = new char[strlen(c.nume) + 1];
        strcpy_s(this->nume, strlen(c.nume) + 1, c.nume);
    }
    else {
        nume = new char[strlen("N/A") + 1];
        strcpy_s(nume, strlen("N/A") + 1, "N/A");
    }
    
}

Coloana::~Coloana() {
    if (nume != nullptr) delete[] nume;
}

char* Coloana::getNume() const {
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

Coloana& Coloana::operator=(const Coloana& c){
    if (this != &c) {
        if (nume != nullptr) delete[] nume;
        if (c.nume != nullptr) {
            this->nume = new char[strlen(c.nume) + 1];
            strcpy_s(this->nume, strlen(c.nume) + 1, c.nume);
        }
        else {
            this->nume = new char[strlen("N/A") + 1];
            strcpy_s(this->nume, strlen("N/A") + 1, "N/A");
        }
        this->date = c.date;
    }
    return *this;
}

Coloana Coloana::operator+(const char* valoare) {
    Coloana copie = *this;
    /*char** temp = new char* [copie.size + 1];
    for (int i = 0; i < copie.size; i++) {
        temp[i] = new char[strlen(copie.date[i]) + 1];
        strcpy_s(temp[i], strlen(copie.date[i]) + 1, copie.date[i]);
    }
    temp[copie.size] = new char[strlen(valoare) + 1];
    strcpy_s(temp[copie.size], strlen(valoare) + 1, valoare);
    for (int i = 0; i < copie.size; i++) delete[] copie.date[i];
    delete[] copie.date;
    copie.date = temp;
    copie.size++;*/
    if (valoare != nullptr) {
        copie.date.emplace_back(valoare);
    }
    else {
        copie.date.emplace_back("");
    }
    return copie;
}

Coloana& Coloana::operator+=(string valoare) {
    this->date.emplace_back(valoare);
    return *this;
}


Coloana::operator int() const {
    return date.size();
}

bool Coloana::operator!() const {
    return date.empty();
}

bool Coloana::operator==(const Coloana& c) const {
    if (strcmp(nume, c.nume) != 0) return false;
    return date == c.date;
}

bool Coloana::operator>(const Coloana& c) {
    return this->date.size() > c.date.size();
}

string& Coloana::operator[](int index) {
    if (index < 0 || index >= date.size())
        throw runtime_error("Index invalid");
    return date.at(index);
}

const string& Coloana::operator[](int index) const {
    if (index < 0 || index >= date.size())
        throw runtime_error("Index invalid");
    return date.at(index);
}

ostream& operator<<(ostream& out, const Coloana& c) {
    out << "Nume coloana: " << c.nume << '\n';
    out << "Valori: ";
    if (c.date.size() > 0)
        for (int i = 0; i < c.date.size(); i++)
            out << c.date[i] << " ";
    else
        out << "Nu exista valori";
    out << '\n';
    return out;
}

istream& operator>>(istream& in, Coloana& c) {
    cout << "Nume coloana: ";
    string buffer;
    in >> buffer;
    if (c.nume != nullptr) delete[] c.nume;
    c.nume = new char[buffer.length() + 1];
    strcpy_s(c.nume, buffer.length() + 1, buffer.c_str());

    int sz = 0;
    cout << "Size-ul vectorului de date: ";
    in >> sz;
    c.date.clear();
    if (sz > 0) {
        for (int i = 0; i < sz; i++) {
            cout << "Valoarea " << i + 1 << ": ";
            in >> buffer;
            c.date.emplace_back(buffer);
        }
    }
    return in;
}