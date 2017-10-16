//***********************************************************************//
//																		 //
//		- "Talk to me like I'm a 3 year old!" Programming Lessons -		 //
//                                                                       //
//		$Author:		DigiBen		DigiBen@GameTutorials.com			 //
//																		 //
//		$Program:		Camera5	(strafing)								 //
//																		 //
//		$Description:	Demonstrates camera strafing right and left		 //


#include "Camera.h"
#include <math.h>


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// This is how fast our camera moves (Sped up a bit due to normalizing our vectors)
#define kSpeed	0.1f

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


///////////////////////////////// FPSCamera \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This is the class constructor
/////
///////////////////////////////// FPSCamera \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

FPSCamera::FPSCamera()
{

	position	 = Vec3f(0.0, 0.0, 0.0);					// Init the position to zero
	view		 = Vec3f(0.0, 1.0, 0.5);					// Init the view to a std starting view
	upVector	 = Vec3f(0.0, 0.0, 1.0);						// Init the UpVector
	strafeVector = Vec3f(1.0, 0.0, 0.0);
    
    sphere.center=position;
    sphere.radius=1.2;
}


///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function sets the camera's position and view and up viewVector.
/////
///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void FPSCamera::setPosition(float positionX, float positionY, float positionZ,
				  		     float viewX,     float viewY,     float viewZ,
							 float upVectorX, float upVectorY, float upVectorZ)
{
	// The code above just makes it cleaner to set the variables.
	// Otherwise we would have to set each variable x y and z.

	position = Vec3f(positionX, positionY, positionZ);	// Assign the position
	view     = Vec3f(viewX, viewY, viewZ);				// Assign the view
	upVector = Vec3f(upVectorX, upVectorY, upVectorZ);	// Assign the up vector
    
    sphere.center=position;
}


///////////////////////////////// SET VIEW BY MOUSE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This allows us to look around using the mouse, like in most first person games.
/////
///////////////////////////////// SET VIEW BY MOUSE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void FPSCamera::setViewByMouse(float mouseX, float mouseY, int width, int height)
{
	//POINT mousePos;					// This is a window structure that holds an X and Y
	int   middleX = width /2;			// This is a binary shift to get half the width
	int   middleY = height /2;			// This is a binary shift to get half the height
	float angleY = 0.0f;				// This is the direction for looking up or down
	float angleZ = 0.0f;				// This will be the value we need to rotate around the Y axis (Left and Right)
	static float currentRotX = 0.0f;
				
	
	// If our cursor is still in the middle, we never moved... so don't update the screen
	if( (mouseX == middleX) && (mouseY == middleY) ) return;

	// Set the mouse position to the middle of our window
    
    Point<float> newPosition(middleX, middleY);
    Desktop::getInstance().getMouseSource (0)->setScreenPosition(newPosition);
    
	// Get the direction the mouse moved in, but bring the number down to a reasonable amount
	angleY = (float)( (middleX - mouseX) ) / 1000.0f;		
	angleZ = (float)( (middleY - mouseY) ) / 1000.0f;		
	
	
    
	rotationX += angleY;
	
		

    static float lastRotX = 0.0f;
 	lastRotX = currentRotX; // We store off the currentRotX and will use it in when the angle is capped
	
	// Here we keep track of the current rotation (for up and down) so that
	// we can restrict the camera from doing a full 360 loop.
	currentRotX += angleZ;
 
	// If the current rotation (in radians) is greater than 1.0, we want to cap it.
	if(currentRotX > 1.0f)     
	{
		currentRotX = 1.0f;
		
		// Rotate by remaining angle if there is any
		if(lastRotX != 1.0f) 
		{
			// To find the axis we need to rotate around for up and down
			// movements, we need to get a perpendicular vector from the
			// camera's view vector and up vector.  This will be the axis.
			// Before using the axis, it's a good idea to normalize it first.
			Vec3f axis = (view - position).cross(upVector);
			axis = axis.normalize();
				
			// rotate the camera by the remaining angle (1.0f - lastRotX)
			rotateView( 1.0f - lastRotX, axis.x, axis.y, axis.z);
			
		}
	}
	// Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
	else if(currentRotX < -1.0f)
	{
		currentRotX = -1.0f;
		
		// Rotate by the remaining angle if there is any
		if(lastRotX != -1.0f)
		{
			// To find the axis we need to rotate around for up and down
			// movements, we need to get a perpendicular vector from the
			// camera's view vector and up vector.  This will be the axis.
			// Before using the axis, it's a good idea to normalize it first.
			Vec3f axis = (view - position).cross(upVector);
			axis = axis.normalize();
			
			// rotate the camera by ( -1.0f - lastRotX)
			rotateView( -1.0f - lastRotX, axis.x, axis.y, axis.z);
			
		}
	}
	// Otherwise, we can rotate the view around our position
	else 
	{	
		// To find the axis we need to rotate around for up and down
		// movements, we need to get a perpendicular vector from the
		// camera's view vector and up vector.  This will be the axis.
		// Before using the axis, it's a good idea to normalize it first.
		Vec3f axis = (view - position).cross(upVector);
		axis = axis.normalize();
	
		// Rotate around our perpendicular axis
		rotateView(angleZ, axis.x, axis.y, axis.z);
		rotationY += angleZ;
	}

	// Always rotate the camera around the y-axis
	rotateView(angleY, 0, 1, 0);
}


