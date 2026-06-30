#include <Conditie.h>


bool isNumber(const std::string& s) {

    if (s.empty()) return false;
    size_t i = 0;
    if (s[i] == '-' || s[i] == '+') {
        i++;
    }
    if (i == s.size()) return false;
    bool comma = false;
    bool hasDigit = false;
    for (; i < s.size(); ++i) {
        if (s[i] == '.') {
            if (comma) {
                return false;
            }
            comma = true;
        }
        else if(std::isdigit(static_cast<unsigned char>(s[i]))) {
            hasDigit = true;
        }
        else {
            return false;
        }
    }
    return hasDigit;
}


std::unique_ptr<NodConditie>parseConditie(const std::vector<std::string>& t, size_t& pos)
{
    auto nod = std::make_unique<NodConditie>();
    nod->tip = TipNod::CONDITIE;
    nod->coloana = t[pos++];
    nod->op = t[pos++];
    nod->valoare = t[pos++];
    return nod;
}

std::unique_ptr<NodConditie> parseTerm(const std::vector<std::string>& t, size_t& pos)
{
    auto stang = parseConditie(t, pos);
    while (pos < t.size() && t[pos] == "AND") {
        pos++;
        auto drept = parseConditie(t, pos);
        auto nod = std::make_unique<NodConditie>();
        nod->tip = TipNod::AND;
        nod->stang = move(stang);
        nod->drept = move(drept);
        stang = move(nod);
    }
    return stang;
}

std::unique_ptr<NodConditie> parseExpr(const std::vector<std::string>& t, size_t& pos)
{
    auto stang = parseTerm(t, pos);
    while (pos < t.size() && t[pos] == "OR") {
        pos++;
        auto drept = parseTerm(t, pos);
        auto nod = std::make_unique<NodConditie>();
        nod->tip = TipNod::OR;
        nod->stang = move(stang);
        nod->drept = move(drept);
        stang = move(nod);
    }
    return stang;
}


std::unique_ptr<ExpresieUpdate> parseFactorUpdate(const std::vector<std::string>& t, size_t& pos) {
    auto nod = std::make_unique<ExpresieUpdate>();
    nod->tipOperatie = TipOperatie::VALOARE;
    if (isNumber(t[pos])) {
        nod->tipValoare = TipValoare::NUMBER;
        nod->valoare = t[pos++];
    }
    else {
        nod->tipValoare = TipValoare::COLOANA;
        nod->valoare = t[pos++];
    }
    return nod;
}

std::unique_ptr<ExpresieUpdate> parseTermUpdate(const std::vector<std::string>& t, size_t& pos) {
    auto stang = parseFactorUpdate(t, pos);
    while (pos < t.size() && (t[pos] == "*" || t[pos] == "/")) {
        std::string op = t[pos++];
        auto drept = parseFactorUpdate(t, pos);
        auto nod = std::make_unique<ExpresieUpdate>();
        nod->tipOperatie = TipOperatie::InmImpart;
        nod->op = op;
        nod->stang = move(stang);
        nod->drept = move(drept);
        stang = move(nod);
    }
    return stang;
}

std::unique_ptr<ExpresieUpdate> parseExprUpdate(const std::vector<std::string>& t, size_t& pos) {
    auto stang = parseTermUpdate(t, pos);
    while (pos < t.size() && (t[pos] == "+" || t[pos] == "-")) {
        std::string op = t[pos++];
        auto drept = parseTermUpdate(t, pos);
        auto nod = std::make_unique<ExpresieUpdate>();
        nod->tipOperatie = TipOperatie::AdScad;
        nod->op = op;
        nod->stang = move(stang);
        nod->drept = move(drept);
        stang = move(nod);
    }
    return stang;
}