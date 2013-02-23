#include "physicsobject.h"

#include <iostream>

PhysicsObject::PhysicsObject(opal::Simulator* sim, osg::Shape* shape) : GraphicsObject(shape)
{
	initSolid(sim);
}


PhysicsObject::PhysicsObject(opal::Simulator* sim, std::string filename) : GraphicsObject(filename)
{
	initSolid(sim);
}

PhysicsObject::PhysicsObject(opal::Simulator* sim, osg::Geode* geode) : GraphicsObject(geode)
{
	initSolid(sim);
}

PhysicsObject::~PhysicsObject()
{
}

opal::Solid* PhysicsObject::getSolid()
{
	return solid;
}


opal::ShapeData* PhysicsObject::getShapeData()
{
	return shape;
}


void PhysicsObject::setShapeData(opal::ShapeData* data)
{
	shape = data;
	solid->clearShapes();
	solid->addShape(*shape);
}


void PhysicsObject::update()
{
	opalTransform = solid->getTransform();
	osgTransform->set(opalTransform(0, 0), opalTransform(1, 0), opalTransform(2, 0), opalTransform(3, 0),
					  opalTransform(0, 1), opalTransform(1, 1), opalTransform(2, 1), opalTransform(3, 1),
					  opalTransform(0, 2), opalTransform(1, 2), opalTransform(2, 2), opalTransform(3, 2), 
					  opalTransform(0, 3), opalTransform(1, 3), opalTransform(2, 3), opalTransform(3, 3));
	rootTransform.get()->setMatrix(*osgTransform);
}

void PhysicsObject::addForce(opal::Force force)
{
	solid->addForce(force);
}


void PhysicsObject::initSolid(opal::Simulator* s)
{
	solid = s->createSolid();
	solid->setLinearDamping(0.0);
	solid->setSleepiness(0);
	solid->setTransform(opalTransform);
	shape = NULL;
	osgTransform = new osg::Matrixd();

}
