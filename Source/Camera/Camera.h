#ifndef _CAMERA_H
#define _CAMERA_H

// This was created to allow us to use this camera code in other projects
// without having to cut and paste code.  This file and camera.cpp could be
// added to a future project.
#include "../JuceLibraryCode/JuceHeader.h"
#include "../Math/Vectors.h"
#include "../Primitives/Triangle.h"
#include "../Primitives/Sphere.h"

#if JUCE_WINDOWS
#pragma comment(lib, "glu32.lib")
#include <Windows.h>
#include <gl/glu.h>
#endif

#if JUCE_MAC
#include <GLUT/glut.h>
#endif


// This is our camera class
class FPSCamera {

public:

	// Our camera constructor
	FPSCamera();	


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// These are are data access functions for our camera's private data
	Vec3f getPosition() {	return position;		}
	Vec3f getView()     {	return view;			}
	Vec3f getUpVector() {	return upVector;		}
	Vec3f getStrafe()	{	return strafeVector;	}
    Sphere getCollisonSphere()  {return sphere;}
    float  getXrotation(){return rotationX;}
    float  getYrotation(){return rotationY;}
    

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	
	// This changes the position, view, and up vector of the camera.
	// This is primarily used for initialization
	void setPosition(float positionX, float positionY, float positionZ,
			 		    float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ);

	// This rotates the camera's view around the position depending on the values passed in.
	void rotateView(float angle, float X, float Y, float Z);

	// This moves the camera's view by the mouse movements (First person view)
	void setViewByMouse(float mouseX, float mouseY, int width, int height);

	// This rotates the camera around a point (I.E. your character).
	void rotateAroundPoint(Vec3f vCenter, float X, float Y, float Z);
    
    void setXrotation(float rotation)
    {
        rotationX=rotation;
    }


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// This strafes the camera left or right depending on the speed (+/-) 
	void strafe(float speed);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


	// This will move the camera forward or backward depending on the speed
	void move(float speed);

	// This checks for keyboard movement
	void checkForMovement();


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// This updates the camera's view and other data (Should be called each frame)
	void update(float mouseX, float mouseY, int width, int height);

	// This uses gluLookAt() to tell OpenGL where to look
	void look();
    void checkCameraCollision(Triangle *roomTriangles, int numTriangles);
    Vec3f getCollisionOffset(Vec3f &vNormal, float radius, float distance);

    //Camera sphere for wall collision detection
    Sphere sphere;
private:

	// The camera's position
	Vec3f position;

	// The camera's view
	Vec3f view;						

	// The camera's up vector
	Vec3f upVector;		
	
	// The camera's strafe vector
	Vec3f strafeVector;
    
    
    //Camera rotation
    float rotationX=0; //Rotation using X as rotation axis
    float rotationY=0; //Rotation using Y as rotation axis


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

};


#endif


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// This file contains the prototype for our camera class.  
//
// We added a few more functions to our FPSCamera class, along with
// some measly data access functions for our now private member variables.
//
// 
// �2000-2005 GameTutorials
