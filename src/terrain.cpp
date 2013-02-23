/***************************************************************************
 *            terrain.cpp
 *
 *  Tue Feb 28 11:49:09 2006
 *  Copyright  2006  User
 *  Email
 ****************************************************************************/
#include "terrain.h"
#include <osg/Geometry>
#include <osg/Geode>

#include <iostream>

Terrain::Terrain(opal::Simulator* sim)
{
	osg::Vec3 size(2000.0, 2000.0, 2000.0);
	double tileSize = 100;
	// make ground plane
	osg::Geometry* geom = new osg::Geometry();
	// set up vertices for ground plane quad
	osg::Vec3Array* vertices = new osg::Vec3Array();
	vertices->push_back(osg::Vec3(1.0, 1.0, 0.0));
	vertices->push_back(osg::Vec3(-1.0, 1.0, 0.0));
	vertices->push_back(osg::Vec3(-1.0, -1.0, 0.0));
	vertices->push_back(osg::Vec3(1.0, -1.0, 0.0));
	// set up base color
	osg::Vec4Array* colors = new osg::Vec4Array();
	colors->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	// set up texture coordinates
	osg::Vec2Array* texCoords = new osg::Vec2Array();
	texCoords->push_back(osg::Vec2(size.x() / tileSize, size.y() / tileSize));
	texCoords->push_back(osg::Vec2(0.0, size.y() / tileSize));
	texCoords->push_back(osg::Vec2(0.0, 0.0));
	texCoords->push_back(osg::Vec2(size.x() / tileSize, 0.0));
	// set geom properties
	geom->setVertexArray(vertices);
	geom->setColorArray(colors);
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);
	geom->setTexCoordArray(0, texCoords);
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, vertices->size()));
	// add geometry to a Geode
	osg::Geode* geode = new osg::Geode();
	geode->addDrawable(geom);
	ground = new PhysicsObject(sim, geode);
	// set ground solid to static so it doesn't move but does collide
	ground->getSolid()->setStatic(true);
	// set up shape data
	opal::PlaneShapeData planeData;
	// parameters for plane in plane equation ax+by+cz=d 
	planeData.abcd[0] = 0;
	planeData.abcd[1] = 0;
	planeData.abcd[2] = 1;
	planeData.abcd[3] = 0;
	// set up plane material
	planeData.material.hardness = 0.0;
	planeData.material.bounciness = 0.0;
	planeData.material.friction = 1.0;
	planeData.material.density = 0.5;
	// set the shape data
	ground->setShapeData(&planeData);
	// scale the plane geometry, we don't have to do anything to the opal shape
	// because it represents an infinite plane, so any size is covered
	osg::Matrixd gpMat = ground->getRootTransform()->getMatrix();
	gpMat.makeScale(size);
	ground->getRootTransform()->setMatrix(gpMat);
	// set the texture
	ground->setTexture("models/textures/tron-tile-blue.png", osg::TexEnv::DECAL);
}

PhysicsObject* Terrain::getTerrain()
{
	return ground;
}
