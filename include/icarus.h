#ifndef _MOODE_H_
#define _MOODE_H_

#include <vrj/vrjConfig.h>

#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>

#include <vrj/Draw/OGL/GlApp.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/DigitalInterface.h>

//OSG  includes
#include <osg/Matrix>
#include <osg/Transform>
#include <osg/MatrixTransform>

#include <osgUtil/SceneView>

#include "nav.h"

#include <vrj/Draw/OSG/OsgApp.h>

#include <opal/opal.h>

#include "terrain.h"
#include "box.h"
#include "user.h"

#ifdef OPAL_USE_DOUBLE
    typedef double real;
#else
    typedef float real;
#endif

/**
 * Demonstration Open Scene Graph application class.
 */
class Icarus : public vrj::OsgApp
{
      public:
	Icarus (vrj::Kernel * kern);

	virtual ~Icarus ()
	{
		/* Do nothing. */ ;
	}

   /**
    * Execute any initialization needed before the API is started.
    *
    * This is called once before OSG is initialized.
    */
	virtual void initScene ();

	void myInit ();

	virtual	osg::Group* getScene ()
	{
		return mRootNode.get ();
	}

	virtual void configSceneView (osgUtil::SceneView * newSceneViewer)
	{
		vrj::OsgApp::configSceneView (newSceneViewer);

		newSceneViewer->getLight ()->
			setAmbient (osg::Vec4 (0.3f, 0.3f, 0.3f, 1.0f));
		newSceneViewer->getLight ()->
			setDiffuse (osg::Vec4 (0.9f, 0.9f, 0.9f, 1.0f));
		newSceneViewer->getLight ()->
			setSpecular (osg::Vec4 (1.0f, 1.0f, 1.0f, 1.0f));
	}

	void bufferPreDraw ();

	// ----- Drawing Loop Functions ------
	//
	//  The drawing loop will look similar to this:
	//
	//  while (drawing)
	//  {
	//        preFrame();
	//       <Application Data Syncronization>
	//        latePreFrame();
	//       draw();
	//        intraFrame();     // Drawing is happening while here
	//       sync();
	//        postFrame();      // Drawing is now done
	//
	//       UpdateTrackers();
	//  }
	//------------------------------------

   /**
    * Function called after tracker update but before start of drawing.
    *
    * Called once before every frame.
    */
	virtual void preFrame ();

   /** Function called after ApplicationData syncronization but before draw() */
	virtual void latePreFrame ();

   /**
    * Function called after drawing has been triggered but BEFORE it
    * completes.
    *
    * Called once during each frame.
    */
	virtual void intraFrame ()
	{
		// Put your intra frame computations here.
	}

   /**
    * Function called before updating trackers but after the frame is drawn.
    *
    * Called once after every frame.
    */
	virtual void postFrame ()
	{
		// Put your post frame computations here.
	}

      	private:
		// graphics variables		  
		osg::ref_ptr < osg::Group > mRootNode;
		osg::ref_ptr < osg::Group > mNoNav;
		osg::ref_ptr < osg::MatrixTransform > mNavTrans;
		osg::ref_ptr < osg::MatrixTransform > mModelTrans;
		osg::ref_ptr < osg::Node > mModel;
	
		OsgNavigator mNavigator;	   /**< Navigation class */
	
	    // time variables
		vpr::Interval oldTime;
		vpr::Interval dt;
		
		// physics variables
		opal::Simulator* physicsSim;
		std::vector <PhysicsObject*> physicsObjectList;
		
		// physical/graphical objects
		Terrain* terrain;
		User* user;
		
		public:
			
		gadget::PositionInterface  head;
};


#endif /* _MOODE_H_ */
