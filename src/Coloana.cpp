#include "Coloana.h"

int Coloana::marimeDefault = 1;

Coloana::Coloana(){
    nume = new char[strlen("N/A") + 1];
    strcpy(nume, "N/A");
    date.reserve(5);
    this->tipData = String;
}

Coloana::Coloana(const char* nume, vector<string> date, TipData tipData) : date(date)  {
    if (nume != nullptr) {
        this->nume = new char[strlen(nume) + 1];
        strcpy(this->nume, nume);
    }
    else {
        this->nume = new char[strlen("N/A") + 1];
        strcpy(this->nume, "N/A");
    }
    this->tipData = tipData;
}

Coloana::Coloana(const Coloana& c) : date(c.date)  {
    if (c.nume != nullptr) {
        this->nume = new char[strlen(c.nume) + 1];
        strcpy(this->nume, c.nume);
    }
    else {
        nume = new char[strlen("N/A") + 1];
        strcpy(nume, "N/A");
    }
    this->tipData = c.tipData;
}

Coloana::~Coloana() {
    if (nume != nullptr) delete[] nume;
}

char* Coloana::getNume() const {
    char* copie = new char[strlen(nume) + 1];
    strcpy(copie, nume);
    return copie;
}

void Coloana::setNume(const char* nume) {
    if (this->nume != nullptr) delete[] this->nume;
    if (nume != nullptr) {
        this->nume = new char[strlen(nume) + 1];
        strcpy(this->nume, nume);
    }
    else {
        this->nume = new char[strlen("N/A") + 1];
        strcpy(this->nume, "N/A");
    }
}

int Coloana::getLastID() {
    //if (date.empty()) return -1;
    int max = -1;
    //return stoi(date.back());
    for (auto a : date) {
        if (stoi(a) > max) {
            max = stoi(a);
        }
    }
    return max;
}

void Coloana::clearData() {
    date.clear();
}

TipData* Coloana::getTip() {
    return &this->tipData;
}

TipData Coloana::getTipData(const char* tipData) {
    if (strcmp(tipData, "date") == 0) {
        return TipData::Data;
    }
    
    else if (strcmp(tipData, "number") == 0) {
        return TipData::Numar;
    }
    else {
        return TipData::String;
    }
}

char* Coloana::getTipData()
{
    char* copie = nullptr;
    if (tipData == TipData::Data) {
        copie = new char[strlen("date") + 1];
        strcpy(copie, "data");
    }
    if (tipData == TipData::Numar) {
        copie = new char[strlen("number") + 1];
        strcpy(copie, "number");
    }
    if (tipData == TipData::String) {
        copie = new char[strlen("string") + 1];
        strcpy(copie, "string");
    }
    return copie;
}

Coloana& Coloana::operator=(const Coloana& c){
    if (this != &c) {
        if (nume != nullptr) delete[] nume;
        if (c.nume != nullptr) {
            this->nume = new char[strlen(c.nume) + 1];
            strcpy(this->nume, c.nume);
        }
        else {
            this->nume = new char[strlen("N/A") + 1];
            strcpy(this->nume, "N/A");
        }
        this->date = c.date;
        this->tipData = c.tipData;
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
    strcpy(c.nume, buffer.c_str());

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