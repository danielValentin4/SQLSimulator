#include "Tabela.h"

int Tabela::nrTabele = 0;

Tabela::Tabela() : idTabela(0) {
    numeTabela = new char[strlen("N/A") + 1];
    strcpy(numeTabela, "N/A");
    coloane.reserve(5);
    Tabela::nrTabele++;
}

Tabela::Tabela(const char* numeTabela, vector<Coloana> coloane, int idTabela) : idTabela(idTabela), coloane(coloane) {
    if (numeTabela != nullptr) {
        this->numeTabela = new char[strlen(numeTabela) + 1];
        strcpy(this->numeTabela, numeTabela);
    }
    else {
        this->numeTabela = new char[strlen("N/A") + 1];
        strcpy(this->numeTabela, "N/A");
    }
    Tabela::nrTabele++;
}

Tabela::Tabela(const Tabela& t) : idTabela(t.idTabela), coloane(t.coloane) {
    if (t.numeTabela != nullptr) {
        this->numeTabela = new char[strlen(t.numeTabela) + 1];
        strcpy(this->numeTabela, t.numeTabela);
    }
    else {
        this->numeTabela = new char[strlen("N/A") + 1];
        strcpy(this->numeTabela, "N/A");
    }
    Tabela::nrTabele++;
}

Tabela::~Tabela() {
    if (numeTabela != nullptr) delete[] numeTabela;
    Tabela::nrTabele--;
}

Tabela& Tabela::operator=(const Tabela& t) {
    if (this != &t) {
        if (numeTabela != nullptr) delete[] numeTabela;
        if (t.numeTabela != nullptr) {
            this->numeTabela = new char[strlen(t.numeTabela) + 1];
            strcpy(this->numeTabela, t.numeTabela);
        }
        else {
            this->numeTabela = new char[strlen("N/A") + 1];
            strcpy(this->numeTabela, "N/A");
        }
        this->coloane = t.coloane;
    }
    return *this;
}

Coloana& Tabela::operator[](int index) {
    if (index >= 0 && index < coloane.size()) return coloane.at(index);
    throw runtime_error("Index invalid");
}

Tabela Tabela::operator+(const Coloana& c) {
    Tabela copie = *this;
    int nrRanduriTabel = copie.coloane.empty() ? 0 : (int)copie.coloane[0];
    int nrRanduriColoana = (int)c;
    if (nrRanduriColoana > 0 &&
        nrRanduriTabel > 0 &&
        nrRanduriColoana != nrRanduriTabel) {
        throw runtime_error("Coloana si tabelul au numar diferit de randuri.");
    }
    copie.coloane.emplace_back(c);
    /*Coloana* temp = new Coloana[copie.nrColoane + 1];
    for (int i = 0; i < copie.nrColoane; i++)
        temp[i] = copie.coloane[i];
    temp[copie.nrColoane] = c;
    delete[] copie.coloane;
    copie.coloane = temp;
    copie.nrColoane++;*/
    return copie;
}

bool Tabela::operator==(const Tabela& t) const {
    if (strcmp(numeTabela, t.numeTabela) != 0) return false;
    return coloane == t.coloane;
}

bool Tabela::operator!() {
    return coloane.empty();
}

int Tabela::getNrColoane() const {
    return coloane.size();
}

void Tabela::setNumeTabela(const char* nume) {
    if (numeTabela != nullptr) {
        delete[] numeTabela;
    }
    if (nume != nullptr) {
        numeTabela = new char[strlen(nume) + 1];
        strcpy(numeTabela,  nume);
    }
    else {
        numeTabela = new char[strlen("N/A") + 1];
        strcpy(numeTabela, "N/A");
    }
}

bool Tabela::operator>(const Tabela& t) {
    return coloane.size() > t.coloane.size();
}

char* Tabela::getNumeTabela() const {
    char* copie = new char[strlen(numeTabela) + 1];
    strcpy(copie, numeTabela);
    return copie;
}

Coloana* Tabela::getColoana(const char* nume) {
    for (size_t i = 0; i < coloane.size(); i++) {
        char* numeCol = coloane[i].getNume();
        if (strcmp(numeCol, nume) == 0)
            return &coloane[i];
        delete[] numeCol;
    }
    return nullptr;
}

Rand Tabela::getRand(int index) const {
    if (index < 0) throw runtime_error("Index invalid");
   
    for (size_t i = 0; i < coloane.size(); i++)
        if (index >= (int)coloane[i]) throw runtime_error("Index invalid");
    vector<string> valori(coloane.size());
    /*char** valori = new char* [nrColoane];
    char** nume = new char* [nrColoane];*/
    for (size_t i = 0; i < coloane.size(); i++) {
        valori[i] = coloane[i][index];
    }
    Rand r(index, move(valori));
    return r;
}

