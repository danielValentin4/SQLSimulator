#include "BazaDeDate.h"

int BazaDeDate::bazeDeDateCreate = 0;

BazaDeDate::BazaDeDate() : idBaza(0) {
    denumireBaza = new char[strlen("N/A") + 1];
    strcpy(denumireBaza, "N/A");
    //tabele.reserve(5);
    cacheTables.reserve(5);
    BazaDeDate::bazeDeDateCreate++;
}

BazaDeDate::BazaDeDate(const char* denumireBaza, std::list<Tabela> tabele, int idBaza) : idBaza(idBaza), tabele(tabele) {
    if (denumireBaza != nullptr) {
        this->denumireBaza = new char[strlen(denumireBaza) + 1];
        strcpy(this->denumireBaza, denumireBaza);
    }
    else {
        this->denumireBaza = new char[strlen("N/A") + 1];
        strcpy(this->denumireBaza, "N/A");
    }
    cacheTables.reserve(5);
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
    cacheTables.reserve(5);
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
        cacheTables = b.cacheTables;
    }
    return *this;
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
    if (cacheTables.find(nume) == cacheTables.end()) {
        return nullptr;
    }
    auto it = cacheTables[nume];
    Tabela* t = &(*it);
    tabele.splice(tabele.begin(), tabele, it);
    return t;
}


string BazaDeDate::showLoadedTables() {
    string mesaj = "";
    for (auto i = tabele.cbegin(); i != tabele.cend(); i++) {
        char* nume = (*i).getNumeTabela();
        mesaj = mesaj + nume + "\n";
        delete[] nume;
    }
    return mesaj;
}

void BazaDeDate::createTable(const char* nume) {
    Tabela noua;
    noua.setNumeTabela(nume);
    noua.addColumn("ID", "number");
    tabele.emplace_front(noua);
    cacheTables[nume] = tabele.begin();
    cout << "Tabela " << nume << " creata!" << '\n';
}

bool BazaDeDate::doesTableExist(const char* nume) {
    if (cacheTables.find(nume) != cacheTables.end()) {
        return true;
    }
    else {
        return false;
    }
}

bool BazaDeDate::dropTable(const char* nume) {
    auto mapIt = cacheTables.find(nume);
    if (mapIt != cacheTables.end()) {
        auto listIt = mapIt->second;
        tabele.erase(listIt);
        cacheTables.erase(mapIt);
        return true;
    }
    return false;
}



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

void BazaDeDate::salveaza(Tabela* t) {
    char numeFisier[200];
    char* numeTabela = t->getNumeTabela();
    strcpy(numeFisier, numeTabela);
    strcat(numeFisier, ".bin");


    ofstream f(numeFisier, ios::binary);
    if (!f.is_open()) {
        cout << "Eroare la deschiderea fisierului!" << '\n';
        return;
    }

    
    scrieString(f, numeTabela);
        
    int nrRanduriDel = t->getNrRanduri();
    f.write((char*)&nrRanduriDel, sizeof(int));
    for (int k = 0; k < nrRanduriDel; k++) {
          bool d = t->isDeleted(k);
          f.write((char*)&d, sizeof(bool));
    }


        
    int nrCol = t->getNrColoane();
    f.write((char*)&nrCol, sizeof(int));

    for (int j = 0; j < nrCol; j++) {
        Coloana& c = (*t)[j];
        char* numeC = c.getNume();
        scrieString(f, numeC);
        delete[] numeC;
        f.write((char*)c.getTip(), sizeof(int));
        int sz = (int)c;
        f.write((char*)&sz, sizeof(int));
        for (int k = 0; k < sz; k++)
           scrieString(f, c[k]);
        }   
    f.close();
    cout << "Salvat in " << numeFisier << '\n';
    delete[] numeTabela;
}

void BazaDeDate::incarca(char* numeTabela) {
    if (cacheTables.count(numeTabela) == 1) {
        auto it = cacheTables[numeTabela];
        tabele.splice(tabele.begin(), tabele, it);
        return;
    }
    else {
        if (tabele.size() >= MAX_CAPACITY) {
            char* numeTab = tabele.back().getNumeTabela();
            cacheTables.erase(numeTab);
            tabele.pop_back();
            delete[] numeTab;
        }
    }
    char numeFisier[200];
    strcpy(numeFisier, numeTabela);
    strcat(numeFisier, ".bin");

    ifstream f(numeFisier, ios::binary);
    if (!f.is_open()) {
        cout << "Fisierul nu exista, se porneste cu baza goala." << '\n';
        return;
    }

    char* numeT = citesteString(f);

    int nrRanduriDeleted = 0;
    f.read((char*)&nrRanduriDeleted, sizeof(int));
    vector<bool> deletedVec(nrRanduriDeleted, false);
    for (int j = 0; j < nrRanduriDeleted; j++) {
         bool d{};
         f.read((char*)&d, sizeof(bool));
         deletedVec[j] = d;
    }

    int nrCol = 0;
    f.read((char*)&nrCol, sizeof(int));

        
    vector<Coloana> cols(nrCol);
    for (int j = 0; j < nrCol; j++) {
         char* numeC = citesteString(f);
         int nrTipData;
         f.read((char*)&nrTipData, sizeof(int));
         TipData tip = (TipData)nrTipData;
         int sz = 0;
         f.read((char*)&sz, sizeof(int));
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

    tabele.emplace_front(numeT, cols, tabele.size() - 1, deletedVec);
    for (int k = 0; k < nrRanduriDeleted; k++) {
        tabele.front().addMap(cols[0][k], k);
    }
    cacheTables[numeTabela] = tabele.begin();
    delete[] numeT;
    f.close();
}
