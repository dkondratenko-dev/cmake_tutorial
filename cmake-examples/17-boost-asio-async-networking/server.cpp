#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <string>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> { 
    //When a class inherits from std::enable_shared_from_this, it can safely create a std::shared_ptr to itself from within a member function.
    //The calls to shared_from_this() below ensure that all shared_ptr instances managing the object share ownership correctly, preventing issues like double deletion or dangling pointers.==> important in asynchronous operations (like Boost.Asio handlers) to keep the object alive as long as the operation is pending.
public:
    Session(tcp::socket socket) : socket_(std::move(socket)) {}
    
    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());//Shared pointer - also see line 32 - This keeps the Session object alive during the async operation, even if the original owner goes out of scope
        //The alternative is to use a regular shared pointer in two places, but the two places can call delete, causing double delete
        //auto p = std::make_shared<Session>(this);

        socket_.async_read_some(
            boost::asio::buffer(data_, max_length),//Read data less or equal to max_length
            [this, self](boost::system::error_code ec, std::size_t length) { //Actual length read is provided to the lambda
                if (!ec) { //If no error during read, then write
                    std::cout << "Server received: " << std::string(data_, length) << std::endl;//data_ is a closure in the lambda
                    do_write(length);//Then wrote out the data
                }
            });
    }
    
    void do_write(std::size_t length) {
        auto self(shared_from_this());//Shared pointer - also see line 18 - This keeps the Session object alive during the async operation, even if the original owner goes out of scope
        //The alternative is to use a regular shared pointer in two places, but the two places can call delete, causing double delete
        //auto p = std::make_shared<Session>(this);
        
        // Echo the message back with a prefix
        std::string response = "Echo: " + std::string(data_, length);
        std::copy(response.begin(), response.end(), data_);
        
        boost::asio::async_write(
            socket_, 
            boost::asio::buffer(data_, response.length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) { //If no error during write, then read
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
                    std::make_shared<Session>(std::move(socket))->start();//Move object and call function immediately
                }
                do_accept();//Asynchronous recursive
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
