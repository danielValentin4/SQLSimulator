#include "Interogare.h"

int Interogare::nrInterogari = 0;

int Interogare::detectTip() {
    if (nrParametrii < 1) return TIP_INVALID;

    // create table student
    if (strcmp(parametrii[0], "create") == 0 &&
        strcmp(parametrii[1], "table") == 0)
        return TIP_CREATE;

    // drop table student
    if (strcmp(parametrii[0], "drop") == 0 &&
        strcmp(parametrii[1], "table") == 0)
        return TIP_DROP;

    // select student / select student 2
    if (strcmp(parametrii[0], "select") == 0)
        return TIP_SELECT;

    // -student add column nume
    if (parametrii[0][0] == '-' && nrParametrii >= 3 &&
        strcmp(parametrii[1], "add") == 0 &&
        strcmp(parametrii[2], "column") == 0)
        return TIP_ADD;

    // -student remove column nume
    if (parametrii[0][0] == '-' && nrParametrii >= 3 &&
        strcmp(parametrii[1], "remove") == 0 &&
        strcmp(parametrii[2], "column") == 0)
        return TIP_REMOVE;

    // -student insert ...
    if (parametrii[0][0] == '-' && nrParametrii >= 3 &&
        strcmp(parametrii[1], "insert") == 0)
        return TIP_INSERT;

    // --help
    if (strcmp(parametrii[0], "--help") == 0)
        return TIP_HELP;

    return TIP_INVALID;
}

Interogare::Interogare() {
    parametrii = nullptr;
    nrParametrii = 0;
    tipInterogare = TIP_INVALID;
    nrInterogari++;
}

// 
Interogare::Interogare(char** argv, int argc) {
    nrParametrii = argc - 1;
    if (nrParametrii > 0) {
        parametrii = new char* [nrParametrii];
        for (int i = 0; i < nrParametrii; i++) {
            parametrii[i] = new char[strlen(argv[i + 1]) + 1];
            strcpy_s(parametrii[i], strlen(argv[i + 1]) + 1, argv[i + 1]);
        }
    }
    else parametrii = nullptr;
    tipInterogare = detectTip();
    nrInterogari++;
}

Interogare::Interogare(const Interogare& i) {
    this->nrParametrii = i.nrParametrii;
    this->tipInterogare = i.tipInterogare;
    if (i.nrParametrii > 0 && i.parametrii != nullptr) {
        this->parametrii = new char* [i.nrParametrii];
        for (int j = 0; j < i.nrParametrii; j++) {
            this->parametrii[j] = new char[strlen(i.parametrii[j]) + 1];
            strcpy_s(this->parametrii[j], strlen(i.parametrii[j]) + 1, i.parametrii[j]);
        }
    }
    else this->parametrii = nullptr;
    nrInterogari++;
}

Interogare::~Interogare() {
    if (parametrii != nullptr) {
        for (int i = 0; i < nrParametrii; i++)
            delete[] parametrii[i];
        delete[] parametrii;
    }
}

Interogare& Interogare::operator=(const Interogare& i) {
    if (this != &i) {
        if (parametrii != nullptr) {
            for (int j = 0; j < nrParametrii; j++)
                delete[] parametrii[j];
            delete[] parametrii;
        }
        this->nrParametrii = i.nrParametrii;
        this->tipInterogare = i.tipInterogare;
        if (i.nrParametrii > 0 && i.parametrii != nullptr) {
            this->parametrii = new char* [i.nrParametrii];
            for (int j = 0; j < i.nrParametrii; j++) {
                this->parametrii[j] = new char[strlen(i.parametrii[j]) + 1];
                strcpy_s(this->parametrii[j], strlen(i.parametrii[j]) + 1, i.parametrii[j]);
            }
        }
        else this->parametrii = nullptr;
    }
    return *this;
}

char* Interogare::operator[](int index) {
    if (index >= 0 && index < nrParametrii)
        return parametrii[index];
    throw runtime_error("Index invalid");
}

bool Interogare::operator!() {
    return tipInterogare == TIP_INVALID;
}

bool Interogare::operator==(const Interogare& i) {
    if (tipInterogare != i.tipInterogare) return false;
    if (nrParametrii != i.nrParametrii) return false;
    for (int j = 0; j < nrParametrii; j++)
        if (strcmp(parametrii[j], i.parametrii[j]) != 0) return false;
    return true;
}