///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This rotates the view around the position using an axis-angle rotation
/////
///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void FPSCamera::rotateView(float angle, float x, float y, float z)
{
	Vec3f newView;

	// Get the view vector (The direction we are facing)
	Vec3f viewVector = view - position;		

	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Find the new x position for the new rotated point
	newView.x  = (cosTheta + (1 - cosTheta) * x * x)		* viewVector.x;
	newView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* viewVector.y;
	newView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* viewVector.z;

	// Find the new y position for the new rotated point
	newView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* viewVector.x;
	newView.y += (cosTheta + (1 - cosTheta) * y * y)		* viewVector.y;
	newView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* viewVector.z;

	// Find the new z position for the new rotated point
	newView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* viewVector.x;
	newView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* viewVector.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z)		* viewVector.z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	view = position + newView;
}


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

///////////////////////////////// STRAFE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This strafes the camera left and right depending on the speed (-/+)
/////
///////////////////////////////// STRAFE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void FPSCamera::strafe(float speed)
{
	// Strafing is quite simple if you understand what the cross product is.
	// If you have 2 vectors (say the up viewVector and the view viewVector) you can
	// use the cross product formula to get a viewVector that is 90 degrees from the 2 vectors.
	// For a better explanation on how this works, check out the OpenGL "Normals" tutorial.
	// In our new update() function, we set the strafing vector (strafeVector).  Due
	// to the fact that we need this vector for many things including the strafing
	// movement and camera rotation (up and down), we just calculate it once.
	//
	// Like our move() function, we add the strafing vector to our current position 
	// and view.  It's as simple as that.  It has already been calculated in update().
	
	// Add the strafe vector to our position
	position.x += strafeVector.x * speed;
	position.z += strafeVector.z * speed;
    
    sphere.center=position;


	// Add the strafe vector to our view
	view.x += strafeVector.x * speed;
	view.z += strafeVector.z * speed;
}

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This will move the camera forward or backward depending on the speed
/////
///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void FPSCamera::move(float speed)
{
	// Get the current view vector (the direction we are looking)
	Vec3f viewVector = view - position;


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// I snuck this change in here!  We now normalize our view vector when
	// moving throughout the world.  This is a MUST that needs to be done.
	// That way you don't move faster than you strafe, since the strafe vector
	// is normalized too.
	viewVector = viewVector.normalize();
	
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


	position.x += viewVector.x * speed;		// Add our acceleration to our position's X
	position.z += viewVector.z * speed;		// Add our acceleration to our position's Z
	view.x += viewVector.x * speed;			// Add our acceleration to our view's X
	view.z += viewVector.z * speed;			// Add our acceleration to our view's Z
    
    sphere.center=position;
}


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// The next 3 functions were added to our camera class.  The less code in 
// Main.cpp the better.

