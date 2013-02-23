#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>

int main (int argc, int argv[])
{
	std::string server_name = "192.168.0.24";
	int server_port = 2129;
	std::string message = "Test message... 1. 2. 3.  EOF\n";
	
	try {
		std::cout << "Connecting to server: " << server_name << ":" << server_port << std::endl;
		ClientSocket client_socket ("192.168.0.24", 2129);

		std::string reply;
		try {
			std::cout << "Sending message: '" << message << "'..." << std::endl;
			client_socket << message;
			std::cout << "Waiting for reply..." << std::endl;
			client_socket >> reply;
			std::cout << "Got reply: '" << reply << "'" << std::endl;
		} catch (SocketException&) {}

		std::cout << "We received this response from the server:\n\"" << reply << "\"\n";

	} catch (SocketException& e) {
		std::cout << "Exception was caught:" << e.description() << "\n";
	}

	std::cout << "Done!" << std::endl;
	return 0;
}
