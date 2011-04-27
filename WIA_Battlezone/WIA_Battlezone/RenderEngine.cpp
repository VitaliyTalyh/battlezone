/*	This file is part of WIA Battlezone.
 *
 *	WIA Battlezone is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	 WIA Battlezone is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with WIA Battlezone.  If not, see <http://www.gnu.org/licenses/>.
 */

/**	
 *	@file RenderEngine.cpp
 *	@headerfile RenderEngine.h "RenderEngine.h"
 *	@author Ben Hubler
 *	@date 4/19/2011
 *	@version 1.0.1
 */
#include <Windows.h>
#include "RenderEngine.h"
#include "xmlParser\tinystr.h"
#include "xmlParser\tinyxml.h"
#include "point3d.h"

const float PI=3.14159265;
const float	RAD_0DEG = 0.0000000;
const float RAD_90DEG = 1.5707963;
const float RAD_180DEG = 3.1415927;
const float RAD_270DEG = 4.7123890;
const float RAD_360DEG = 6.2831853;

/**
 *	Default destructor
 */
RenderEngine::~RenderEngine()
{
	delete window;
	//add code to properly delete renderobjects vector of pointers
}

/**
 *	Default construction
 *	sets currentx, z, and angles to 0 and raises view elevation to 6
 */
RenderEngine::RenderEngine()
{
	int _argc=0;
	char* _argv[5];
	currentx = 0.0;
	currentz = 0.0;
	currangle.setXY(1,0); 
	window = new CS325Graphics(_argc, _argv);
	window->SetViewDirection(Vector2D(1,0));
	window->SetViewPosition(Point2D(currentx, currentz));
	window->setViewElevation(-6.0);
}

/**
 *	Function used to populate the renderobjects vector with an xml file
 *	@param _renderobjects is a pointer to a vector of RenderObject used to store directions on how to draw objects
 *	@param _rederobjectfile is a string representing the location of the xml file with direction on how to draw objects
 */
void RenderEngine::initRenderObjects(vector<RenderObject>* _renderobjects, string _renderobjectfile)
{
	TiXmlElement *rootxml;
	TiXmlElement *objectxml;
	TiXmlElement *linexml;
	TiXmlElement *pointxml;
	int *objectid = new int();
	int *x = new int();
	int *y = new int();
	int *z = new int();
	double *maxdist = new double();

	TiXmlDocument renderobjectsxml(_renderobjectfile.c_str());
	renderobjectsxml.LoadFile();
	rootxml = renderobjectsxml.RootElement();
	objectxml = rootxml->FirstChildElement("object");
	_renderobjects->clear();

	while(objectxml != 0)
	{
		objectxml->Attribute("id", objectid);
		objectxml->Attribute("maxdist", maxdist);
		RenderObject *renderobject = new RenderObject(*objectid, *maxdist);
		linexml = objectxml->FirstChildElement("line");
		while(linexml != 0)
		{
			pointxml = linexml->FirstChildElement("point");
			pointxml->Attribute("x",x);
			pointxml->Attribute("y",y);
			pointxml->Attribute("z",z);
			renderobject->addPoint(Point(float(*x),float(*y),float(*z)));
			pointxml = pointxml->NextSiblingElement();
			pointxml->Attribute("x",x);
			pointxml->Attribute("y",y);
			pointxml->Attribute("z",z);
			renderobject->addPoint(Point(float(*x),float(*y),float(*z)));
			linexml = linexml->NextSiblingElement("line");
		}
		_renderobjects->push_back(*renderobject);
		objectxml = objectxml->NextSiblingElement("object");
	}
	delete objectid;
	delete x;
	delete y;
	delete z;
}

/**
 *	Function used to draw all objects in a vector of object pointers that are related to renderobjects
 *	@param _objects is a vector of Object pointers that contains a list of all objects to draw
 *	@param _renderobjects is a vector of RenderObject that contain the direction of how to draw each object
 */
