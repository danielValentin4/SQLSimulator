#include "BazaDeDate.h"

int BazaDeDate::bazeDeDateCreate = 0;

BazaDeDate::BazaDeDate() : idBaza(0) {
    denumireBaza = new char[strlen("N/A") + 1];
    strcpy_s(denumireBaza, strlen("N/A") + 1, "N/A");
    tabele = nullptr;
    nrTabele = 0;
    bazeDeDateCreate++;
}

BazaDeDate::BazaDeDate(const char* denumireBaza, Tabela* tabele, int nrTabele, int idBaza) : idBaza(idBaza) {
    if (denumireBaza != nullptr) {
        this->denumireBaza = new char[strlen(denumireBaza) + 1];
        strcpy_s(this->denumireBaza, strlen(denumireBaza) + 1, denumireBaza);
    }
    else {
        this->denumireBaza = new char[strlen("N/A") + 1];
        strcpy_s(this->denumireBaza, strlen("N/A") + 1, "N/A");
    }
    this->nrTabele = nrTabele;
    if (nrTabele > 0 && tabele != nullptr) {
        this->tabele = new Tabela[nrTabele];
        for (int i = 0; i < nrTabele; i++)
            this->tabele[i] = tabele[i];
    }
    else this->tabele = nullptr;
    bazeDeDateCreate++;
}

BazaDeDate::BazaDeDate(const BazaDeDate& b) : idBaza(b.idBaza) {
    if (b.denumireBaza != nullptr) {
        this->denumireBaza = new char[strlen(b.denumireBaza) + 1];
        strcpy_s(this->denumireBaza, strlen(b.denumireBaza) + 1, b.denumireBaza);
    }
    else {
        this->denumireBaza = new char[strlen("N/A") + 1];
        strcpy_s(this->denumireBaza, strlen("N/A") + 1, "N/A");
    }
    this->nrTabele = b.nrTabele;
    if (b.nrTabele > 0 && b.tabele != nullptr) {
        this->tabele = new Tabela[b.nrTabele];
        for (int i = 0; i < b.nrTabele; i++)
            this->tabele[i] = b.tabele[i];
    }
    else this->tabele = nullptr;
    bazeDeDateCreate++;
}

BazaDeDate::~BazaDeDate() {
    if (denumireBaza != nullptr) delete[] denumireBaza;
    if (tabele != nullptr) delete[] tabele;
}

BazaDeDate& BazaDeDate::operator=(const BazaDeDate& b) {
    if (this != &b) {
        if (denumireBaza != nullptr) delete[] denumireBaza;
        if (tabele != nullptr) delete[] tabele;
        if (b.denumireBaza != nullptr) {
            this->denumireBaza = new char[strlen(b.denumireBaza) + 1];
            strcpy_s(this->denumireBaza, strlen(b.denumireBaza) + 1, b.denumireBaza);
        }
        else {
            this->denumireBaza = new char[strlen("N/A") + 1];
            strcpy_s(this->denumireBaza, strlen("N/A") + 1, "N/A");
        }
        this->nrTabele = b.nrTabele;
        if (b.nrTabele > 0 && b.tabele != nullptr) {
            this->tabele = new Tabela[b.nrTabele];
            for (int i = 0; i < b.nrTabele; i++)
                this->tabele[i] = b.tabele[i];
        }
        else this->tabele = nullptr;
    }
    return *this;
}

Tabela& BazaDeDate::operator[](int index) {
    if (index >= 0 && index < nrTabele) return tabele[index];
    throw runtime_error("Index invalid");
}

BazaDeDate BazaDeDate::operator+(const Tabela& t) {
    BazaDeDate copie = *this;
    Tabela* temp = new Tabela[copie.nrTabele + 1];
    for (int i = 0; i < copie.nrTabele; i++)
        temp[i] = copie.tabele[i];
    temp[copie.nrTabele] = t;
    delete[] copie.tabele;
    copie.tabele = temp;
    copie.nrTabele++;
    return copie;
}

bool BazaDeDate::operator==(const BazaDeDate& b) {
    if (strcmp(denumireBaza, b.denumireBaza) != 0) return false;
    if (nrTabele != b.nrTabele) return false;
    for (int i = 0; i < nrTabele; i++)
        if (!(tabele[i] == b.tabele[i])) return false;
    return true;
}

bool BazaDeDate::operator!() {
    return nrTabele == 0;
}

bool BazaDeDate::operator<(const BazaDeDate& b) {
    return nrTabele < b.nrTabele;
}

Tabela* BazaDeDate::getTabela(const char* nume) {
    for (int i = 0; i < nrTabele; i++) {
        char* n = tabele[i].getNumeTabela();
        if (strcmp(n, nume) == 0) {
            delete[] n;
            return &tabele[i];
        }
        delete[] n;
    }
    return nullptr;
}

void BazaDeDate::createTable(const char* nume) {
    if (getTabela(nume) != nullptr) {
        cout << "Tabela exista deja!" << endl;
        return;
    }
    Tabela noua;
    noua.setNumeTabela(nume);
    
    *this = *this + noua;
   
    cout << "Tabela " << nume << " creata!" << endl;
}

