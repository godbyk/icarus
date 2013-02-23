// OSG includes
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Array>

// OPAL includes
#include <opal/opal.h>

// my includes
#include "findnodevisitor.h"

// standard includes
#include <iostream>
#include <vector>
 
 
int main(int argc, char* argv[])
{
	std::cout << "Starting..." << std::endl;
	
	
	// get a model from file
	osg::Node* root = NULL;
	root = osgDB::readNodeFile("models/test-world.osg");
	if(root == NULL)
	{	
		std::cout << "Failed to read in model." << std::endl;		
		return -1;
	}
	
	// traverse the model graph and grab the first node of class Geode
	FindNodeVisitor* findNode = new FindNodeVisitor("Geode");
	findNode->setSearchMode(FindNodeVisitor::CLASS_NAME);
	root->accept(*findNode);
	if(findNode->getNumNodesFound() <= 0)
	{
		std::cout << "Didn't find any nodes with the correct name." << std::endl;
		return -1;
	}
	osg::Geode* node = dynamic_cast<osg::Geode*> (findNode->getNode(0));
	
	// now grab the Drawable from the Geode and turn it into Geometry
	osg::Geometry* geom = node->getDrawable(0)->asGeometry();
	if(geom == 0)
	{
		std::cout << "Couldn't use Drawable as Geometry" << std::endl;
		return -1;
	}
	
	// get vertex array and index array
	osg::Array* varr = NULL;
	varr = geom->getVertexArray();
	if(varr == NULL)
	{
		std::cout << "varr is NULL" << std::endl;
		return -1;
	}
	
	// pull the vertex data out of the OSG data structure, messy but works
	// will try to change this to use the visitor patterns later
	unsigned int sz = (unsigned int)varr->getDataSize();	// size of vertex, should be 3
	unsigned int num = (unsigned int)varr->getNumElements();	// number of vertices
	
	const GLfloat* data = (GLfloat*)varr->getDataPointer();
	
	
	
	
	std::cout << "Done." << std::endl;
}