void RenderEngine::drawobjects(vector<Object*>* _objects, vector<RenderObject> _renderobjects)
{
	vector<RenderObject>::iterator renderobjectsiter;
	vector<RenderObject>::iterator renderobjectsend;

	list<Point>::iterator piter;
	int objectID;
	Point3D point1;
	Point3D point2;
	Pose position;
	for(int i = 0;i < _objects->size(); i++)
	{
		objectID = (*_objects)[i]->getObjectID();
		position = (*_objects)[i]->getPose();
		renderobjectsiter = _renderobjects.begin();
		renderobjectsend = _renderobjects.end();
		while(renderobjectsiter != renderobjectsend)
		{
			if(renderobjectsiter->getObjectID() == objectID)
			{
				piter = renderobjectsiter->getPointsBegin();
				while(piter != renderobjectsiter->getPointsEnd())
				{
					point1.setXYZ(piter->getX() + position.getX(), 
						piter->getY() - position.getY(),
						piter->getZ() - position.getZ());
					piter++;
					point2.setXYZ(piter->getX() + position.getX(), 
						piter->getY() - position.getY(),
						piter->getZ() - position.getZ());
					window->DrawLineInSpace(point1,point2);
					piter++;
				}
			}
			renderobjectsiter++;
			window->DrawLineOnScreen(Point2D(-1,0),Point2D(1,0));
			window->DrawLineOnScreen(Point2D(-.001,0),Point2D(.001,0));
			window->DrawLineOnScreen(Point2D(0.00,-.001),Point2D(0.00,.001));
		}
	}
}

/**
 *	Function used to set the rotation of the view
 *	@param _degrees is a float that represents the change in degress the view will rotate
 */
void RenderEngine::rotate(float _degrees)
{
	float radians = _degrees * PI / 180;
	float angle;
	angle = currangle.getAngle();
	angle = angle + radians;
	// The following conversions is to aid in the move function
	// making it easy to detect the correct quadrant
	// If angle is greater than 360, convert to same angle less than 360
	while(angle >= RAD_360DEG)
	{
		angle = angle - RAD_360DEG;
	}
	// If angle is negatave, convert to same positive angle
	while(angle <  0)
	{
		angle = angle + RAD_360DEG;
	}
	currangle.setAngle(angle);
	window->SetViewDirection(currangle);
}

/**
 *	Function used to display the currently drawn window
 */
void RenderEngine::draw()
{
	window->DisplayNow();
}

/**
 *	Function used to move the view
 *	@param _speed is a float representing the distance the view will move during this cycle
 */
void RenderEngine::move(float _speed)
{
	double radian = double(currangle.getAngle());
	//Determine which quadrant the move is in and perform the calcualtion to find
	//new x and z coordinates
	if(radian <= RAD_90DEG)
	{
		currentz = currentz - (_speed * sin(radian));
		currentx = currentx + (_speed * cos(radian));
	}
	else if(radian <= RAD_180DEG)
	{
		currentz = currentz - (_speed * sin(RAD_180DEG - radian));
		currentx = currentx - (_speed * cos(RAD_180DEG - radian));
	}
	else if(radian <= RAD_270DEG)
	{
		currentz = currentz + (_speed * sin(radian - RAD_180DEG));
		currentx = currentx - (_speed * cos(radian - RAD_180DEG));
	}
	else if(radian <= RAD_360DEG)
	{
		currentz = currentz + (_speed * sin(RAD_360DEG - radian));
		currentx = currentx + (_speed * cos(RAD_360DEG - radian));
	}
	window->SetViewPosition(Point2D(currentz, currentx));
}

/**
 *	a function used to display curretn X, Y, Z, and Theta values used for troubleshooting
 */
void RenderEngine::displayXYZTheta()
{
	cout << "X: " << currentx << "  Z: " << currentz << "  Theta: " << currangle.getAngle() *180 / PI << endl;
}