#pragma once
#include <asio.hpp>
#include <memory>
#include <vector>
#include <chrono>
#include "BazaDeDate.h"
#include "Interogare.h"
#include "ResultSet.h"
using asio::ip::tcp;
using clk = std::chrono::high_resolution_clock;
class Session : public std::enable_shared_from_this<Session> {
private:
	tcp::socket _socket;
	BazaDeDate& db;
	asio::thread_pool& dbPool;

	asio::awaitable<void> run();
public:
	Session(tcp::socket socket,asio::thread_pool&, BazaDeDate& db);
	void start();
};