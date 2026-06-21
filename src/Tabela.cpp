#include "Tabela.h"

int Tabela::nrTabele = 0;

Tabela::Tabela() : idTabela(0) {
    numeTabela = new char[strlen("N/A") + 1];
    strcpy(numeTabela, "N/A");
    coloane.reserve(5);
    deleted.reserve(5);
    mapID.reserve(5);
    Tabela::nrTabele++;
}

Tabela::Tabela(const char* numeTabela, vector<Coloana> coloane, int idTabela, vector<bool> deleted) : idTabela(idTabela), coloane(coloane), deleted(deleted) {
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

Tabela::Tabela(const Tabela& t) : idTabela(t.idTabela), coloane(t.coloane), deleted(t.deleted) {
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
        this->deleted = t.deleted;
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
    
    if (deleted[index] == false) {
        
        vector<string> valori(coloane.size());
        /*char** valori = new char* [nrColoane];
        char** nume = new char* [nrColoane];*/
        for (size_t i = 0; i < coloane.size(); i++) {
            valori[i] = coloane[i][index];
        }
        Rand r(index, move(valori));
        return r;
    }
    vector<string> gol;
    return Rand(0, gol);
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
        else if (tip == TipData::Data) {
            // de facut pt tip data .
        }
        else {
            
        }
    }
    catch (...) {
        throw runtime_error("Clauza where folosita gresit. Pentru comparare string folositi like. Pt valori folositi operatori aritmetici.");
    }
    return false;
}

bool evalueazaConditie(const NodConditie* nod, Tabela* t, int indexRand) {
    if (nod->tip == TipNod::CONDITIE) {
        Coloana* c = t->getColoana(nod->coloana.c_str());
        if (c == nullptr) {
            throw runtime_error("Coloana " + nod->coloana + " nu exista");
        }
        return verificaConditie((*c)[indexRand], nod->op, nod->valoare, *c->getTip());
    }
    bool rezStang = evalueazaConditie(nod->stang.get(), t, indexRand);
    if (nod->tip == TipNod::AND) {
        return rezStang && evalueazaConditie(nod->drept.get(), t, indexRand);
    }
    else {
        return rezStang || evalueazaConditie(nod->drept.get(), t, indexRand);
    }
}

string evalueazaExpresieUpdate(const ExpresieUpdate* expresie, Tabela* t, int indexRand) {
    if (expresie == nullptr) {
        return "";
    }
    if (expresie->tipOperatie == TipOperatie::VALOARE) {
        if (expresie->tipValoare == TipValoare::NUMBER) {
            return expresie->valoare;
        }

        Coloana* c = t->getColoana(expresie->valoare.c_str());
        if (c != nullptr) {
            return (*c)[indexRand];
        }
        return expresie->valoare;
    }
    try {
        double stang = std::stod(evalueazaExpresieUpdate(expresie->stang.get(), t, indexRand));
        double drept = std::stod(evalueazaExpresieUpdate(expresie->drept.get(), t, indexRand));
        double rez = 0.0;
        if (expresie->op == "+") rez = stang + drept;
        else if (expresie->op == "-") rez = stang - drept;
        else if (expresie->op == "*") rez = stang * drept;
        else if (expresie->op == "/") {
            if (drept == 0.0) {
                throw runtime_error("Impartire la 0");
            }
            rez = stang / drept;
        }
        return std::format("{:.2f}", rez);
    }
    catch (...) {
        throw runtime_error("Operatie invalida. Ati incercat operatii aritmetice pe string-uri");
    }
}


void Tabela::selectRand(const NodConditie* conditie) {
   
    for (size_t i = 0; i < coloane.size(); i++) {
        char* nume = coloane[i].getNume();
        cout << nume << "\t";
        delete[] nume;
    }
    cout << "\n";

    int nrRanduri = coloane.empty() ? 0 : int(coloane[0]);
    for (int i = 0; i < nrRanduri; i++) {
        if (deleted[i] == true) { continue; }
        if (evalueazaConditie(conditie, this, i)) {
            cout << getRand(i);
        }
    }

}


