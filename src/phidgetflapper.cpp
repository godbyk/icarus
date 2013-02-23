#include <iostream>
#include <string>
#include <fstream>
#include <ClientSocket.h>
#include <SocketException.h>
#include <sys/time.h>       // for timestamp
#include <time.h>           // for timestamp
#include <math.h>
#include <unistd.h>

// forward declarations
double getTimeStamp();
void calcY(int vl, int vr, double &yl, double &yr);

// constants
const double leftZero = 2975.0;
const double leftVert = 1235.0;
const double rightZero = 1165.0;
const double rightVert = 2835.0;
const double avgDiff = (leftZero - leftVert + rightVert - rightZero) / 2.0;
const double angleFactor = (M_PI / 2.0) / avgDiff;

const unsigned long timediff = 100;	// time to wait until next output in seconds

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		std::cout << "Usage: phidgetflapper phidgetkitpath serverip" << std::endl;
		return 0;
	}
	// set up file input
	std::string path(argv[1]);
	std::ifstream infile;
	std::string leftpath = path + "/sensor1";
	std::string rightpath = path + "/sensor2";
	int left = 0;
	int right = 0;
	double yLeft = 0.0;
	double yRight = 0.0;
	
	
	
	// set up socket stuff
	int portNum = 2129;
	std::string ip(argv[2]);
	ClientSocket socket;
	try 
	{
		std::cout << "Connecting to server: " << ip << ":" << portNum << std::endl;
		socket.connect(ip, portNum);
	} 
	catch (SocketException& e) 
	{
		std::cout << "Exception was caught:" << e.description() << std::endl;
	}
	while(true)
	{
		infile.open(leftpath.c_str());
		infile >> left;
		infile.close();
		infile.open(rightpath.c_str());
		infile >> right;
		infile.close();
		
		calcY(left, right, yLeft, yRight);
		
		//std::cout << "[ " << getTimeStamp() << ", " << yLeft << ", " << yRight << " ] " << std::endl;
		
		try 
		{
			socket << "@" << getTimeStamp() << " " << yLeft << " " << yRight << "!\n";
		} 
		catch (SocketException& e) 
		{
			std::cout << "Exception:" << e.description() << std::endl;
			return 0;
		}
		usleep(timediff);
	}
	return 0;
}

double getTimeStamp()
{
    // Returns the elapsed time since the program started (in microsec)
    struct timeval tval;
    struct timezone tzone;
    gettimeofday(&tval, &tzone);
    return (double)tval.tv_sec * 1000000.0 + (double)tval.tv_usec;
}

void calcY(int vl, int vr, double &yl, double &yr)
{
	yl = sin((leftZero - vl) * angleFactor);
	yr = sin((vr - rightZero) * angleFactor);
}
