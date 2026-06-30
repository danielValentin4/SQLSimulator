#pragma once
#include <asio.hpp>
#include <memory>
#include <vector>
#include "BazaDeDate.h"
#include "Interogare.h"
#include "ResultSet.h"
using asio::ip::tcp;
class Session : public std::enable_shared_from_this<Session> {
private:
	tcp::socket _socket;
	asio::strand<asio::any_io_executor> strand;
	BazaDeDate& db;
	uint8_t bufferSize[4];
	vector<uint8_t> payloadBuffer;
	asio::thread_pool& dbPool;

	uint32_t readU32(size_t& offset);
	string readString(size_t& offset);
	void write(const ResultSet& rs);
	void readPaylod(uint32_t);
	void read();
public:
	Session(tcp::socket socket,asio::thread_pool&, BazaDeDate& db);
	void start();
};