#include "BazaDeDate.h"

int BazaDeDate::bazeDeDateCreate = 0;

BazaDeDate::BazaDeDate() : idBaza(0) {
    denumireBaza = new char[strlen("N/A") + 1];
    strcpy(denumireBaza, "N/A");
    tabele.reserve(5);
    BazaDeDate::bazeDeDateCreate++;
}

BazaDeDate::BazaDeDate(const char* denumireBaza, vector<Tabela> tabele, int idBaza) : idBaza(idBaza), tabele(tabele) {
    if (denumireBaza != nullptr) {
        this->denumireBaza = new char[strlen(denumireBaza) + 1];
        strcpy(this->denumireBaza, denumireBaza);
    }
    else {
        this->denumireBaza = new char[strlen("N/A") + 1];
        strcpy(this->denumireBaza, "N/A");
    }
    BazaDeDate::bazeDeDateCreate++;
}

BazaDeDate::BazaDeDate(const BazaDeDate& b) : idBaza(b.idBaza), tabele(b.tabele) {
    if (b.denumireBaza != nullptr) {
        this->denumireBaza = new char[strlen(b.denumireBaza) + 1];
        strcpy(this->denumireBaza, b.denumireBaza);
    }
    else {
        this->denumireBaza = new char[strlen("N/A") + 1];
        strcpy(this->denumireBaza, "N/A");
    }
    BazaDeDate::bazeDeDateCreate++;
}

BazaDeDate::~BazaDeDate() {
    if (denumireBaza != nullptr) delete[] denumireBaza;
    BazaDeDate::bazeDeDateCreate--;
}

BazaDeDate& BazaDeDate::operator=(const BazaDeDate& b) {
    if (this != &b) {
        if (denumireBaza != nullptr) delete[] denumireBaza;
        if (b.denumireBaza != nullptr) {
            this->denumireBaza = new char[strlen(b.denumireBaza) + 1];
            strcpy(this->denumireBaza, b.denumireBaza);
        }
        else {
            this->denumireBaza = new char[strlen("N/A") + 1];
            strcpy(this->denumireBaza, "N/A");
        }
        tabele = b.tabele;
    }
    return *this;
}

Tabela& BazaDeDate::operator[](int index) {
    if (index >= 0 && index < tabele.size()) return tabele.at(index);
    throw runtime_error("Index invalid");
}

BazaDeDate BazaDeDate::operator+(const Tabela& t) {
    BazaDeDate copie = *this;
    /*Tabela* temp = new Tabela[copie.nrTabele + 1];
    for (int i = 0; i < copie.nrTabele; i++)
        temp[i] = copie.tabele[i];
    temp[copie.nrTabele] = t;
    delete[] copie.tabele;
    copie.tabele = temp;
    copie.nrTabele++;*/
    
    copie.tabele.emplace_back(t);
    
    return copie;
}

bool BazaDeDate::operator==(const BazaDeDate& b) const {
    if (strcmp(denumireBaza, b.denumireBaza) != 0) return false;
    return(tabele == b.tabele);
}

bool BazaDeDate::operator!() {
    return tabele.size();
}

bool BazaDeDate::operator<(const BazaDeDate& b) {
    return tabele.size() < b.tabele.size();
}

Tabela* BazaDeDate::getTabela(const char* nume) {
    for (size_t i = 0; i < tabele.size(); i++) {
        char* n = tabele[i].getNumeTabela();
        if (strcmp(n, nume) == 0) {
            delete[] n;
            return &tabele[i];
        }
        delete[] n;
    }
    return nullptr;
}


void BazaDeDate::showTables() {
    cout << "Tabele: \n";
    for (size_t i = 0; i < tabele.size(); i++) {
        char* nume = tabele[i].getNumeTabela();
        cout << nume << "\n";
        delete[] nume;
    }
}

void BazaDeDate::createTable(const char* nume) {
    if (getTabela(nume) != nullptr) {
        cout << "Tabela exista deja!" << '\n';
        return;
    }
    Tabela noua;
    noua.setNumeTabela(nume);
    noua.addColumn("ID", "number");
    *this = *this + noua;
   
    cout << "Tabela " << nume << " creata!" << '\n';
}

