#ifndef _GRAPHICSOBJECT_H
#define _GRAPHICSOBJECT_H

// OSG includes
#include <osg/MatrixTransform>
#include <osg/Shape>
#include <osg/Texture2D>
#include <osg/TexEnv>

// standard includes
#include <string>

class GraphicsObject : public osg::Referenced
{
	protected:
		osg::ref_ptr<osg::MatrixTransform> rootTransform;
		osg::ref_ptr<osg::Texture2D> texture;
	
	public:
		GraphicsObject(osg::Shape* shape);
		GraphicsObject(const std::string& filename);
		GraphicsObject(osg::Geode* geode);
		osg::MatrixTransform* getRootTransform();
		void setTexture(const std::string textureFileName, const osg::TexEnv::Mode mode);
};

#endif	/*_GRAPHICSOBJECT_H*/
