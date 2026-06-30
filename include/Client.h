#pragma once
#include <asio.hpp>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include "ResultSet.h"


using asio::ip::tcp;


class Client : public std::enable_shared_from_this<Client> {
private:
	tcp::socket socket;
	tcp::resolver resolver;
	asio::strand<asio::any_io_executor> strand;
	uint8_t bufferSize[4];
	vector<uint8_t> payLoadBuffer;

	void connect(const string&, const string&);
	void read();
	void readPayload(uint32_t);

public:
	Client(asio::io_context&);
	void start(const std::string&, const std::string&);
	void send(const vector<string>&);

};