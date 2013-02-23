#include <opal/opal.h>

int main()
{
	opal::Simulator* sim = opal::createSimulator(); 
	opal::Vec3r g(0, -9.81, 0);
	sim->setGravity(g);
	sim->setStepSize(0.02);
	return 0;
}
