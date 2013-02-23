#include "findnodevisitor.h"

#include <iostream>
// Default constructor - initialize searchForName to "" and 
// set the traversal mode to TRAVERSE_ALL_CHILDREN
FindNodeVisitor::FindNodeVisitor() : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), 
                                   	searchForName(),
									mode(NAME)
{ 
} 

// Constructor that accepts string argument
// Initializes searchForName to user string
// set the traversal mode to TRAVERSE_ALL_CHILDREN
FindNodeVisitor::FindNodeVisitor(const std::string &searchName) : 
                                   	osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), 
                                   	searchForName(searchName),
									mode(NAME)
{ 
} 

// The 'apply' method for 'node' type instances.
// Compare the 'searchForName' data member against the node's name.
// If the strings match, add this node to our list
void FindNodeVisitor::apply(osg::Node &searchNode) 
{ 
	std::string name;
	switch (mode)
	{
		case NAME:
			name = searchNode.getName();
			break;
		case CLASS_NAME:
			name = searchNode.className();
			break;
		case LIB_NAME:
			name = searchNode.libraryName();
			break;
	}
   if (name == searchForName)
   {
      foundNodeList.push_back(&searchNode);
   }
   traverse(searchNode); 
} 

// Set the searchForName to user-defined string
void FindNodeVisitor::setNameToFind(const std::string &searchName) 
{ 
   searchForName = searchName; 
   foundNodeList.clear(); 
} 


int FindNodeVisitor::getNumNodesFound()
{
	return foundNodeList.size();
}
   
osg::Node* FindNodeVisitor::getNode(unsigned int i)
{
	if(i >= 0 && i < foundNodeList.size())
	{
		return foundNodeList[i];
	}
	return NULL;
}

void FindNodeVisitor::setSearchMode(searchmode m)
{
	mode = m;
}