int Tabela::updateRand(const vector<std::pair<string, unique_ptr<ExpresieUpdate>>>& setari, const NodConditie* clauza)
{
    int nrActualizari = 0;
    for (const auto& setare : setari) {
        if (getColoana(setare.first.c_str()) == nullptr) {
            cout << "Coloana nu exista\n";
            return 0;
        }
    }

    int nrRanduri = coloane.empty() ? 0 : (int)coloane[0];

    for (int i = 0; i < nrRanduri; i++) {
        if (deleted[i] == true) continue;

        bool conditieIndeplinita = true;
        if (clauza != nullptr) {
            conditieIndeplinita = evalueazaConditie(clauza, this, i);
        }

        if (conditieIndeplinita) {
            for (const auto& setare : setari) {
                Coloana* colSet = getColoana(setare.first.c_str());
                try {
                    string valoare = evalueazaExpresieUpdate(setare.second.get(), this, i);
                    (*colSet)[i] = valoare;
                }
                catch (runtime_error e) {
                    cout << e.what() << "\n";
                    nrActualizari--;
                }
            }
            nrActualizari++;
        }
    }
    return nrActualizari;
}

//int Tabela::updateRand(const char* coloana, const char* op, const char* valoareCheck, const char* coloanaSet, const char* valoareSet)
//{
//    int numar = 0;
//    Coloana* coloanaFind = getColoana(coloana);
//    Coloana* coloanaSetare = getColoana(coloanaSet);
//    if (coloanaFind == nullptr) {
//        cout << "Coloana " << coloana << " nu exista" << "\n";
//    }
//    if (coloanaSet == nullptr) {
//        cout << "Coloana " << coloana << " nu exista" << "\n";
//    }
//
//    int nrRanduri = coloane.empty() ? 0 : (int)coloane[0];
//    for (int i = 0; i < nrRanduri; i++) {
//        if (deleted[i] == true) { continue; }
//        string valoareCol = (*coloanaFind)[i];
//        if (verificaConditie(valoareCol, op, valoareCheck, *coloanaFind->getTip())) {
//            (*coloanaSetare)[i] = valoareSet;
//            numar++;
//        }
//    }
//    return numar;
//}


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
    int slot = findSlot();
    int dimensiuneRand = (int)r;
    if (slot == -1) {
        for (int i = 0; i < coloane.size(); i++) {
            

            if (i < dimensiuneRand) {
                coloane[i] += r[i];
            }
            else {
                coloane[i] += " ";
            }
        }
        string ID = coloane[0][(int)coloane[0] - 1];
        int index = (int)coloane[0] - 1;
        mapID.emplace(ID, index);
        deleted.emplace_back(false);
    }
    else {
        for (int i = 0; i < coloane.size(); i++) {
            
            if (i < dimensiuneRand) {
                coloane[i][slot] = r[i];
            }
            else {
                coloane[i][slot] = " ";
            }
        }
        deleted[slot] = false;
        string id = r[0];
        mapID.emplace(id, slot);
    }

}



void Tabela::purgeTable(int conditie) {
    if (conditie == 1) {
        coloane.clear();
        addColumn("ID", "number");
    }
    else {
        for (size_t i = 0; i < coloane.size(); i++) {
            coloane[i].clearData();
        }
    }
}


void Tabela::addMap(string key, int index) {
    mapID.emplace(key, index);
}

int Tabela::getIndex(string key) {
    return mapID[key];
}

int Tabela::findSlot() {
    
    for (int j = 0; j < deleted.size(); j++) {
        if (deleted[j] == true) {
            return j;
        }
    }
    return -1;
}

int Tabela::getNrRanduri() {
    return (int)coloane[0];
}

bool Tabela::isDeleted(int index) {
    return deleted[index];
}

bool Tabela::isDeleted(int index) const
{
    return deleted[index];
}

