#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using boost::asio::ip::tcp;

class Client {
public:
    Client(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
        : io_context_(io_context), socket_(io_context) {
        connect(host, port);
    }
    
    // Use boost::asio::post to ensure the async_write is initiated from the io_context's thread,
    // making it thread-safe to call from main().
    void send_message(const std::string& message) {
        std::cout << "Client: Sending message: " << message << std::endl;
        boost::asio::post(io_context_, [this, message]() {
            boost::asio::async_write(
                socket_,
                boost::asio::buffer(message),
                [this](boost::system::error_code ec, std::size_t /*length*/) {
                    if (ec) {
                        std::cerr << "Client write error: " << ec.message() << std::endl;
                    }
                });
        });
    }

private:
    void connect(const std::string& host, const std::string& port) {
        tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve(host, port);
        
        boost::asio::async_connect(
            socket_, 
            endpoints,
            [this](boost::system::error_code ec, tcp::endpoint) {
                if (!ec) {
                    std::cout << "Client: Connected to server" << std::endl;
                    // FIX 1: Start listening for data as soon as we connect.
                    do_read(); 
                } else {
                    std::cout << "Client: Connection failed: " << ec.message() << std::endl;
                }
            });
    }
    
    void do_read() {
        socket_.async_read_some(
            boost::asio::buffer(data_, max_length),
            [this](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::cout << "Client received: " << std::string(data_, length) << std::endl;
                    
                    // FIX 2: Listen for the next message to create a read loop.
                    do_read(); 
                } else {
                    // Don't report an error if the server simply closes the connection.
                    if (ec != boost::asio::error::eof) {
                        std::cerr << "Client read error: " << ec.message() << std::endl;
                    }
                }
            });
    }
    
    boost::asio::io_context& io_context_;
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

// The main function is required for the program to link and run.
int main() {
    try {
        boost::asio::io_context io_context;
        
        // Connect to localhost by default, or specify different host
        std::string host = "127.0.0.1";
        std::string port = "12345";
        
        Client client(io_context, host, port);
        
        // Start the io_context in a separate thread
        std::thread io_thread([&io_context]() {
            try {
                io_context.run();
            } catch (std::exception& e) {
                std::cerr << "IO thread exception: " << e.what() << std::endl;
            }
        });
        
        // Give connection time to establish
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Send some messages
        std::cout << "Client: Sending messages..." << std::endl;
        
        client.send_message("Hello from Client!");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        client.send_message("Message 2");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        client.send_message("Goodbye!");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        std::cout << "Client: Finished sending messages. Press Enter to exit..." << std::endl;
        std::cin.get();
        
        io_context.stop();
        if (io_thread.joinable()) {
            io_thread.join();
        }
        
    } catch (std::exception& e) {
        std::cerr << "Client exception: " << e.what() << std::endl;
    }
    
    return 0;
}
