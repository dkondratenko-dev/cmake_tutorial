#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <string>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket) : socket_(std::move(socket)) {}
    
    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(
            boost::asio::buffer(data_, max_length),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::cout << "Server received: " << std::string(data_, length) << std::endl;
                    do_write(length);
                }
            });
    }
    
    void do_write(std::size_t length) {
        auto self(shared_from_this());
        
        // Echo the message back with a prefix
        std::string response = "Echo: " + std::string(data_, length);
        std::copy(response.begin(), response.end(), data_);
        
        boost::asio::async_write(
            socket_, 
            boost::asio::buffer(data_, response.length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    do_read();
                }
            });
    }
    
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

class Server {
public:
    Server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::cout << "Server: New client connected" << std::endl;
                    std::make_shared<Session>(std::move(socket))->start();
                }
                do_accept();
            });
    }
    
    tcp::acceptor acceptor_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        
        const short port = 12345;
        Server server(io_context, port);
        
        std::cout << "Server: Listening on port " << port << std::endl;
        std::cout << "Server: Press Ctrl+C to stop" << std::endl;
        
        io_context.run();
        
    } catch (std::exception& e) {
        std::cerr << "Server exception: " << e.what() << std::endl;
    }
    
    return 0;
}