bool BazaDeDate::dropTable(const char* nume) {
    /*int index = -1;*/
    for (size_t i = 0; i < tabele.size(); i++) {
        char* n = tabele[i].getNumeTabela();
        if (strcmp(n, nume) == 0) {
            tabele.erase(tabele.begin() + i);
            delete[] n;
            
            return true;
        }
        delete[] n;
    }
    /*if (index == -1) return false;
    Tabela* temp = new Tabela[nrTabele - 1];
    for (int i = 0, j = 0; i < nrTabele; i++)
        if (i != index) temp[j++] = tabele[i];
    delete[] tabele;
    tabele = temp;
    nrTabele--;
    return true;*/
    return false;
}


//folosim fisiere binare pt retinerea informatiilor
void scrieString(ofstream& f, string str) {
    int len;
    if (str.length() > 0) {
        len = str.length();
    }
    else {
        len = 0;
    }
    char* strC = new char[str.length() + 1];
    strcpy(strC, str.c_str());
    //int len = str ? strlen(str) : 0;
    f.write((char*)&len, sizeof(int));
    if (len > 0) f.write(strC, len);
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
    strcpy(numeFisier, denumireBaza);
    strcat(numeFisier, ".bin");

    ofstream f(numeFisier, ios::binary);
    if (!f.is_open()) {
        cout << "Eroare la deschiderea fisierului!" << '\n';
        return;
    }

    size_t nrTabele = tabele.size();
    scrieString(f, denumireBaza);
    f.write((char*)&nrTabele, sizeof(size_t));

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
            f.write((char*)c.getTip(), sizeof(int));
            int sz = (int)c;
            f.write((char*)&sz, sizeof(int));
            for (int k = 0; k < sz; k++)
                scrieString(f, c[k]);
        }
    }
    f.close();
    cout << "Salvat in " << numeFisier << '\n';
}

void BazaDeDate::incarca() {
    char numeFisier[200];
    strcpy(numeFisier, denumireBaza);
    strcat(numeFisier, ".bin");

    ifstream f(numeFisier, ios::binary);
    if (!f.is_open()) {
        cout << "Fisierul nu exista, se porneste cu baza goala." << '\n';
        return;
    }

    /*if (tabele != nullptr) delete[] tabele;*/
    if (denumireBaza != nullptr) delete[] denumireBaza;
    size_t nrTabele = tabele.size();
    denumireBaza = citesteString(f);
    f.read((char*)&nrTabele, sizeof(size_t));
    tabele.reserve(nrTabele);

    for (int i = 0; i < nrTabele; i++) {
        char* numeT = citesteString(f);
        int nrCol = 0;
        f.read((char*)&nrCol, sizeof(int));

        //Coloana* cols = new Coloana[nrCol];
        vector<Coloana> cols(nrCol);
        for (int j = 0; j < nrCol; j++) {
            char* numeC = citesteString(f);
            int nrTipData;
            f.read((char*)&nrTipData, sizeof(int));
            TipData tip = (TipData)nrTipData;
            int sz = 0;
            f.read((char*)&sz, sizeof(int));
            //char** valori = new char* [sz];
            vector<string> valori;
            valori.reserve(sz);
            for (int k = 0; k < sz; k++) {
                char* buffer = citesteString(f);
                valori.emplace_back(buffer);
                delete[] buffer;
            }
            cols[j] = Coloana(numeC, valori, tip);
            delete[] numeC;
        }
        tabele.emplace_back(numeT, cols, i);
        
        delete[] numeT;
    }
    f.close();
}

ostream& operator<<(ostream& out, const BazaDeDate& b) {
    out << "Baza de date: " << b.denumireBaza << '\n';
    out << "Nr tabele: " << b.tabele.size() << '\n';
    for (size_t i = 0; i < b.tabele.size(); i++)
        out << b.tabele[i];
    return out;
}

istream& operator>>(istream& in, BazaDeDate& b) {
    cout << "Denumire baza: ";
    string buffer;
    in >> buffer;
    if (b.denumireBaza != nullptr) delete[] b.denumireBaza;
    b.denumireBaza = new char[buffer.length() + 1];
    strcpy(b.denumireBaza, buffer.c_str());
    size_t nrTabele;
    cout << "Nr tabele: ";
    in >> nrTabele;
    
    if (nrTabele > 0) {
        b.tabele.resize(nrTabele);
        for (int i = 0; i < nrTabele; i++) {
            cout << "Tabela " << i << ": ";
            in >> b.tabele[i];
        }
    }
    
    return in;
}