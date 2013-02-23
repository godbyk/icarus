#ifndef _PHYSICSOBJECT_H
#define _PHYSICSOBJECT_H

//#ifndef OPAL_USE_DOUBLE
//#define OPAL_USE_DOUBLE
//#endif	/* OPAL_USE_DOUBLE */

// OPAL includes
#include <opal/opal.h>
#include "graphicsobject.h"

class PhysicsObject : public GraphicsObject
{
	private:
		void initSolid(opal::Simulator* s);
	
	protected:
		opal::Solid* solid;
		opal::ShapeData* shape;
		opal::Matrix44r opalTransform;
		osg::Matrixd* osgTransform;
	
	public:
		PhysicsObject(opal::Simulator* sim, osg::Shape* shape);
		PhysicsObject(opal::Simulator* sim, std::string filename);
		PhysicsObject(opal::Simulator* sim, osg::Geode* geode);
		virtual ~PhysicsObject();
		opal::Solid* getSolid();
		opal::ShapeData* getShapeData();
		void setShapeData(opal::ShapeData* data);
		virtual void update();
		void addForce(opal::Force force);
};

#endif	/* _PHYSICSOBJECT_H */
