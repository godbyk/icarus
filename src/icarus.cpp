//#ifndef OPAL_USE_DOUBLE
//#define OPAL_USE_DOUBLE
//#endif /* OPAL_USE_DOUBLE */

#include <vrj/vrjConfig.h>

#include <icarus.h>

#include <osg/Math>
#include <osg/Geode>
#include <osg/Material>
#include <osg/Vec3>
#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>

#include <gmtl/Vec.h>
#include <gmtl/Coord.h>
#include <gmtl/Xforms.h>
#include <gmtl/Math.h>

#include <cstdlib>

//for testing
#include <iostream>
#include <fstream>
#include <sstream>

Icarus::Icarus (vrj::Kernel * kern) : vrj::OsgApp (kern)
{
	oldTime = vpr::Interval::now();
}

void Icarus::latePreFrame()
{
	// calculate delta time
	vpr::Interval time = vpr::Interval::now();
	dt = time - oldTime ;
	oldTime = time;
	
	// update user from socket
	user->update(static_cast<opal::real>(dt.secd()));
	
	// update the physics
	physicsSim->simulate(dt.secd());
	for(unsigned int i = 0; i < physicsObjectList.size(); i++)
	{
		physicsObjectList[i]->update();
	}
	
	/*** OLD VIEWER UPDATE ***/
	//gmtl::Matrix44f world_transform;
	//gmtl::invertFull (world_transform, mNavigator.getCurPos ());
	// Update the scene graph
	//osg::Matrix osg_current_matrix;
	//mNavTrans->setMatrix (osg_current_matrix);
	//osg_current_matrix.set (world_transform.getData ());
	
	mNavTrans->setMatrix(osg::Matrixd::inverse(*(user->getTransform())));
}

void Icarus::preFrame ()
{

}

void Icarus::bufferPreDraw ()
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClear (GL_COLOR_BUFFER_BIT);
}

void Icarus::initScene ()
{
	// do this later, it breaks my desktop version
	//head.init("VJHead");
	myInit ();
}

void Icarus::myInit ()
{
	//
	//          /-- mNoNav
	// mRootNode
	//         \-- mNavTrans -- mModelTrans -- mModel

	// The top level nodes of the tree
	mRootNode = new osg::Group();
	mNoNav = new osg::Group();
	mNavTrans = new osg::MatrixTransform();

	mNavigator.init();

	mRootNode->addChild (mNoNav.get());
	mRootNode->addChild (mNavTrans.get());
	
	// set up the physics simulation
	physicsSim = opal::createSimulator();
	opal::Vec3r g(0.0, 0.0, -9.81);
	physicsSim->setGravity(g);

	// set up terrain
	terrain = new Terrain(physicsSim);
	mNavTrans->addChild(terrain->getTerrain()->getRootTransform());
	
	
	// set up boxes
	std::ifstream infile("data/boxes.data");
	while(!infile.eof())
	{
		std::string instr;
		real size, x, y, z;
		std::getline(infile, instr);
		if(instr.empty())
		{
			continue;
		}
		std::stringstream ss(instr);
		ss >> size >> x >> y >> z;
		Box* box = new Box(physicsSim, size, x, y, z);
		box->getBox()->setTexture("models/textures/tron-tile-red.png", osg::TexEnv::DECAL);
		mNavTrans->addChild(box->getBox()->getRootTransform());
		physicsObjectList.push_back(box->getBox());
	}
	
	// set up temp user
	user = new User(physicsSim, "data/user.data");
	
	// run optimization over the scene graph
	osgUtil::Optimizer optimizer;
	optimizer.optimize (mRootNode.get());
}
