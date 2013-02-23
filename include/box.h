#ifndef _BOX_H
#define _BOX_H

//#ifndef OPAL_USE_DOUBLE
//#define OPAL_USE_DOUBLE
//#endif	/* OPAL_USE_DOUBLE */

#include "physicsobject.h"
#include <opal/opal.h>

class Box
{
	private:
		PhysicsObject* box;
	
	public:
		Box(opal::Simulator* sim, double size, double x, double y, double z);
		PhysicsObject* getBox();
};

#endif /* _BOX_H */
