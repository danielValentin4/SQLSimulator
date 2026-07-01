#include "Client.h"

void writeBytes(vector<uint8_t>& buffer, const void* data, size_t size) {
	const uint8_t* ptr = reinterpret_cast<const uint8_t*>(data);
	buffer.insert(buffer.end(), ptr, ptr + size);
}

void writeString(vector<uint8_t>& buffer, const string& s) {
	uint32_t length = s.size();
	writeBytes(buffer, &length, 4);
	writeBytes(buffer, s.data(), length);
}

Client::Client(asio::io_context& io) : io_ctx(io), socket(io), resolver(io) {}

asio::awaitable<void> Client::run(const std::string host, const std::string port)
{
	auto self = shared_from_this();
	try {
		auto endpoints = co_await resolver.async_resolve(host, port, asio::use_awaitable);
		co_await asio::async_connect(socket, endpoints, asio::use_awaitable);
		while (true) {
			//Send
			vector<uint8_t> payLoadBuffer;
			uint32_t netSize;
			string line;
			vector<string> args;
			if (!std::getline(std::cin, line)) {
				break;
			}
			if (line == "disconnect") {
				break;
			}
			std::istringstream iss(line);
			string token;
			while (iss >> token) {
				args.push_back(token);
			}
			
			uint32_t argc = args.size();
			writeBytes(payLoadBuffer, &argc, 4);
			for (const auto& s : args) {
				writeString(payLoadBuffer, s);
			}
			auto bufferWrite = std::make_shared<vector<uint8_t>>();
			bufferWrite->resize(4 + payLoadBuffer.size());
			netSize = htonl(payLoadBuffer.size());
			memcpy(bufferWrite->data(), &netSize, 4);
			memcpy(bufferWrite->data() + 4, payLoadBuffer.data(), payLoadBuffer.size());
			co_await asio::async_write(socket, asio::buffer(*bufferWrite), asio::use_awaitable);

			// Read
			uint8_t buffer[4];
			payLoadBuffer.clear();
			co_await asio::async_read(socket, asio::buffer(buffer, 4), asio::use_awaitable);
			
			memcpy(&netSize, buffer, 4);
			uint32_t size = ntohl(netSize);

			payLoadBuffer.resize(size);

			co_await asio::async_read(socket, asio::buffer(payLoadBuffer), asio::use_awaitable);
			ResultSet rs = ResultSet::deserialize(payLoadBuffer.data(), payLoadBuffer.size());
			std::cout << rs;
		}
	}
	catch (const std::exception& exc) {
		cout << exc.what();
	}

	io_ctx.stop();

}

void Client::start(const string& host, const string& port) {
	asio::co_spawn(socket.get_executor(), run(host, port), asio::detached);
}

int main() {
	std::cout.setf(std::ios::unitbuf);
	asio::io_context io;
	auto client = std::make_shared<Client>(io);
	client->start("127.0.0.1", "5000");
	std::thread t([&io] { io.run();});
	t.join();
}

