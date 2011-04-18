//
/* file: CS325GraphicsDriver.cpp
/* author: djbouvier
/* date: 12 April 2011 
 */

#include <iostream>
#include <windows.h>
#include "vector3D.h"
#include "cs325graphics.h"
#include "Pose.h"
#include "Object.h"
#include "RenderType.h"
#include <list>
#include "Tank.h"

#define SCREEN_TEST 100
#define ROTATE_TEST 100
#define MOVE_TEST 100
using namespace std;

int main(int argc, char* argv[])
{
	int temp;
	Tank test;
	RenderType testrt(001);
	
	for (int i=0;i<10;i++)
	{
		testrt.addPoint(Point(float(i),float(i),float(i)));

	}

	list<Point>::iterator pointIter = testrt.getPointsBegin();
	while(pointIter != testrt.getPointsEnd())
	{
		cout << "X: " << pointIter->getX() << ", Y: " 
			 << pointIter->getY() << ", Z: " 
			 << pointIter->getZ() << endl;
		pointIter++;
	}
	cout << endl;
	test.setCoordinate(3.12,4.12,5.0);
	test.setHealth(10);
	test.setMovable(true);
	test.setOrientation(45);
	
	while(!test.isDestroyed())
	{
		
		cout << "Firing..." << endl;
		test.shotFired();
		cout << "test health: " << test.getHealth() << endl;
		cout << "test position: " << test.getPose().getX() << ", " << test.getPose().getY() << ", " << test.getPose().getZ() << endl;
		cout << "test hit..." << endl;
		test.setHealth(test.getHealth() - 1);
		while(!test.canFire())
		{
			Sleep(1000);
		}
	}
	cout << endl;
	cout << "Type any number and press enter to begin graphics test" << endl;
	cin >> temp;

	CS325Graphics window(argc, argv);

	float delta = 0.1;

	Point2D p1(CS325Graphics::X_MIN, CS325Graphics::Y_MAX);
	Point2D p2(CS325Graphics::X_MAX, CS325Graphics::Y_MIN);
	Point2D p3(CS325Graphics::X_MIN, CS325Graphics::Y_MIN);
	Point2D p4(CS325Graphics::X_MAX, CS325Graphics::Y_MAX);

	Point3D p30(0.5,  0.5, 0.5);
	Point3D p31(0.5, -0.5, 0.5);
	Point3D p32(-0.5,-0.5, 0.5);
	Point3D p33(-0.5, 0.5, 0.5);
	Point3D p34(0.5,  0.5, -0.5);
	Point3D p35(0.5, -0.5, -0.5);
	Point3D p36(-0.5,-0.5, -0.5);
	Point3D p37(-0.5, 0.5, -0.5);

	for(int i = 0; i < SCREEN_TEST; i++){

		cout << "draw lines" << endl;
		window.DrawLineOnScreen(p1, p2);
		window.DrawLineOnScreen(p4, p3);

		window.DrawLineInSpace(p30, p31);
		window.DrawLineInSpace(p31, p32);
		window.DrawLineInSpace(p32, p33);
		window.DrawLineInSpace(p33, p30);
		window.DrawLineInSpace(p34, p35);
		window.DrawLineInSpace(p35, p36);
		window.DrawLineInSpace(p36, p37);
		window.DrawLineInSpace(p37, p34);
		window.DrawLineInSpace(p30, p34);
		window.DrawLineInSpace(p31, p35);
		window.DrawLineInSpace(p32, p36);
		window.DrawLineInSpace(p33, p37);


		p1.setX(p1.getX() + delta);
		if(p1.getX() > CS325Graphics::X_MAX) delta *=-1;
		if(p1.getX() < CS325Graphics::X_MIN) delta *=-1;

		window.DisplayNow();
		Sleep(50);
		
	}

	Point2D viewPos;
	Vector2D viewDir;
	Vector3D deltaV;
	viewDir.setAngle(0);


	// move view position

	for(int i = 0; i < MOVE_TEST; i++){
		window.DrawLineOnScreen(p1, p2);
		window.DrawLineOnScreen(p4, p3);

		window.DrawLineInSpace(p30, p31);
		window.DrawLineInSpace(p31, p32);
		window.DrawLineInSpace(p32, p33);
		window.DrawLineInSpace(p33, p30);
		window.DrawLineInSpace(p34, p35);
		window.DrawLineInSpace(p35, p36);
		window.DrawLineInSpace(p36, p37);
		window.DrawLineInSpace(p37, p34);
		window.DrawLineInSpace(p30, p34);
		window.DrawLineInSpace(p31, p35);
		window.DrawLineInSpace(p32, p36);
		window.DrawLineInSpace(p33, p37);

		delta = -.1;
		viewPos.setY(viewPos.getY() + delta);
		window.SetViewPosition(viewPos);
		cout << "view pos: " << viewPos.toString() << endl;

		window.DisplayNow();
		Sleep(50);

	}

	// rotate view position

	for(int i = 0; i < ROTATE_TEST; i++){
		window.DrawLineOnScreen(p1, p2);
		window.DrawLineOnScreen(p4, p3);

		window.DrawLineInSpace(p30, p31);
		window.DrawLineInSpace(p31, p32);
		window.DrawLineInSpace(p32, p33);
		window.DrawLineInSpace(p33, p30);
		window.DrawLineInSpace(p34, p35);
		window.DrawLineInSpace(p35, p36);
		window.DrawLineInSpace(p36, p37);
		window.DrawLineInSpace(p37, p34);
		window.DrawLineInSpace(p30, p34);
		window.DrawLineInSpace(p31, p35);
		window.DrawLineInSpace(p32, p36);
		window.DrawLineInSpace(p33, p37);

		delta = .1;

		viewDir.setAngle(viewDir.getAngle()+delta);
		window.SetViewDirection(viewDir);
		cout << "view dir: " << viewDir.getAngle() << endl;

		window.DisplayNow();
		Sleep(50);		
	}
	return (0);
}

