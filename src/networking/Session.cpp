#include "Session.h"

uint32_t Session::readU32(size_t& offset) {
	uint32_t value;
	memcpy(&value, payloadBuffer.data() + offset, 4);
	offset += 4;
	return value;
}

string Session::readString(size_t& offset) {
	uint32_t length = readU32(offset);
	string s(reinterpret_cast<char*>(payloadBuffer.data() + offset), length);
	offset += length;
	return s;
}

void Session::write(const ResultSet& rs) {
	auto self = shared_from_this();
	auto payload = std::make_shared<vector<uint8_t>>(serialize(rs));
	uint32_t netSize = htonl(payload->size());

	auto buf = std::make_shared<vector<uint8_t>>();
	buf->resize(4 + payload->size());
	memcpy(buf->data(), &netSize, 4);
	memcpy(buf->data() + 4, payload->data(), payload->size());

	asio::async_write(_socket, asio::buffer(*buf),
		asio::bind_executor(strand,
			[this, self, buf](asio::error_code ec, size_t bytesWritten) {
				if (ec) std::cout << "Eroare la scrierea rezultatului \n";
				read();
			}));
}

void Session::read()
{
	auto self = shared_from_this();
	asio::async_read(_socket, asio::buffer(bufferSize, 4),
		asio::bind_executor(strand,
			[this, self](asio::error_code ec, size_t) {
				if (ec) {std::cout << "Client deconectat \n"; return;}
				//uint32_t size = ntohl(*reinterpret_cast<uint32_t*>(bufferSize));
				uint32_t netSize;
				std::memcpy(&netSize, bufferSize, sizeof(netSize));
				uint32_t size = ntohl(netSize);
				readPaylod(size);
			}));
}

Session::Session(tcp::socket socket,asio::thread_pool& dPool, BazaDeDate& db) : _socket(std::move(socket)),strand(_socket.get_executor()),dbPool(dPool), db(db) {}

void Session::start()
{
	read();
}


void Session::readPaylod(uint32_t size)
{
	auto self = shared_from_this();
	payloadBuffer.resize(size);
	asio::async_read(_socket, asio::buffer(payloadBuffer),
		asio::bind_executor(strand,
			[this, self](asio::error_code ec, size_t) {
				if (ec) {
					std::cout << "Eroare la citirea comenzii \n"; return;
				}

				size_t offset = 0;

				uint32_t argc = readU32(offset);

				vector<string> args;
				args.reserve(argc);
				
				
				for (uint32_t i = 0; i < argc; i++) {
					args.emplace_back(readString(offset));
					
				}

				
				asio::post(dbPool, [this, self, args, argc]() mutable {
					vector<char*> argv;
					argv.reserve(argc);
					for (auto& s : args) {
						argv.emplace_back(s.data());
					}
					Interogare interogare = Interogare(argv.data(), argc);
					ResultSet rs = interogare.executa(db);
					
					asio::post(strand, [this, self, rs = std::move(rs)]() {
						std::cout << rs;
						write(rs);
						});

					});
			}));
}


