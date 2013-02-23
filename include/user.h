#ifndef _USER_H
#define _USER_H

//#ifndef OPAL_USE_DOUBLE
//#define OPAL_USE_DOUBLE
//#endif	/* OPAL_USE_DOUBLE */

#include <opal/opal.h>
#include <osg/Matrix>
#include <vpr/IO/Socket/SocketStream.h>

#include <string>
#include <sstream>

class User
{
	private:
		double A;
		double B;
		double C;
		double D;
		double E;
		double Yr;
		double Yl;
		double YrDot;
		double YlDot;
		double oldTime;
		double time;
		bool isTouching;
		opal::Solid* solid;
		opal::BoxShapeData boundBox;
		opal::Matrix44r opalTransform;
		opal::Force buoyantForce;
		opal::Force forwardForce;
		opal::Force turnTorque;
	
		vpr::SocketStream socket;
		std::string instr;
		std::string cmdstr;
		std::stringstream ss;
	
		std::string startTag;
		std::string endTag;
	
		void getSocket();
	
	public:
		User(opal::Simulator* sim, std::string filename);
		osg::Matrix* getTransform();
		void update(opal::real dt);
		void setIsTouching(bool t);
};

class UserCollisionEventHandler : public opal::CollisionEventHandler
{
	public:
        virtual void OPAL_CALL handleCollisionEvent(const opal::CollisionEvent& e)
        {
			User* user = (User*)e.thisSolid->getUserData();
			user->setIsTouching(true);
		}
};

#endif /*_USER_H*/
