#pragma once
#include "Session.h"
#include "BazaDeDate.h"

using asio::ip::tcp;

class Server {
private:
	tcp::acceptor acceptor;
	asio::thread_pool& dbPool;
	BazaDeDate& db;

	void accept();
public:
	Server(asio::io_context&, asio::thread_pool&, uint16_t, BazaDeDate&);
};