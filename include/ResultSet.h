#include "Tabela.h"

class ResultSet {
private:
	vector<string> numeColoane;
	vector<Rand> randuri;
public:
	ResultSet();
	ResultSet(const Tabela&);
	ResultSet(vector<string>, vector<Rand>);
	ResultSet(const ResultSet&);
	ResultSet& operator=(const ResultSet&);
	int getNrColoane() const;
	int getNrRanduri() const;
	Rand operator[](int) const;
	vector<string> getNumeColoane() const;
	friend ostream& operator<<(ostream&, const ResultSet&);
	friend vector<size_t> calculareWidth(const ResultSet&);
};

