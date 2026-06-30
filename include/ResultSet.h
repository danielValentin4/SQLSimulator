#pragma once
#include "Tabela.h"

class ResultSet {
private:
	vector<string> numeColoane;
	vector<Rand> randuri;
	string message = "";
public:
	ResultSet();
	ResultSet(const Tabela&);
	ResultSet(vector<string>, vector<Rand>);
	ResultSet(const ResultSet&);
	ResultSet(string);
	ResultSet& operator=(const ResultSet&);
	int getNrColoane() const;
	int getNrRanduri() const;
	string getValue(int,int);
	Rand operator[](int) const;
	vector<string> getNumeColoane() const;
	friend ostream& operator<<(ostream&, const ResultSet&);
	friend vector<size_t> calculareWidth(const ResultSet&);
	friend vector<uint8_t> serialize(const ResultSet&);
	static ResultSet deserialize(const uint8_t*, size_t);
};