bool BazaDeDate::dropTable(const char* nume) {
    int index = -1;
    for (int i = 0; i < nrTabele; i++) {
        char* n = tabele[i].getNumeTabela();
        if (strcmp(n, nume) == 0) {
            index = i;
            delete[] n;
            break;
        }
        delete[] n;
    }
    if (index == -1) return false;
    Tabela* temp = new Tabela[nrTabele - 1];
    for (int i = 0, j = 0; i < nrTabele; i++)
        if (i != index) temp[j++] = tabele[i];
    delete[] tabele;
    tabele = temp;
    nrTabele--;
    return true;
}


//folosim fisiere binare pt retinerea informatiilor
void scrieString(ofstream& f, const char* str) {
    int len = str ? strlen(str) : 0;
    f.write((char*)&len, sizeof(int));
    if (len > 0) f.write(str, len);
}

char* citesteString(ifstream& f) {
    int len = 0;
    f.read((char*)&len, sizeof(int));
    char* str = new char[len + 1];
    if (len > 0) f.read(str, len);
    str[len] = '\0';
    return str;
}

void BazaDeDate::salveaza() {
    
    char numeFisier[200];
    strcpy_s(numeFisier, denumireBaza);
    strcat_s(numeFisier, ".bin");

    ofstream f(numeFisier, ios::binary);
    if (!f.is_open()) {
        cout << "Eroare la deschiderea fisierului!" << endl;
        return;
    }

    scrieString(f, denumireBaza);
    f.write((char*)&nrTabele, sizeof(int));

    for (int i = 0; i < nrTabele; i++) {
        char* numeT = tabele[i].getNumeTabela();
        scrieString(f, numeT);
        delete[] numeT;

        int nrCol = tabele[i].getNrColoane();
        f.write((char*)&nrCol, sizeof(int));

        for (int j = 0; j < nrCol; j++) {
            Coloana& c = tabele[i][j];
            char* numeC = c.getNume();
            scrieString(f, numeC);
            delete[] numeC;

            int sz = (int)c;
            f.write((char*)&sz, sizeof(int));
            for (int k = 0; k < sz; k++)
                scrieString(f, c[k]);
        }
    }
    f.close();
    cout << "Salvat in " << numeFisier << endl;
}

void BazaDeDate::incarca() {
    char numeFisier[200];
    strcpy_s(numeFisier, denumireBaza);
    strcat_s(numeFisier, ".bin");

    ifstream f(numeFisier, ios::binary);
    if (!f.is_open()) {
        cout << "Fisierul nu exista, se porneste cu baza goala." << endl;
        return;
    }

    if (tabele != nullptr) delete[] tabele;
    if (denumireBaza != nullptr) delete[] denumireBaza;

    denumireBaza = citesteString(f);
    f.read((char*)&nrTabele, sizeof(int));
    tabele = new Tabela[nrTabele];

    for (int i = 0; i < nrTabele; i++) {
        char* numeT = citesteString(f);
        int nrCol = 0;
        f.read((char*)&nrCol, sizeof(int));

        Coloana* cols = new Coloana[nrCol];
        for (int j = 0; j < nrCol; j++) {
            char* numeC = citesteString(f);
            int sz = 0;
            f.read((char*)&sz, sizeof(int));
            char** valori = new char* [sz];
            for (int k = 0; k < sz; k++)
                valori[k] = citesteString(f);
            cols[j] = Coloana(numeC, valori, sz);
            for (int k = 0; k < sz; k++) delete[] valori[k];
            delete[] valori;
            delete[] numeC;
        }
        tabele[i] = Tabela(numeT, cols, nrCol, i);
        delete[] cols;
        delete[] numeT;
    }
    f.close();
}

ostream& operator<<(ostream& out, const BazaDeDate& b) {
    out << "Baza de date: " << b.denumireBaza << endl;
    out << "Nr tabele: " << b.nrTabele << endl;
    for (int i = 0; i < b.nrTabele; i++)
        out << b.tabele[i];
    return out;
}

istream& operator>>(istream& in, BazaDeDate& b) {
    cout << "Denumire baza: ";
    string buffer;
    in >> buffer;
    if (b.denumireBaza != nullptr) delete[] b.denumireBaza;
    b.denumireBaza = new char[buffer.length() + 1];
    strcpy_s(b.denumireBaza, buffer.length() + 1, buffer.c_str());
    cout << "Nr tabele: ";
    in >> b.nrTabele;
    if (b.tabele != nullptr) delete[] b.tabele;
    if (b.nrTabele > 0) {
        b.tabele = new Tabela[b.nrTabele];
        for (int i = 0; i < b.nrTabele; i++) {
            cout << "Tabela " << i << ": ";
            in >> b.tabele[i];
        }
    }
    else {
        b.nrTabele = 0;
        b.tabele = nullptr;
    }
    return in;
}