// debugging cu tip , parametrii
void Interogare::executa(BazaDeDate& baza) {
    cout << "TIP: " << tipInterogare << endl;
    cout << "Parametrii: ";
    for (int i = 0; i < nrParametrii; i++)
        cout << parametrii[i] << " ";
    cout << endl;

    switch (tipInterogare) {

    case TIP_CREATE: {
        
        if (nrParametrii < 3) { cout << "Comanda invalida!" << endl; return; }
        baza.createTable(parametrii[2]);
        break;
    }

    case TIP_DROP: {
        
        if (nrParametrii < 3) { cout << "Comanda invalida!" << endl; return; }
        bool ok = baza.dropTable(parametrii[2]);
        if (ok) cout << "Tabela stearsa!" << endl;
        else cout << "Tabela nu exista!" << endl;
        break;
    }

    case TIP_SELECT: {
        
        if (nrParametrii < 2) { cout << "Comanda invalida!" << endl; return; }
        Tabela* t = baza.getTabela(parametrii[1]);
        if (t == nullptr) { cout << "Tabela nu exista!" << endl; return; }
        if (nrParametrii == 2) {
            // afiseaza toata tabela
            cout << *t;
        }
        else if (nrParametrii == 3) {
            // afiseaza randul cu indexul dat
            int index = atoi(parametrii[2]);
            try {
                Rand r = t->getRand(index);
                cout << r;
            }
            catch (...) {
                cout << "Index invalid!" << endl;
            }
        }
        break;
    }

    case TIP_ADD: {
        
        if (nrParametrii < 4) { cout << "Comanda invalida!" << endl; return; }
        const char* numeTabela = parametrii[0] + 2; 
        Tabela* t = baza.getTabela(numeTabela);
        if (t == nullptr) { cout << "Tabela nu exista!" << endl; return; }
        t->addColumn(parametrii[3]);
        cout << "Coloana adaugata!" << endl;
        break;
    }

    case TIP_REMOVE: {
        
        if (nrParametrii < 4) { cout << "Comanda invalida!" << endl; return; }
        const char* numeTabela = parametrii[0] + 2;
        Tabela* t = baza.getTabela(numeTabela);
        if (t == nullptr) { cout << "Tabela nu exista!" << endl; return; }
        t->removeColumn(parametrii[3]);
        cout << "Coloana stearsa!" << endl;
        break;
    }

    case TIP_INSERT: {
        
        if (nrParametrii < 3) { cout << "Comanda invalida!" << endl; return; }
        const char* numeTabela = parametrii[0] + 2;
        Tabela* t = baza.getTabela(numeTabela);
        if (t == nullptr) { cout << "Tabela nu exista!" << endl; return; }

        int nrValori = nrParametrii - 2; 
        int idRand = atoi(parametrii[2]);

        
        char** valori = new char* [nrValori];
        char** numeCol = new char* [nrValori];
        for (int i = 0; i < nrValori; i++) {
            valori[i] = parametrii[i + 2];
            Coloana& c = (*t)[i];
            numeCol[i] = c.getNume();
        }
        Rand r(idRand, valori, nrValori, numeCol);
        t->insertRand(r);
        for (int i = 0; i < nrValori; i++) delete[] numeCol[i];
        delete[] valori;
        delete[] numeCol;
        cout << "Rand inserat!" << endl;
        break;
    }


    case TIP_HELP: {
        cout << "===============================================" << endl;
        cout << "         SQLSimulator - Comenzi disponibile   " << endl;
        cout << "===============================================" << endl;
        cout << endl;
        cout << "Creare tabela:" << endl;
        cout << "  aplicatie.exe create table <numeTabel>" << endl;
        cout << endl;
        cout << "Adaugare coloana:" << endl;
        cout << "  aplicatie.exe --<numeTabel> add column <numeColoana>" << endl;
        cout << endl;
        cout << "Stergere coloana:" << endl;
        cout << "  aplicatie.exe --<numeTabel> remove column <numeColoana>" << endl;
        cout << endl;
        cout << "Inserare rand:" << endl;
        cout << "  aplicatie.exe --<numeTabel> -<numeColoana> insert <id> <val1> <val2> ..." << endl;
        cout << endl;
        cout << "Afisare tabela completa:" << endl;
        cout << "  aplicatie.exe select <numeTabel>" << endl;
        cout << endl;
        cout << "Afisare rand specific:" << endl;
        cout << "  aplicatie.exe select <numeTabel> <index>" << endl;
        cout << endl;
        cout << "Stergere tabela:" << endl;
        cout << "  aplicatie.exe drop table <numeTabel>" << endl;
        cout << endl;
        cout << "Ajutor:" << endl;
        cout << "  aplicatie.exe --help" << endl;
        cout << "===============================================" << endl;
        break;
    }

    default:
        cout << "Comanda necunoscuta!" << endl;
        break;
    }
}

ostream& operator<<(ostream& out, const Interogare& i) {
    out << "Tip interogare: " << i.tipInterogare << endl;
    out << "Parametrii: ";
    for (int j = 0; j < i.nrParametrii; j++)
        out << i.parametrii[j] << " ";
    out << endl;
    return out;
}