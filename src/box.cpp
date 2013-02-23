#include "box.h"

Box::Box(opal::Simulator* sim, double size, double x, double y, double z)
{
	osg::Vec3 origin(0.0, 0.0, 0.0);
	box = new PhysicsObject(sim, new osg::Box(origin, size));
	opal::Matrix44r boxMat = box->getSolid()->getTransform();
	boxMat.translate(x, y, z);
	box->getSolid()->setTransform(boxMat);
	opal::BoxShapeData boxData;
	boxData.dimensions.set(size, size, size);
	boxData.material.hardness = 1.0;
	boxData.material.bounciness = 0.0;
	boxData.material.friction = 1.0;
	boxData.material.density = 0.01;
	box->setShapeData(&boxData);
}


PhysicsObject* Box::getBox()
{
	return box;
}