void Tabela::setDeleted(int index) {
    this->deleted[index] = true;
}

void Tabela::setMap(unordered_map<string, int> map) {
    mapID.clear();
    mapID = map;
}

unordered_map<string, int> Tabela::getMap()
{
    return unordered_map<string, int>(mapID);
}

void Tabela::afisareMap()
{
    for (auto it : mapID) {
        cout << it.first << ":" << it.second << "\n";
    }

}

void Tabela::describeTable()
{
    for (size_t i = 0; i < coloane.size(); i++) {
        char* nume = coloane[i].getNume();
        char* tip = coloane[i].getTipData();
        cout << nume << " - " << tip << "\n";
        delete[] nume;
        delete[] tip;
    }



}


unordered_map<string, vector<int>> makeIndex(Tabela* tabela, char* coloana) {
    unordered_map<string, vector<int>> index;
    Coloana* c = tabela->getColoana(coloana);
    index.reserve((int)(*c));
    for (size_t i = 0; i < (int)(*c); i++) {
        index[(*c)[i]].push_back(i);
    }
    return index;
}



Tabela Tabela::joinTables(Tabela* tabela1, Tabela* tabela2, char* coloana1, char* coloana2)
{
    Tabela t{};
    int nrRanduri1 = (int)(tabela1->coloane[0]);
    int nrRanduri2 = (int)(tabela2->coloane[0]);
    Coloana* c1 = (tabela1)->getColoana(coloana1);
    Coloana* c2 = (tabela2)->getColoana(coloana2);
    
    if (c1 == nullptr || c2 == nullptr) {
        cout << "Coloane gresite\n"; return t;
    }
    auto index = makeIndex(tabela2, coloana2);
    for (int i = 0; i < tabela1->coloane.size(); i++) {
        t.addColumn(tabela1->coloane[i].getNume(), tabela1->coloane[i].getTipData());
    }
    for (int i = 0; i < tabela2->coloane.size(); i++) {
        t.addColumn(tabela2->coloane[i].getNume(), tabela2->coloane[i].getTipData());
    }

    for (int i = 0; i < nrRanduri1; i++) {
        for (auto a : index[(*c1)[i]]) {
            Rand r = (tabela1)->getRand(i) + (tabela2)->getRand(a);
            t.insertRand(r);
        }
    }

    return t;
}


std::vector<size_t> widthColoane(const Tabela& t) {
    std::vector<size_t> widthCol(t.getNrColoane());
    for (size_t i = 0; i < t.getNrColoane(); i++) {
        char* numeCol = t.coloane[i].getNume();
        widthCol[i] = strlen(numeCol);
        for (size_t j = 0; j < (int)t.coloane[0]; j++) {
            if (!t.isDeleted(j)) {
                widthCol[i] = std::max(widthCol[i], t.coloane[i][j].size());
            }
        }
        delete[] numeCol;
    }
    return widthCol;
}




ostream& operator<<(ostream& out, const Tabela& t) {
    if (t.coloane.size() == 0) { out << "Tabela goala !" << '\n'; return out; }
    std::vector<size_t> widthCol = widthColoane(t);

    size_t nrLinii = 1;
    for (auto w : widthCol) {
        nrLinii += w + 3;
    }
    string separator(nrLinii, '-');
    out << separator << "\n";
    //out << "ID" << "\t";
    for (int i = 0;i < t.coloane.size();i++) {
        char* nume = t.coloane[i].getNume();
        out << "| "<< std::left << std::setw(widthCol[i]) << nume << " ";
        delete[] nume;
    }
    out << "| \n";
    out << separator << "\n";
    int nrRanduri = (int)t.coloane[0];
    for (int i = 0;i < nrRanduri;i++) {
        if (t.deleted[i] == true) { continue; }
        Rand r = t.getRand(i);
        for (int j = 0;j < t.coloane.size();j++) {
            
            out << "| " << std::left << std::setw(widthCol[j]) << r[j] << " ";
        }
        out << "|" << "\n" << separator << "\n";
        
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