//////////////////////////// CHECK FOR MOVEMENT \\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles the input faster than in the WinProc()
/////
//////////////////////////// CHECK FOR MOVEMENT \\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void FPSCamera::checkForMovement()
{
	if (KeyPress::isKeyCurrentlyDown(KeyPress::upKey)) {

		// Move our camera forward by a positive SPEED
		move(kSpeed);
	}

	// Check if we hit the Down arrow or the 's' key
	if (KeyPress::isKeyCurrentlyDown(KeyPress::downKey)) {

		// Move our camera backward by a negative SPEED
		move(-kSpeed);
	}

	// Check if we hit the Left arrow or the 'a' key
	if (KeyPress::isKeyCurrentlyDown(KeyPress::leftKey)) {

		// Strafe the camera left
		strafe(-kSpeed);
	}

	// Check if we hit the Right arrow or the 'd' key
	if (KeyPress::isKeyCurrentlyDown(KeyPress::rightKey)) {

		// Strafe the camera right
		strafe(kSpeed);
	}
}


///////////////////////////////// update \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This updates the camera's view and strafe vector
/////
///////////////////////////////// update \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void FPSCamera::update(float mouseX, float mouseY ,int width, int height)
{
    //Check rotation acumulator
    if (rotationX>  2*M_PI || rotationX < - 2*M_PI)
        rotationX=0;
    
	// Below we calculate the strafe vector every time we update
	// the camera.  This is because many functions use it so we might
	// as well calculate it only once.  

	// Initialize a variable for the cross product result
	Vec3f cross = (view - position).cross(upVector);

	// Normalize the strafe vector
	strafeVector = cross.normalize();

	// Move the camera's view by the mouse
	setViewByMouse(mouseX,mouseY,width,height);

	// This checks to see if the keyboard was pressed
	checkForMovement();
}


///////////////////////////////// look \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This updates the camera according to the 
/////
///////////////////////////////// look \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void FPSCamera::look()
{
	// Give openGL our camera position, then camera view, then camera up vector
	gluLookAt(position.x, position.y, position.z,	
			  view.x,	 view.y,     view.z,	
			  upVector.x, upVector.y, upVector.z);
}

void FPSCamera::checkCameraCollision(Triangle *roomTriangles, int numTriangles)
{
    // This function is pretty much a direct rip off of SpherePolygonCollision()
    // We needed to tweak it a bit though, to handle the collision detection once
    // it was found, along with checking every triangle in the list if we collided.
    // pVertices is the world data. If we have space partitioning, we would pass in
    // the vertices that were closest to the camera. What happens in this function
    // is that we go through every triangle in the list and check if the camera's
    // sphere collided with it.  If it did, we don't stop there.  We can have
    // multiple collisions so it's important to check them all.  One a collision
    // is found, we calculate the offset to move the sphere off of the collided plane.
    
    // Go through all the triangles
    for(int i = 0; i < numTriangles; i++)
    {
        // Store of the current triangle we testing

        
        // 1) STEP ONE - Finding the sphere's classification
        
        // We want the normal to the current polygon being checked
        Vec3f normal = roomTriangles[i].getNormal();
        
        // This will store the distance our sphere is from the plane
        float distance = 0.0f;
        
        //Sphere a;
        //float m_radius=1.2;//ver radio esfera
        // This is where we determine if the sphere is in FRONT, BEHIND, or INTERSECTS the plane
        
        for(int j=0;j<3;j++)
        {
        int classification = sphere.classifySphere(normal, roomTriangles[i].vertexes[j], distance);
        
        // If the sphere intersects the polygon's plane, then we need to check further
        if(classification == INTERSECTS)
        {
            // 2) STEP TWO - Finding the psuedo intersection point on the plane
            
            // Now we want to project the sphere's center onto the triangle's plane
            Vec3f offset = normal * distance;
            
            // Once we have the offset to the plane, we just subtract it from the center
            // of the sphere.  "vIntersection" is now a point that lies on the plane of the triangle.
            Vec3f vIntersection = position - offset;
            
            // 3) STEP THREE - Check if the intersection point is inside the triangles perimeter
            
            // We first check if our intersection point is inside the triangle, if not,
            // the algorithm goes to step 4 where we check the sphere again the polygon's edges.
            
            // We do one thing different in the parameters for EdgeSphereCollision though.
            // Since we have a bulky sphere for our camera, it makes it so that we have to
            // go an extra distance to pass around a corner. This is because the edges of
            // the polygons are colliding with our peripheral view (the sides of the sphere).
            // So it looks likes we should be able to go forward, but we are stuck and considered
            // to be colliding.  To fix this, we just pass in the radius / 2.  Remember, this
            // is only for the check of the polygon's edges.  It just makes it look a bit more
            // realistic when colliding around corners.  Ideally, if we were using bounding box
            // collision, cylinder or ellipses, this wouldn't really be a problem.
            
            if(roomTriangles[i].isInside(vIntersection) ||
               sphere.getEdgeSphereCollision(roomTriangles[i],true))
            {
                // If we get here, we have collided!  To handle the collision detection
                // all it takes is to find how far we need to push the sphere back.
                // GetCollisionOffset() returns us that offset according to the normal,
                // radius, and current distance the center of the sphere is from the plane.
                offset = getCollisionOffset(normal, sphere.radius, distance);
                
                // Now that we have the offset, we want to ADD it to the position and
                // view vector in our camera.  This pushes us back off of the plane.  We
                // don't see this happening because we check collision before we render
                // the scene.
                position = position + offset;
                view = view + offset;
                
                sphere.center=position;
            }
        }
        }
    }
}