bool verificaConditie(const string& valoareColoana, const string& op, const string& valoareData, TipData tip) {
    if (op == "=") return valoareColoana == valoareData;
    if (op == "like") return valoareColoana.find(valoareData) != string::npos;

    try {
        float valColoanaFloat = std::stof(valoareColoana);
        float valDataFloat = std::stof(valoareData);
        if (tip == TipData::Numar) {
            if (op == ">") return valColoanaFloat > valDataFloat;
            if (op == "<") return valColoanaFloat < valDataFloat;
            if (op == ">=") return valColoanaFloat >= valDataFloat;
            if (op == "<=") return valColoanaFloat <= valDataFloat;
        }
        else {
            // de facut pt tip data .
        }
    }
    catch (...) {
        throw runtime_error("Clauza where folosita gresit. Pentru comparare string folositi like. Pt valori folositi operatori aritmetici.");
    }
    return false;
}


void Tabela::selectRand(const char* coloana, const char* op, const char* valoare) {
    Coloana* c = getColoana(coloana);
    if (c == nullptr) {
        cout << "Coloana " << coloana << " nu exista" << "\n";
    }
    
    for (size_t i = 0; i < coloane.size(); i++) {
        char* nume = coloane[i].getNume();
        cout << nume << "\t";
        delete[] nume;
    }
    cout << "\n";

    int nrRanduri = coloane.empty() ? 0 : int(coloane[0]);
    for (int i = 0; i < nrRanduri; i++) {
        string valoareColoana = (*c)[i];
        if (verificaConditie(valoareColoana, op, valoare, *c->getTip())) {
            cout << getRand(i);
        }
    }

}


void Tabela::addColumn(const char* nume, const char* tipData) {
    /*Coloana noua(nume, );
    Coloana* temp = new Coloana[nrColoane + 1];
    for (int i = 0; i < nrColoane; i++)
        temp[i] = coloane[i];
    temp[nrColoane] = noua;
    delete[] coloane;
    coloane = temp;*/
    int nrRanduri = 0;
    if (!coloane.empty()) {
        nrRanduri = (int)coloane[0];
    }
    vector<string> valori(nrRanduri, " ");
    TipData tip = Coloana::getTipData(tipData);

    coloane.emplace_back(nume, valori, tip);
}

void Tabela::removeColumn(const char* nume) {
    /*int index = -1;
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
    coloane = temp;*/
    if (nume == nullptr) return;
    for (size_t i = 0; i < coloane.size(); i++) {
        if (strcmp(coloane[i].getNume(), nume) == 0) {
            coloane.erase(coloane.begin() + i);
            return;
        }
    }
    throw runtime_error("Coloana inexistenta");
    
}

void Tabela::insertRand(const Rand& r) {
    for (int i = 0; i < coloane.size(); i++) {
        int dimensiuneRand = (int)r;

        if (i < dimensiuneRand) {
            coloane[i] +=  r[i];
        }
        else {
            coloane[i] += " ";
        }
    }
    
    
}

ostream& operator<<(ostream& out, const Tabela& t) {
    if (t.coloane.size() == 0) { out << "Tabela goala !" << '\n'; return out; }
    //out << "ID" << "\t";
    for (int i = 0;i < t.coloane.size();i++) {
        char* nume = t.coloane[i].getNume();
        out << nume << "\t";
        delete[] nume;
    }
    out << '\n';

    int nrRanduri = (int)t.coloane[0];
    for (int i = 0;i < nrRanduri;i++) {
        
        Rand r = t.getRand(i);
        for (int j = 0;j < t.coloane.size();j++) {
            
            out << r[j] << "\t";
            
        }
        out << '\n';
    }
    return out;
}

istream& operator>>(istream& in, Tabela& t) {
    cout << "Nume tabela: ";
    string buffer;
    in >> buffer;
    if (t.numeTabela != nullptr) delete[] t.numeTabela;
    t.numeTabela = new char[buffer.length() + 1];
    strcpy(t.numeTabela, buffer.c_str());
    cout << "Nr coloane: ";
    int sz = 0;
    in >> sz;
    t.coloane.clear();
    if (sz > 0) {
        for (int i = 0; i < sz; i++) {
            Coloana coloanaTemp;
            cout << "Coloana " << i << ": ";
            in >> coloanaTemp;
            t.coloane.emplace_back(coloanaTemp);
        }
    }
    
    return in;
}