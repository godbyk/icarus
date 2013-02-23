#include "user.h"

#include <fstream>
#include <iostream>

#include <vpr/IO/Socket/SocketAcceptor.h>
#include <vpr/IO/Socket/InetAddr.h>

#ifdef OPAL_USE_DOUBLE
    typedef double real;
#else
    typedef float real;
#endif

User::User(opal::Simulator* sim, std::string filename)
{
	startTag = "@";
	endTag = "!";
	
	// get user data from file
	std::ifstream infile(filename.c_str());
	infile >> A >> B >> C >> D >> E;
	isTouching = true;
	
	solid = sim->createSolid();
	solid->setLinearDamping(0.2);
	solid->setAngularDamping(0.5);
	solid->setSleepiness(0);
	opalTransform.translate(0.0, 0.0, 3.0);
	solid->setTransform(opalTransform);
	boundBox.dimensions.set(2, 2, 6);
	boundBox.material.hardness = 0.0;
	boundBox.material.bounciness = 0.0;
	boundBox.material.friction = 0.0;
	boundBox.material.density = 0.8;
	solid->addShape(boundBox);
	
	solid->setUserData(this);
	UserCollisionEventHandler* handler = new UserCollisionEventHandler();
	solid->setCollisionEventHandler(handler);
	
	buoyantForce.type = opal::LOCAL_FORCE;
	forwardForce.type = opal::LOCAL_FORCE;
	turnTorque.type = opal::LOCAL_TORQUE;
	
	time = oldTime = 0.0;
	
	getSocket();
}


osg::Matrix* User::getTransform()
{
	opalTransform = solid->getTransform();
	osg::Matrix* x = new osg::Matrix();
	x->set(opalTransform(0, 0), opalTransform(1, 0), opalTransform(2, 0), opalTransform(3, 0),
		   opalTransform(0, 1), opalTransform(1, 1), opalTransform(2, 1), opalTransform(3, 1),
		   opalTransform(0, 2), opalTransform(1, 2), opalTransform(2, 2), opalTransform(3, 2), 
		   opalTransform(0, 3), opalTransform(1, 3), opalTransform(2, 3), opalTransform(3, 3));
	return x;
}

void User::update(real dt)
{
	if(!socket.isConnected())
	{
		std::cout << "Socket closed, waiting to reopen..." << std::endl;
		getSocket();
	}
	
    vpr::Interval nb;
	// read socket and add input to command string
	socket.read(instr, socket.availableBytes(), nb);
	cmdstr.append(instr);
	// search for the last end tag
	if(!cmdstr.empty())
	{
		unsigned int endIndex = cmdstr.rfind(endTag, cmdstr.size() - 1);
		if(endIndex == std::string::npos)
		{
			return;
		}
		// search for the start tag proceeding the end tag
		unsigned int startIndex = cmdstr.rfind(startTag, endIndex);
		if(startIndex == std::string::npos)
		{
			return;
		}
		// pull out the substring and put it in a stream
		ss.str(cmdstr.substr(startIndex + 1, endIndex - startIndex + 1));
		// read out the variables and clear the string
		real l, r;
		ss >> time >> l >> r;
		cmdstr.erase();
		time /= 1000000.0;
		real dl = l - Yl;
		real dr = r - Yr;
		real dtt = time - oldTime;
		YlDot = dl / dtt;
		if(YlDot > 0.0)
		{
			YlDot = 0.0;
		}
		YrDot = dr / dtt;
		if(YrDot > 0.0)
		{
			YrDot = 0.0;
		}
		oldTime = time;
		Yl = l;
		Yr = r;
		
		//std::cout << "params: " << time << ", " << Yl << ", " << Yr << ", " << YlDot << ", " << YrDot << std::endl;
		//solid->zeroForces();
		opal::Vec3r linVel(0.0, 0.0, 0.0);
		if(!isTouching)
		{
			// force-based 
			/*
			turnTorque.vec = opal::Vec3r(0.0, 0.0, D * (Yl - Yr));
			turnTorque.duration = dt;
			solid->addForce(turnTorque);
			forwardForce.vec = opal::Vec3r(E * (Yl - Yr), 50.0 + C * (YlDot + YrDot), 0.0);
			forwardForce.duration = dt;
			solid->addForce(forwardForce);
			*/
			
			// velocity-based
			solid->setLocalAngularVel(opal::Vec3r(0.0, 0.0, D * (Yl - Yr)));
			linVel += opal::Vec3r(0.0, C, 0.0);
		}
		else
		{
			setIsTouching(false);
		}
		//std::cout << A * Yl << std::endl;
		// force-based
		/*
		buoyantForce.vec = opal::Vec3r(0.0, 0.0, A * (fabs(Yl) + fabs(Yr)) + B * (YlDot + YrDot));
		buoyantForce.duration = dt;
		solid->addForce(buoyantForce);
		*/
		
		// velocity-based
		linVel += opal::Vec3r(0.0, 0.0, A * (fabs(Yl) + fabs(Yr)) + B * (YlDot + YrDot));
		solid->setLocalLinearVel(linVel);
		
		// ack that we recvd
		socket.write("OK", 2, nb);
	}
}

void User::getSocket()
{
	vpr::InetAddr* addr = new vpr::InetAddr();
	addr->setAddress("0.0.0.0:2129");
	vpr::SocketAcceptor sockAcc(*addr);
	std::cout << "Waiting for a socket connection..." << std::endl;
	socket.setBlocking(false);
	sockAcc.accept(socket);
}

void User::setIsTouching(bool t)
{
	isTouching = t;
}