Vec3f FPSCamera::getCollisionOffset(Vec3f &normal, float radius, float distance)
{
    Vec3f offset = Vec3f(0, 0, 0);
    
    // Once we find if a collision has taken place, we need make sure the sphere
    // doesn't move into the wall.  In our app, the position will actually move into
    // the wall, but we check our collision detection before we render the scene, which
    // eliminates the bounce back effect it would cause.  The question is, how do we
    // know which direction to move the sphere back?  In our collision detection, we
    // account for collisions on both sides of the polygon.  Usually, you just need
    // to worry about the side with the normal vector and positive distance.  If
    // you don't want to back face cull and have 2 sided planes, I check for both sides.
    //
    // Let me explain the math that is going on here.  First, we have the normal to
    // the plane, the radius of the sphere, as well as the distance the center of the
    // sphere is from the plane.  In the case of the sphere colliding in the front of
    // the polygon, we can just subtract the distance from the radius, then multiply
    // that new distance by the normal of the plane.  This projects that leftover
    // distance along the normal vector.  For instance, say we have these values:
    //
    //	normal = (1, 0, 0)		radius = 5		distance = 3
    //
    // If we subtract the distance from the radius we get: (5 - 3 = 2)
    // The number 2 tells us that our sphere is over the plane by a distance of 2.
    // So basically, we need to move the sphere back 2 units.  How do we know which
    // direction though?  This part is easy, we have a normal vector that tells us the
    // direction of the plane.
    // If we multiply the normal by the left over distance we get:  (2, 0, 0)
    // This new offset vectors tells us which direction and how much to move back.
    // We then subtract this offset from the sphere's position, giving is the new
    // position that is lying right on top of the plane.  Ba da bing!
    // If we are colliding from behind the polygon (not usual), we do the opposite
    // signs as seen below:
    
    // If our distance is greater than zero, we are in front of the polygon
    if(distance > 0)
    {
        // Find the distance that our sphere is overlapping the plane, then
        // find the direction vector to move our sphere.
        float distanceOver = radius - distance;
        offset = normal * distanceOver;
    }
    else // Else colliding from behind the polygon
    {
        // Find the distance that our sphere is overlapping the plane, then
        // find the direction vector to move our sphere.
        float distanceOver = radius + distance;
        offset = normal * -distanceOver;
    }
    
    // There is one problem with check for collisions behind the polygon, and that
    // is if you are moving really fast and your center goes past the front of the
    // polygon, it will then assume you were colliding from behind and not let
    // you back in.  Most likely you will take out the if / else check, but I
    // figured I would show both ways in case someone didn't want to back face cull.
    
    // Return the offset we need to move back to not be intersecting the polygon.
    
    //No mover en Z
    offset.y=0;
    
    return offset;
}
