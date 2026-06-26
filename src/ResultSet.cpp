#include "ResultSet.h"

ResultSet::ResultSet()
{
	numeColoane.reserve(5);
	randuri.reserve(5);
}

ResultSet::ResultSet(const Tabela& t)
{
	numeColoane.reserve(t.getNrColoane());
	randuri.reserve(t.getNrRanduri());
	
	for (int i = 0; i < t.getNrColoane(); i++) {
		numeColoane.emplace_back(t.getNumeColoana(i));
	}

	for (int i = 0; i < t.getNrRanduri(); i++) {
		randuri.emplace_back(t.getRand(i));
	}
}

ResultSet::ResultSet(vector<string> numeCol, vector<Rand> rand) : numeColoane(numeCol), randuri(rand) {}

ResultSet::ResultSet(const ResultSet& result) : numeColoane(result.numeColoane), randuri(result.randuri) {}
ResultSet& ResultSet::operator=(const ResultSet& r) 
{
	if (this != &r) {
		this->numeColoane = r.numeColoane;
		this->randuri = r.randuri;
	}
	return *this;
}

int ResultSet::getNrColoane() const
{
	return numeColoane.size();
}

int ResultSet::getNrRanduri() const {
	return randuri.size();
}

string ResultSet::getValue(int indexRand, int indexColoana)
{
	if (indexRand >= 0 && indexRand < randuri.size()) {
		Rand row = randuri.at(indexRand);
		return row[indexColoana];
	}
	else {
		throw runtime_error("Index invalid");
	}
}

Rand ResultSet::operator[](int index) const {
	if (index >= 0 && index < randuri.size()) return randuri.at(index);
	throw runtime_error("Index invalid");
}

vector<string> ResultSet::getNumeColoane() const
{
	return vector<string>(numeColoane);
}

vector<size_t> calculareWidth(const ResultSet& resultSet)
{
	vector<size_t> width(resultSet.getNrColoane());
	for (size_t i = 0; i < resultSet.getNrColoane(); i++) {
		width[i] = resultSet.numeColoane[i].length();
		for (size_t j = 0; j < resultSet.getNrRanduri(); j++) {
			Rand r = resultSet.randuri[j];
			width[i] = std::max(width[i], r[i].length());
		}
	}
	return width;
}

ostream& operator<<(ostream& out, const ResultSet& r)
{
	if (r.numeColoane.size() == 0) { out << "Tabela goala! \n"; return out; }
	auto width = calculareWidth(r);

	size_t nrLinii = 1;
	for (auto w : width) {
		nrLinii += w + 3;
	}
	string separator(nrLinii, '-');
	out << separator << '\n';

	for (size_t i = 0; i < r.numeColoane.size(); i++) {
		out << "| " << std::left << std::setw(width[i]) << r.numeColoane[i] << " ";
	}

	out << "| \n";
	out << separator << "\n";
	for (size_t i = 0; i < r.randuri.size(); i++) {
		Rand rand = r.randuri[i];
		for (size_t j = 0; j < r.numeColoane.size(); j++) {
			out << "| " << std::left << std::setw(width[j]) << rand[j] << " ";
		}
		out << "|" << "\n" << separator << "\n";
	}
	return out;
}


