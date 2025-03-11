#include <boost/asio.hpp>
#include <iostream>
#include <array>
#include <memory>

class Admin {
public:
    Admin(boost::asio::io_context& io_context, const std::string& socket_path)
        : io_context_(io_context), socket_(io_context), endpoint_(socket_path),
        acceptor_(io_context) {  // Initialize acceptor without binding
        std::remove(endpoint_.path().c_str());  // Remove old socket file

        acceptor_.open();
        acceptor_.bind(endpoint_);
        acceptor_.listen();

        std::cout << "Server listening on " << endpoint_.path() << "...\n";
        accept_connection();
    }

private:
    boost::asio::io_context& io_context_;
    boost::asio::local::stream_protocol::socket socket_;
    boost::asio::local::stream_protocol::endpoint endpoint_;
    boost::asio::local::stream_protocol::acceptor acceptor_;
    boost::asio::streambuf buffer_;

    // Accept incoming connections
    void accept_connection() {
        acceptor_.async_accept(socket_, [this](const boost::system::error_code& ec) {
            if (!ec) {
                std::cout << "Client connected!\n";
                execute();  // Start reading asynchronously
            } else {
                std::cerr << "Accept error: " << ec.message() << std::endl;
            }
            // accept_connection();
        });
    }
    

    // Read asynchronously and call callback function
    void execute() {
        boost::asio::async_read_until(socket_, buffer_, '\n',
            [this](const boost::system::error_code& ec, std::size_t bytes_transferred) {
                if (!ec) {
                    // client sends one message and then closes connection
                    // this is going to execute the first time when a message is read
                    std::istream is(&buffer_);
                    std::string message;
                    std::getline(is, message);

                    parse_and_print(message);

                    if (message == "exit") {
                        std::cout << "Exit command received. Shutting down server.\n";
                        socket_.close();
                        return;
                    }

                    execute();  // Continue reading asynchronously
                } else {
                    // this is going to execute second time since the client closed connection, EOF is encountered.
                    std::cerr << "Read error: " << ec.message() << std::endl;
                    socket_.close();
                    socket_ = boost::asio::local::stream_protocol::socket(io_context_);
                    accept_connection();
                }
            });
    }

    // Simple callback function to process received data
    void parse_and_print(const std::string& data) {
        std::cout << "Received: " << data << std::endl;
    }
};
