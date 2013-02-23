#include "graphicsobject.h"
#include <osgDB/ReadFile>
#include <osg/Node>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/StateAttribute>

#include <iostream>

GraphicsObject::GraphicsObject(osg::Shape* shape) : osg::Referenced()
{
	rootTransform = new osg::MatrixTransform();
	osg::Geode* geo = new osg::Geode();
	osg::ShapeDrawable* sd = new osg::ShapeDrawable(shape);
	geo->addDrawable(sd);
	rootTransform->addChild(dynamic_cast <osg::Node*>(geo));
	texture = new osg::Texture2D();
}

GraphicsObject::GraphicsObject(const std::string& filename)
{
	rootTransform = new osg::MatrixTransform();
	osg::Node* node = osgDB::readNodeFile(filename);
	rootTransform->addChild(node);
	texture = new osg::Texture2D();
}

GraphicsObject::GraphicsObject(osg::Geode* geode)
{
	rootTransform = new osg::MatrixTransform();
	rootTransform->addChild(geode);
	texture = new osg::Texture2D();
}

osg::MatrixTransform* GraphicsObject::getRootTransform()
{
	return rootTransform.get();
}

void GraphicsObject::setTexture(const std::string textureFileName, const osg::TexEnv::Mode mode)
{
	// protect from being optimized away as static state:
	texture->setDataVariance(osg::Object::DYNAMIC); 
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);	
	// load an image by reading a file: 
	osg::Image* texImage = osgDB::readImageFile(textureFileName);
	if(!texImage)
	{
		std::cout << "Could not load texture image." << std::endl;
		return;
	}
	// Assign the texture to the image we read from file: 
	texture->setImage(texImage);
	// Create a new StateSet with default settings: 
	osg::StateSet* state = new osg::StateSet();
	// Assign texture unit 0 of our new StateSet to the texture 
	// we just created and enable the texture.
	osg::TexEnv* texenv = new osg::TexEnv();
	texenv->setMode(mode);
	state->setTextureAttributeAndModes(0, texenv, osg::StateAttribute::ON); 
	state->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON); 
	rootTransform->setStateSet(state);
}
