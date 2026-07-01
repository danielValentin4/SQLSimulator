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
	asio::io_context& io_ctx;
	tcp::socket socket;
	tcp::resolver resolver;
	asio::awaitable<void> run(const std::string, const std::string);

public:
	Client(asio::io_context&);
	void start(const std::string&, const std::string&);
};