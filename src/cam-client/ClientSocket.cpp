// Implementation of the ClientSocket class

#include "ClientSocket.h"
#include "SocketException.h"
#include <sstream>

ClientSocket::ClientSocket (std::string host, int port)
{
	if (! Socket::create())
		throw SocketException ("Could not create client socket.");

	if (! Socket::connect (host, port))
		throw SocketException ("Could not bind to port.");

}

ClientSocket::ClientSocket () {}

void ClientSocket::connect(std::string host, int port)
{
	if (!Socket::create())
		throw SocketException("Could not create client socket.");
	
	if (!Socket::connect(host, port))
		throw SocketException("Could not bind to port.");
}


const ClientSocket& ClientSocket::operator << (const std::string& s) const
{
	if (! Socket::send (s)) {
		throw SocketException ("Could not write to socket.");
	}

	return *this;

}

const ClientSocket& ClientSocket::operator << (const int& i) const
{
	std::ostringstream oss;
	oss.setf(std::ios::fixed);
	oss.setf(std::ios::showpoint);
	oss << i;
	if (!Socket::send(oss.str()))
		throw SocketException("Could not write to socket.");
	return *this;
}

const ClientSocket& ClientSocket::operator << (const double& d) const
{
	std::ostringstream oss;
	oss.setf(std::ios::fixed);
	oss.setf(std::ios::showpoint);
	oss << d;
	if (!Socket::send(oss.str()))
		throw SocketException("Could not write to socket.");
	return *this;
}

const ClientSocket& ClientSocket::operator >> (std::string& s) const
{
	if (! Socket::recv (s)) {
		throw SocketException ("Could not read from socket.");
	}

	return *this;
}
