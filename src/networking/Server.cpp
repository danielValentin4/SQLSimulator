#include "Server.h"

void Server::accept() {
	acceptor.async_accept(
		[this](asio::error_code ec, tcp::socket socket) {
			if (!ec) {
				std::make_shared<Session>(std::move(socket), dbPool, db)->start();
			}
			accept();
		});
}


Server::Server(asio::io_context& io, asio::thread_pool& dbPool, uint16_t port, BazaDeDate& db) :
	acceptor(io, tcp::endpoint(tcp::v4(), port)),
	dbPool(dbPool),
	db(db) {
	accept();
}