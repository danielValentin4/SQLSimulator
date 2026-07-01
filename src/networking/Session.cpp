#include "Session.h"

asio::awaitable<void> Session::run() {
	auto self = shared_from_this();
	try {
		while (true) {
			uint8_t buffer[4];
			vector<uint8_t> payloadBuffer;
			// Read the size of the message
			co_await asio::async_read(_socket, asio::buffer(buffer, 4), asio::use_awaitable);
			const auto start = clk::now();
			uint32_t netSize;
			memcpy(&netSize, buffer, 4);
			uint32_t size = ntohl(netSize);


			payloadBuffer.resize(size);
			// Read the message
			co_await asio::async_read(_socket, asio::buffer(payloadBuffer), asio::use_awaitable);
			size_t offset = 0;
			auto readU32 = [&]() {
				uint32_t value;
				memcpy(&value, payloadBuffer.data() + offset, 4);
				offset += 4;
				return value;
				};
			auto readString = [&]() {
				uint32_t length = readU32();
				string s(reinterpret_cast<char*>(payloadBuffer.data() + offset), length);
				offset += length;
				return s;
				};

			uint32_t argc = readU32();

			vector<string> args;
			args.reserve(argc);
			vector<char*> argv;
			argv.reserve(argc);

			//Deserialize the message into args
			for (uint32_t i = 0; i < argc; i++) {
				args.emplace_back(readString());
				argv.emplace_back(args.back().data());
			}

			Interogare interogare = Interogare(argv.data(), argc);
			ResultSet rs = interogare.executa(db);

			auto payload = make_shared<vector<uint8_t>>(serialize(rs));
			uint32_t responseSize = htonl(payload->size());
			vector<uint8_t> buf(4 + payload->size());
			memcpy(buf.data(), &responseSize, 4);
			memcpy(buf.data() + 4, payload->data(), payload->size());
			auto diff = clk::now() - start;
			std::chrono::duration<double, std::milli> ms = diff;
			//Send the response
			std::cout << std::format("Au durat {:} milisecunde procesarea cererii \n", ms.count());
			co_await asio::async_write(_socket, asio::buffer(buf), asio::use_awaitable);

		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\n";
	}
}

Session::Session(tcp::socket socket,asio::thread_pool& dPool, BazaDeDate& db) : _socket(std::move(socket)),dbPool(dPool), db(db) {}

void Session::start()
{
	std::cout << "Client conectat. \n";
	asio::co_spawn(_socket.get_executor(), run(), asio::detached);
}


