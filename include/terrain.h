/***************************************************************************
 *            terrain.h
 *
 *  Tue Feb 28 11:24:54 2006
 *  Copyright  2006  User
 *  Email
 ****************************************************************************/

#ifndef _TERRAIN_H
#define _TERRAIN_H

//#ifndef OPAL_USE_DOUBLE
//#define OPAL_USE_DOUBLE
//#endif	/* OPAL_USE_DOUBLE */

#include <string>
#include <osg/Math>
#include <opal/opal.h>
#include "physicsobject.h"


class Terrain
{
	private:
		PhysicsObject* ground;
	
	public:
		Terrain(opal::Simulator* sim);
		PhysicsObject* getTerrain();
};

#endif /* _TERRAIN_H */
