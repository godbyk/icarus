#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <iostream>

int main (int argc, int argv[])
{
  std::cout << "running....\n";

  try
    {
      // Create the socket
      ServerSocket server (2129);

      while (true)
	{

	  ServerSocket new_sock;
	  server.accept (new_sock);

	  try
	    {
	      while (true)
		{
		  std::string data;
		  new_sock >> data;
		  new_sock << data;
		  std::cout << "Received: " << data << std::endl;
		}
	    }
	  catch (SocketException&) {}

	}
    }
  catch (SocketException& e)
    {
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

  return 0;
}
