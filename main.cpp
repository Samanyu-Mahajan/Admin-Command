#include "admin.cpp"
#include <boost/asio.hpp>
#include <iostream>

int main() {
    try {
        boost::asio::io_context io_context;
        Admin admin(io_context, "/tmp/my_socket");  // Instantiates Admin and starts listening
        io_context.run();  // Run the event loop

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
