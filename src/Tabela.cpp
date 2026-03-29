#include "Tabela.h"

int Tabela::nrTabele = 0;

Tabela::Tabela() : idTabela(0) {
    numeTabela = new char[strlen("N/A") + 1];
    strcpy_s(numeTabela, strlen("N/A") + 1, "N/A");
    nrColoane = 0;
    coloane = nullptr;
    nrTabele++;
}

Tabela::Tabela(const char* numeTabela, Coloana* coloane, int nrColoane, int idTabela) : idTabela(idTabela) {
    if (numeTabela != nullptr) {
        this->numeTabela = new char[strlen(numeTabela) + 1];
        strcpy_s(this->numeTabela, strlen(numeTabela) + 1, numeTabela);
    }
    else {
        this->numeTabela = new char[strlen("N/A") + 1];
        strcpy_s(this->numeTabela, strlen("N/A") + 1, "N/A");
    }
    this->nrColoane = nrColoane;
    if (nrColoane > 0 && coloane != nullptr) {
        this->coloane = new Coloana[nrColoane];
        for (int i = 0; i < nrColoane; i++)
            this->coloane[i] = coloane[i];
    }
    else this->coloane = nullptr;
    nrTabele++;
}

Tabela::Tabela(const Tabela& t) : idTabela(t.idTabela) {
    if (t.numeTabela != nullptr) {
        this->numeTabela = new char[strlen(t.numeTabela) + 1];
        strcpy_s(this->numeTabela, strlen(t.numeTabela) + 1, t.numeTabela);
    }
    else {
        this->numeTabela = new char[strlen("N/A") + 1];
        strcpy_s(this->numeTabela, strlen("N/A") + 1, "N/A");
    }
    this->nrColoane = t.nrColoane;
    if (t.nrColoane > 0 && t.coloane != nullptr) {
        this->coloane = new Coloana[t.nrColoane];
        for (int i = 0; i < t.nrColoane; i++)
            this->coloane[i] = t.coloane[i];
    }
    else this->coloane = nullptr;
    nrTabele++;
}

Tabela::~Tabela() {
    if (numeTabela != nullptr) delete[] numeTabela;
    if (coloane != nullptr) delete[] coloane;
}

Tabela& Tabela::operator=(const Tabela& t) {
    if (this != &t) {
        if (numeTabela != nullptr) delete[] numeTabela;
        if (coloane != nullptr) delete[] coloane;
        if (t.numeTabela != nullptr) {
            this->numeTabela = new char[strlen(t.numeTabela) + 1];
            strcpy_s(this->numeTabela, strlen(t.numeTabela) + 1, t.numeTabela);
        }
        else {
            this->numeTabela = new char[strlen("N/A") + 1];
            strcpy_s(this->numeTabela, strlen("N/A") + 1, "N/A");
        }
        this->nrColoane = t.nrColoane;
        if (t.nrColoane > 0 && t.coloane != nullptr) {
            this->coloane = new Coloana[t.nrColoane];
            for (int i = 0; i < t.nrColoane; i++)
                this->coloane[i] = t.coloane[i];
        }
        else this->coloane = nullptr;
    }
    return *this;
}

Coloana& Tabela::operator[](int index) {
    if (index >= 0 && index < nrColoane) return coloane[index];
    throw runtime_error("Index invalid");
}

Tabela Tabela::operator+(const Coloana& c) {
    Tabela copie = *this;
    Coloana* temp = new Coloana[copie.nrColoane + 1];
    for (int i = 0; i < copie.nrColoane; i++)
        temp[i] = copie.coloane[i];
    temp[copie.nrColoane] = c;
    delete[] copie.coloane;
    copie.coloane = temp;
    copie.nrColoane++;
    return copie;
}

bool Tabela::operator==(const Tabela& t) {
    if (strcmp(numeTabela, t.numeTabela) != 0) return false;
    if (nrColoane != t.nrColoane) return false;
    for (int i = 0; i < nrColoane; i++)
        if (!(coloane[i] == t.coloane[i])) return false;
    return true;
}

bool Tabela::operator!() {
    return nrColoane == 0;
}

int Tabela::getNrColoane() {
    return nrColoane;
}

void Tabela::setNumeTabela(const char* nume) {
    if (numeTabela != nullptr) {
        delete[] numeTabela;
    }
    if (nume != nullptr) {
        numeTabela = new char[strlen(nume) + 1];
        strcpy_s(numeTabela, strlen(nume) + 1, nume);
    }
    else {
        numeTabela = new char[strlen("N/A") + 1];
        strcpy_s(numeTabela, strlen("N/A") + 1, "N/A");
    }
}

