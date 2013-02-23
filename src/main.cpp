#include <cstdlib>
#include <icarus.h>

// --- Lib Stuff --- //
#include <vrj/Kernel/Kernel.h>

//#define OSG_USE_IO_DOT_H


int main (int argc, char *argv[])
{
	vrj::Kernel * kernel = vrj::Kernel::instance ();	// Get the kernel
	Icarus *application = new Icarus (kernel);	// Instantiate an instance of the app

	if (argc <= 1)
	{
		// display some usage info (holding the user by the hand stuff)
		//  this will probably go away once the kernel becomes separate
		//  and can load application plugins.
		std::cout << "\n" << std::flush;
		std::cout << "\n" << std::flush;

		std::cout << "Usage: " << argv[0]
			<<
			"vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]\n"
			<< std::endl << std::endl;

		std::exit (1);
	}

	// Load any config files specified on the command line
	for (int i = 1; i < argc; ++i)
	{
		kernel->loadConfigFile (argv[i]);
	}

	kernel->start ();

	kernel->setApplication (application);
	kernel->waitForKernelStop ();

	delete application;

	return 0;
}
