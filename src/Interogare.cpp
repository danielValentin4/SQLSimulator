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

    // alter student add column nume
    if (strcmp(parametrii[0], "alter") == 0 && 
        nrParametrii >= 3 &&
        strcmp(parametrii[2], "add") == 0 &&
        strcmp(parametrii[3], "column") == 0)
        return TIP_ADD;

    // alter student remove column nume
    if (strcmp(parametrii[0], "alter") == 0 && 
        nrParametrii >= 3 &&
        strcmp(parametrii[2], "remove") == 0 &&
        strcmp(parametrii[3], "column") == 0)
        return TIP_REMOVE;

    // insert into student ...
    if (strcmp(parametrii[0], "insert") == 0 && 
        nrParametrii >= 3 &&
        strcmp(parametrii[1], "into") == 0)
        return TIP_INSERT;

    // --help
    if (strcmp(parametrii[0], "--help") == 0)
        return TIP_HELP;

    if (parametrii[0][0] == '-' && nrParametrii >=3 &&
        strcmp(parametrii[1], "where") == 0) {
        return TIP_WHERE;
    }

    if (strcmp(parametrii[0], "show") == 0 &&
        strcmp(parametrii[1], "tables") == 0)
        return TIP_SHOW_TABLES;

    if (strcmp(parametrii[0], "purge") == 0 &&
        strcmp(parametrii[1], "table") == 0 &&
        nrParametrii > 2)
        return TIP_PURGE_TABLE;

    if (strcmp(parametrii[0], "delete") == 0 &&
        strcmp(parametrii[1], "from") == 0 &&
        nrParametrii > 3)
        return TIP_DELETE;

    //update table student where Varsta > 10 to Varsta == 10*Varsta
    if (strcmp(parametrii[0], "update") == 0 &&
        nrParametrii >= 6)
        return TIP_UPDATE;

    if (strcmp(parametrii[0], "describe") == 0 &&
        strcmp(parametrii[1], "table") == 0 &&
        nrParametrii == 3)
        return TIP_DESCRIBE;

    if (strcmp(parametrii[0], "join") == 0 &&
        nrParametrii == 4)
        return TIP_JOIN;
    // join tabel1.id on tabel2.id
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
            strcpy(parametrii[i], argv[i + 1]);
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
            strcpy(this->parametrii[j], i.parametrii[j]);
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
                strcpy(this->parametrii[j], i.parametrii[j]);
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
    cout << "TIP: " << tipInterogare << '\n';
    cout << "Parametrii: ";
    for (int i = 0; i < nrParametrii; i++)
        cout << i  << ":" << parametrii[i] << '\n';
    cout << '\n';

    switch (tipInterogare) {

    case TIP_CREATE: {
        
        if (nrParametrii < 3) { cout << "Comanda invalida!" << '\n'; return; }
        baza.createTable(parametrii[2]);
        break;
    }

    case TIP_DROP: {
        
        if (nrParametrii < 3) { cout << "Comanda invalida!" << '\n'; return; }
        bool ok = baza.dropTable(parametrii[2]);
        if (ok) cout << "Tabela stearsa!" << '\n';
        else cout << "Tabela nu exista!" << '\n';
        break;
    }

    case TIP_SELECT: {
        
        if (nrParametrii < 2) { cout << "Comanda invalida!" << '\n'; return; }
        Tabela* t = baza.getTabela(parametrii[1]);
        if (t == nullptr) { cout << "Tabela nu exista!" << '\n'; return; }
        if (nrParametrii == 2) {
            // afiseaza toata tabela
            cout << *t;
            t->afisareMap();
        }
        else if (nrParametrii == 3) {
            // afiseaza randul cu indexul dat
            int index = atoi(parametrii[2]);
            try {
                
                Rand r = t->getRand(index);
               
                cout << r;
            }
            catch (...) {
                cout << "Index invalid!" << '\n';
            }
        }
        else if (nrParametrii > 3) {
            vector<string> tokens;
            for (int i = 3; i < nrParametrii; i++) {
                tokens.push_back(parametrii[i]);
            }
            try {
                size_t pos = 0;
                auto arbore = parseExpr(tokens, pos);
                t->selectRand(arbore.get());
            }
            catch (runtime_error& e) {
                cout << e.what() << '\n';
            }
            /*const char* numeColoana = parametrii[3];
            const char* op = parametrii[4];
            const char* valoare = parametrii[5];
            if (t == nullptr) {
                cout << "Tabela inexistenta!";
            }
            try {
                t->selectRand(numeColoana, op, valoare);
            }
            catch (runtime_error e) {
                cout << e.what() << '\n';
            }*/

        }
        break;
    }

    case TIP_ADD: {
        
        if (nrParametrii < 4) { cout << "Comanda invalida!" << '\n'; return; }
        const char* numeTabela = parametrii[1];
        Tabela* t = baza.getTabela(numeTabela);
        if (t == nullptr) { cout << "Tabela nu exista!" << '\n'; return; }
        t->addColumn(parametrii[4], parametrii[6]);
        cout << "Coloana adaugata!" << '\n';
        break;
    }

    case TIP_REMOVE: {
        
        if (nrParametrii < 4) { 
            cout << "Comanda invalida!" << '\n'; 
            return; 
        }
        const char* numeTabela = parametrii[1];
        Tabela* t = baza.getTabela(numeTabela);
        if (t == nullptr) { cout << "Tabela nu exista!" << '\n'; return; }
        t->removeColumn(parametrii[4]);
        cout << "Coloana stearsa!" << '\n';
        break;
    }

    case TIP_INSERT: {
        
        if (nrParametrii < 3) { cout << "Comanda invalida!" << '\n'; return; }
        const char* numeTabela = parametrii[2];
        Tabela* t = baza.getTabela(numeTabela);
        if (t == nullptr) { cout << "Tabela nu exista!" << '\n'; return; }

        int nrValori = nrParametrii - 3; 
        

        
        //char** valori = new char* [nrValori];
        vector<string> valori;
        valori.reserve(nrValori + 1);
        try {
            std::stoi(parametrii[3]);
            valori.emplace_back(parametrii[3]);
        }
        catch (...) {
            int id = t->getColoana("ID")->getLastID();
            id++;
            valori.emplace_back(std::to_string(id));
            valori.emplace_back(parametrii[3]);
        }
        for (int i = 1; i < nrValori; i++) {
            valori.emplace_back(parametrii[i + 3]);
        }
        Rand r(0, valori);
        t->insertRand(r);
        
        
        cout << "Rand inserat!" << '\n';
        break;
    }


    /*case TIP_WHERE: {
        
        if (nrParametrii < 3) {
            cout << "Comanda invalida" << "\n";
            return;
        }
        const char* numeTabela = parametrii[0] + 2;
        const char* numeColoana = parametrii[2];
        const char* op = parametrii[3];
        const char* valoare = parametrii[4];
        Tabela* t = baza.getTabela(numeTabela);
        if (t == nullptr) {
            cout << "Tabela inexistenta!";
        }
        t->selectRand(numeColoana, op, valoare);
        break;
    }*/


    case TIP_SHOW_TABLES: {
        baza.showTables();
        break;
    }

    case TIP_PURGE_TABLE: {
        if (nrParametrii < 3 || nrParametrii > 4) {
            cout << "Comanda invalida\n";
        }
        int conditie = 0;
        if (nrParametrii == 4) {
            if (strcmp(parametrii[3], "--full") == 0) {
                conditie = 1;
            }
        }
        Tabela* tabela = baza.getTabela(parametrii[2]);
        if (tabela == nullptr) {
            cout << "Tabela nu exista \n";
            return;
        }
        char* nume = tabela->getNumeTabela();
        tabela->purgeTable(conditie);
        cout << "Datele din tabela " << nume << " au fost sterse\n";
        delete[] nume;
        break;
    }


    case TIP_DELETE: {
        // delete from tableName where ID == ...
        Tabela* t = baza.getTabela(parametrii[2]);
        int index = t->getIndex(parametrii[6]);
        t->setDeleted(index);
        break;
    }


    case TIP_UPDATE: {
        if (nrParametrii < 6) {
            cout << "Comanda UPDATE invalida! Sintaxa: update <tabela> set <col> = <valoare / operator> [;<col2> = <expr2> ...] [where <conditie>]\n";
            return;
        }

        Tabela* tabela = baza.getTabela(parametrii[1]);
        if (tabela == nullptr) {
            cout << "Tabela nu exista!\n";
            return;
        }

        if (strcmp(parametrii[2], "set") != 0) {
            cout << "Comanda UPDATE invalida! Lipseste cuvantul cheie 'set'.\n";
            return;
        }

        
        int indexWhere = -1;
        for (int i = 3; i < nrParametrii; i++) {
            if (strcmp(parametrii[i], "where") == 0) {
                indexWhere = i;
                break;
            }
        }

        
        vector<string> setTokens;
        int limit = (indexWhere != -1) ? indexWhere : nrParametrii;
        for (int i = 3; i < limit; i++) {
            string t = parametrii[i];
            if (t.length() > 1 && t.back() == ';') {
                setTokens.push_back(t.substr(0, t.length() - 1));
                setTokens.push_back(";");
            }
            else {
                setTokens.push_back(t);
            }
        }

        
        vector<std::pair<string, unique_ptr<ExpresieUpdate>>> setari;
        try {
            size_t pos = 0;
            while (pos < setTokens.size()) {
                string colName = setTokens[pos++];
                if (pos >= setTokens.size() || setTokens[pos] != "=") {
                    throw runtime_error("Sintaxa invalida set: se astepta '=' dupa coloana '" + colName + "'");
                }
                pos++; 

                vector<string> exprTokens;
                while (pos < setTokens.size() && setTokens[pos] != ";") {
                    exprTokens.push_back(setTokens[pos++]);
                }

                if (exprTokens.empty()) {
                    throw runtime_error("Expresie vida pentru coloana '" + colName + "'");
                }

                size_t exprPos = 0;
                auto exprNode = parseExprUpdate(exprTokens, exprPos);
                setari.emplace_back(colName, std::move(exprNode));

                if (pos < setTokens.size() && setTokens[pos] == ";") {
                    pos++; 
                }
            }
        }
        catch (const runtime_error& e) {
            cout << "Eroare la parsarea setarilor: " << e.what() << '\n';
            return;
        }

        
        unique_ptr<NodConditie> arboreConditie = nullptr;
        if (indexWhere != -1) {
            vector<string> whereTokens;
            for (int i = indexWhere + 1; i < nrParametrii; i++) {
                whereTokens.push_back(parametrii[i]);
            }
            if (!whereTokens.empty()) {
                try {
                    size_t posWhere = 0;
                    arboreConditie = parseExpr(whereTokens, posWhere);
                }
                catch (const runtime_error& e) {
                    cout << "Eroare la parsarea clauzei WHERE: " << e.what() << '\n';
                    return;
                }
            }
        }

        
        try {
            int nrActualizari = tabela->updateRand(setari, arboreConditie.get());
            cout << "Au fost realizate " << nrActualizari << " actualizari." << '\n';
        }
        catch (const runtime_error& e) {
            cout << "Eroare in timpul actualizarii datelor: " << e.what() << '\n';
        }
        break;
    }


    case TIP_DESCRIBE: {
        Tabela* t = baza.getTabela(parametrii[2]);
        if (t != nullptr) { t->describeTable(); }
        else { cout << "Tabela nu exista\n"; }
        break;
    }

    case TIP_JOIN: {
        // join tabel1.id on tabel2.id
        char* copieParametru1 = new char[strlen(parametrii[1]) + 1];
        strcpy(copieParametru1, parametrii[1]);
        char* copieParametru2 = new char[strlen(parametrii[3]) + 1];
        strcpy(copieParametru2, parametrii[3]);

        char* numeTabel1 = strtok(copieParametru1, ".");
        char* numeColoana1 = strtok(NULL, ".");
        
        char* numeTabel2 = strtok(copieParametru2, ".");
        char* numeColoana2 = strtok(NULL, ".");

        Tabela* tabela1 = baza.getTabela(numeTabel1);
        Tabela* tabela2 = baza.getTabela(numeTabel2);

        Tabela tabelaJoin = Tabela::joinTables(tabela1, tabela2, numeColoana1, numeColoana2);
        
        cout << tabelaJoin;


        delete[] copieParametru1;
        delete[] copieParametru2;
        
        break;
    }


    case TIP_HELP: {
        cout << "===============================================" << '\n';
        cout << "         SQLSimulator - Comenzi disponibile   " << '\n';
        cout << "===============================================" << '\n';
        cout << '\n';
        cout << "Creare tabela:" << '\n';
        cout << "  aplicatie.exe create table <numeTabel>" << '\n';
        cout << '\n';
        cout << "Adaugare coloana:" << '\n';
        cout << "  aplicatie.exe alter <numeTabel> add column <numeColoana> as date/number/string" << '\n';
        cout << '\n';
        cout << "Stergere coloana:" << '\n';
        cout << "  aplicatie.exe alter <numeTabel> remove column <numeColoana>" << '\n';
        cout << '\n';
        cout << "Inserare rand:" << '\n';
        cout << "  aplicatie.exe insert into <numeTabel> <val1> <val2> ..." << '\n';
        cout << '\n';
        cout << "Afisare tabela completa:" << '\n';
        cout << "  aplicatie.exe select <numeTabel>" << '\n';
        cout << '\n';
        cout << "Afisare rand specific:" << '\n';
        cout << "  aplicatie.exe select <numeTabel> <index>" << '\n';
        cout << '\n';
        cout << "Afisare rand cu clauze:" << '\n';
        cout << "  aplicatie.exe select <numeTabel> where <coloana> <operator> <conditie> [AND/OR] ...." << '\n';
        cout << "  Operatori: 'like', toti operatorii aritmetici " << '\n';
        cout << '\n';
        cout << "Update rand:" << '\n';
        cout << "  aplicatie.exe update <numeTabel> set <coloana> = <valoare / operatie> [where ....]" << '\n';
        cout << "  Operatori: 'like', toti operatorii aritmetici " << '\n';
        cout << '\n';
        cout << "Stergere tabela:" << '\n';
        cout << "  aplicatie.exe drop table <numeTabel>" << '\n';
        cout << '\n';
        cout << "Stergere rand:" << '\n';
        cout << "  aplicatie.exe delete from <numeTabel> where ID == <valoare>" << '\n';
        cout << '\n';
        cout << "Afisare toate tabelele:" << '\n';
        cout << "  aplicatie.exe show tables" << '\n';
        cout << '\n';
        cout << "Stergere totala a datelor din o tabela:" << '\n';
        cout << "  aplicatie.exe purge table <numeTabel> [--full]" << '\n';
        cout << '\n';
        cout << "Ajutor:" << '\n';
        cout << "  aplicatie.exe --help" << '\n';
        cout << "===============================================" << '\n';
        break;
    }

    default:
        cout << "Comanda necunoscuta!" << '\n';
        break;
    }
}

ostream& operator<<(ostream& out, const Interogare& i) {
    out << "Tip interogare: " << i.tipInterogare << '\n';
    out << "Parametrii: ";
    for (int j = 0; j < i.nrParametrii; j++)
        out << i.parametrii[j] << " ";
    out << '\n';
    return out;
}