/***************************************************************************
 *            blender2osgstripper.cpp
 *
 *  Sat Mar 18 00:48:53 2006
 *  Copyright  2006  Jesse Lane
 *  jesse.a.lane@gmail.com
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Copyright  2006  Jesse Lane
 *  jesse.a.lane@gmail.com
 ****************************************************************************/
 
 #include <osg/Node>
 #include <osgDB/ReadFile>
 #include <osgDB/WriteFile>
 #include "findnodevisitor.h"
 #include <iostream>
 
 
int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		std::cout << "You must specify osg files to strip." << std::endl;
	}
	
	for(int i = 1; i < argc; i++)
	{
		osg::Group* root = NULL;
		root = osgDB::readNodeFile(argv[i])->asGroup();
		if(root != NULL)
		{			
			FindNodeVisitor* findNode = new FindNodeVisitor("frame1");
			root->accept(*findNode);
			if(findNode->getNumNodesFound() > 0)
			{
				osg::Node* node = findNode->getNode(0);
				node->setName("group1");
				osgDB::writeNodeFile(*node, argv[i]);
			}
			else
			{
				std::cout << "Didn't find any nodes with the correct name." << std::endl;
			}
		}
	}
}
