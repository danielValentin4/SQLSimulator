#include "Client.h"

Client::Client(asio::io_context& io) : socket(io),
resolver(io),
strand(io.get_executor()) {}

void Client::start(const string& host, const string& port) {
	connect(host, port);
}

void Client::connect(const string& host, const string& port) {
	auto self = shared_from_this();
	resolver.async_resolve(host, port,
		[this, self](asio::error_code ec, tcp::resolver::results_type endpoints) {
			if (ec) { std::cerr << "Error on resolve \n"; return; }
			asio::async_connect(socket, endpoints,
				[this, self](asio::error_code ec, tcp::endpoint) {
					if (ec) { std::cerr << "Connection error \n"; return; }
					read();
				});
		});
}

void Client::read() {
	auto self = shared_from_this();
	asio::async_read(socket, asio::buffer(bufferSize, 4),
		asio::bind_executor(strand,
			[this, self](asio::error_code ec, size_t bytes) {
				if (ec) { std::cerr << "Read error \n"; return; }
				//uint32_t size = ntohl(*reinterpret_cast<uint32_t*>(bufferSize));
				uint32_t netSize;
				std::memcpy(&netSize, bufferSize, sizeof(netSize));
				uint32_t size = ntohl(netSize);
				readPayload(size);
			}));
}

void Client::readPayload(uint32_t size) {
	auto self = shared_from_this();
	payLoadBuffer.resize(size);
	asio::async_read(socket, asio::buffer(payLoadBuffer),
		asio::bind_executor(strand,
			[this, self](asio::error_code ec, size_t bytes) {
				if (ec) { std::cerr << "Payload error \n"; return; }
				ResultSet rs = ResultSet::deserialize(payLoadBuffer.data(), payLoadBuffer.size());
				std::cout << rs;
				read();
			}));
}

void writeBytes(vector<uint8_t>& buffer, const void* data, size_t size) {
	const uint8_t* ptr = reinterpret_cast<const uint8_t*>(data);
	buffer.insert(buffer.end(), ptr, ptr + size);
}

void writeString(vector<uint8_t>& buffer, const string& s) {
	uint32_t length = s.size();
	writeBytes(buffer, &length, 4);
	writeBytes(buffer, s.data(), length);
}

void Client::send(const vector<string>& args) {
	vector<uint8_t> payload;
	uint32_t argc = args.size();
	writeBytes(payload, &argc, 4);
	for (const auto& s : args) {
		writeString(payload, s);
	}

	auto buffer = std::make_shared<vector<uint8_t>>();
	buffer->resize(4 + payload.size());
	uint32_t netSize = htonl(payload.size());
	memcpy(buffer->data(), &netSize, 4);
	memcpy(buffer->data() + 4, payload.data(), payload.size());
	auto self = shared_from_this();
	asio::post(strand, [this, self, buffer]() {
		asio::async_write(socket, asio::buffer(*buffer),
			asio::bind_executor(strand,
				[this, self, buffer](asio::error_code ec, size_t) {
					if (ec) { std::cerr << "Send error \n"; return; }
				}));
		});
}

int main() {
	std::cout.setf(std::ios::unitbuf);
	asio::io_context io;
	auto client = std::make_shared<Client>(io);
	client->start("127.0.0.1", "5000");
	std::thread t([&io] { io.run();});
	string line;
	while (std::getline(std::cin, line)) {
		if (line == "disconnect") {
			break;
		}
		vector<string> args;
		std::istringstream iss(line);
		string token;
		while (iss >> token) {
			args.push_back(token);
		}
		if (!args.empty()) {
			client->send(args);
		}
	}

	io.stop();
	t.join();
}