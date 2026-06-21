#pragma once
#include <memory>
#include <vector>
#include <string>

enum class TipNod { CONDITIE, AND, OR };
enum class TipValoare {NUMBER, COLOANA};
enum class TipOperatie {VALOARE, AdScad, InmImpart};
struct NodConditie {
     TipNod tip;
     std::string coloana, op, valoare;
     std::unique_ptr<NodConditie> stang, drept;
 };

struct ExpresieUpdate {
    TipValoare tipValoare;
    std::string valoare;
    TipOperatie tipOperatie;
    std::string op;
    std::unique_ptr<ExpresieUpdate> stang, drept;
};
std::unique_ptr<NodConditie> parseConditie(const std::vector<std::string>&, size_t& );
std::unique_ptr<NodConditie> parseTerm(const std::vector<std::string>& , size_t&);
std::unique_ptr<NodConditie> parseExpr(const std::vector<std::string>& , size_t& );

std::unique_ptr<ExpresieUpdate> parseFactorUpdate(const std::vector<std::string>&, size_t&);
std::unique_ptr<ExpresieUpdate> parseTermUpdate(const std::vector<std::string>&, size_t&);
std::unique_ptr<ExpresieUpdate> parseExprUpdate(const std::vector<std::string>&, size_t&);