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
		if (!t.isDeleted(i)) {
			randuri.emplace_back(t.getRand(i));
		}
	}
}

ResultSet::ResultSet(vector<string> numeCol, vector<Rand> rand) : numeColoane(numeCol), randuri(rand) {}

ResultSet::ResultSet(const ResultSet& result) : numeColoane(result.numeColoane), randuri(result.randuri) { this->message = result.message; }

ResultSet::ResultSet(string message) {
	this->message = message;
}
ResultSet& ResultSet::operator=(const ResultSet& r) 
{
	if (this != &r) {
		this->numeColoane = r.numeColoane;
		this->randuri = r.randuri;
		this->message = r.message;
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


void writeBytesRS(vector<uint8_t>& buffer, const void* data, size_t size) {
	const uint8_t* ptr = reinterpret_cast<const uint8_t*>(data);
	buffer.insert(buffer.end(), ptr, ptr + size);
}

void writeStringRS(vector<uint8_t>& buffer, const std::string& s) {
	uint32_t len = s.size();
	writeBytesRS(buffer, &len, 4);
	writeBytesRS(buffer, s.data(), len);
}

void writeStringVectorRS(vector<uint8_t>& buffer, const vector<string>& vector) {
	uint32_t count = vector.size();
	writeBytesRS(buffer, &count, 4);
	for (const auto& s : vector) {
		writeStringRS(buffer, s);
	}
}

void writeRowVectorRS(vector<uint8_t>& buffer, const vector<Rand>& vector) {
	uint32_t count = vector.size();
	writeBytesRS(buffer, &count, 4);
	for (const auto& r : vector) {
		uint32_t numberOfElements = (int)r;
		writeBytesRS(buffer, &numberOfElements, 4);
		for (size_t i = 0; i < numberOfElements; i++) {
			writeStringRS(buffer, r[i]);
		}
	}
}


Rand readRow(const uint8_t* data, size_t& offset) {
	uint32_t count;
	memcpy(&count, data + offset, 4);
	offset += 4;
	vector<string> date;
	date.reserve(count);
	for (uint32_t i = 0; i < count; i++) {
		uint32_t len;
		memcpy(&len, data + offset, 4);
		offset += 4;
		string s(reinterpret_cast<const char*>(data + offset), len);
		offset += len;
		date.emplace_back(move(s));
	}

	return Rand(0, move(date));
}

vector<Rand> readRows(const uint8_t* data, size_t& offset) {
	uint32_t count;
	memcpy(&count, data + offset, 4);
	offset += 4;
	vector<Rand> rows;
	rows.reserve(count);
	for (uint32_t i = 0; i < count; i++) {
		rows.emplace_back(readRow(data, offset));
	}
	return rows;
}


vector<uint8_t> serialize(const ResultSet& rs)
{
	vector<uint8_t> buffer;
	writeStringRS(buffer, rs.message);
	writeStringVectorRS(buffer, rs.numeColoane);
	writeRowVectorRS(buffer, rs.randuri);
	return buffer;
}

ResultSet ResultSet::deserialize(const uint8_t* data, size_t size)
{
	size_t offset = 0;
	auto readU32 = [&]() {
		uint32_t value;
		memcpy(&value, data + offset, 4);
		offset += 4;
		return value;
		};

	auto readString = [&]() {
		uint32_t len = readU32();
		string s(reinterpret_cast<const char*>(data + offset), len);
		offset += len;
		return s;
		};

	auto readColumns = [&]() {
		uint32_t count = readU32();
		vector<string> vector;
		for (uint32_t i = 0; i < count; i++) {
			vector.push_back(readString());
		}
		return vector;
		};

	string message = readString();
	vector<string> numeColoane = readColumns();
	vector<Rand> randuri = readRows(data, offset);
	if (message == "") {
		ResultSet rs{ numeColoane, randuri };
		return rs;
	}
	else {
		ResultSet rs{ message };
		return rs;
	}
	/*ResultSet rs{ numeColoane, randuri };
	return rs;*/
}



ostream& operator<<(ostream& out, const ResultSet& r)
{
	if (r.message == "") {
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
	}
	else {
		out << r.message;
	}
	return out;
}