bool Tabela::operator>(const Tabela& t) {
    return nrColoane > t.nrColoane;
}

char* Tabela::getNumeTabela() {
    char* copie = new char[strlen(numeTabela) + 1];
    strcpy_s(copie, strlen(numeTabela) + 1, numeTabela);
    return copie;
}

Coloana* Tabela::getColoana(const char* nume) {
    for (int i = 0; i < nrColoane; i++)
        if (strcmp(coloane[i].getNume(), nume) == 0)
            return &coloane[i];
    return nullptr;
}

Rand Tabela::getRand(int index) {
    if (index < 0) throw runtime_error("Index invalid");
   
    for (int i = 0; i < nrColoane; i++)
        if (index >= (int)coloane[i]) throw runtime_error("Index invalid");

    char** valori = new char* [nrColoane];
    char** nume = new char* [nrColoane];
    for (int i = 0; i < nrColoane; i++) {
        valori[i] = coloane[i][index];
        char* n = coloane[i].getNume();
        nume[i] = n;
    }
    Rand r(index, valori, nrColoane, nume);
    for (int i = 0; i < nrColoane; i++) delete[] nume[i];
    delete[] valori;
    delete[] nume;
    return r;
}

void Tabela::addColumn(const char* nume) {
    Coloana noua(nume, nullptr, 0);
    Coloana* temp = new Coloana[nrColoane + 1];
    for (int i = 0; i < nrColoane; i++)
        temp[i] = coloane[i];
    temp[nrColoane] = noua;
    delete[] coloane;
    coloane = temp;
    nrColoane++;
}

void Tabela::removeColumn(const char* nume) {
    int index = -1;
    for (int i = 0; i < nrColoane; i++)
        if (strcmp(coloane[i].getNume(), nume) == 0) {
            index = i;
            break;
        }
    if (index == -1) return;
    Coloana* temp = new Coloana[nrColoane - 1];
    for (int i = 0, j = 0; i < nrColoane; i++)
        if (i != index) temp[j++] = coloane[i];
    delete[] coloane;
    coloane = temp;
    nrColoane--;
}

void Tabela::insertRand(const Rand& r) {

    int nrRanduriCurent = 0;
    if (nrColoane > 0)
        nrRanduriCurent = (int)coloane[0]; 

    for (int i = 0; i < nrColoane; i++) {
        char* numeC = coloane[i].getNume();
        try {
            char* val = r[numeC];
            coloane[i] = coloane[i] + val;
        }
        catch (...) {
            coloane[i] = coloane[i] + "0";
        }
        delete[] numeC;
    }
}

ostream& operator<<(ostream& out, Tabela& t) {
    if (t.nrColoane == 0) { out << "Tabela goala !" << endl; return out; }
    out << "ID" << "\t";
    for (int i = 0;i < t.nrColoane;i++) {
        char* nume = t.coloane[i].getNume();
        out << nume << "\t";
        delete[] nume;
    }
    out << endl;

    int nrRanduri = (int)t.coloane[0];
    for (int i = 0;i < nrRanduri;i++) {
        out << i << "\t";
        Rand r = t.getRand(i);
        for (int j = 0;j < t.nrColoane;j++) {
            char* numeC = t.coloane[j].getNume();
            out << r[numeC] << "\t";
            delete[] numeC;
        }
        out << endl;
    }
    return out;
}

istream& operator>>(istream& in, Tabela& t) {
    cout << "Nume tabela: ";
    string buffer;
    in >> buffer;
    if (t.numeTabela != nullptr) delete[] t.numeTabela;
    t.numeTabela = new char[buffer.length() + 1];
    strcpy_s(t.numeTabela, buffer.length() + 1, buffer.c_str());
    cout << "Nr coloane: ";
    int oldNr = t.nrColoane;
    in >> t.nrColoane;
    if (t.coloane != nullptr) delete[] t.coloane;
    if (t.nrColoane > 0) {
        t.coloane = new Coloana[t.nrColoane];
        for (int i = 0; i < t.nrColoane; i++) {
            cout << "Coloana " << i << ": ";
            in >> t.coloane[i];
        }
    }
    else {
        t.nrColoane = 0;
        t.coloane = nullptr;
    }
    return in;
}