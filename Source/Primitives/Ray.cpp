#include "Ray.h"

////////////////////////////////////////////////////////////////////////////////////////////

Ray::Ray()
{
	startPos  = Vec3f(0, 0, 0);
	direction = Vec3f(0, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////

Ray::Ray(const Vec3f& newStartPos, const Vec3f& newDirection):
startPos(newStartPos), direction(newDirection)
{
	direction.normalize();
}

bool Ray:: intersectedPlane(Vec3f planeNormal, Vec3f planeVertex, Vec3f &vNormal, float &originDistance)
{
    float distance1 = 0, distance2 = 0;						// The distances from the 2 points of the line from the plane
    
    
    // Let's find the distance our plane is from the origin.  We can find this value
    // from the normal to the plane (polygon) and any point that lies on that plane (Any vertex)
    originDistance = planeVertex.getPlaneDistance(planeNormal);
    
    // Get the distance from point1 from the plane using: Ax + By + Cz + D = (The distance from the plane)
    
    distance1 = ((planeNormal.x * startPos.x) +					// Ax +
                 (planeNormal.y * startPos.y) +					// Bx +
                 (planeNormal.z * startPos.z)) + originDistance;	// Cz + D
    
    // Get the distance from point2 from the plane using Ax + By + Cz + D = (The distance from the plane)
    
    distance2 = ((planeNormal.x * direction.x) +					// Ax +
                 (planeNormal.y * direction.y) +					// Bx +
                 (planeNormal.z * direction.z)) + originDistance;	// Cz + D
    
    // Now that we have 2 distances from the plane, if we times them together we either
    // get a positive or negative number.  If it's a negative number, that means we collided!
    // This is because the 2 points must be on either side of the plane (IE. -1 * 1 = -1).
    
    if (distance1 * distance2 >= 0)			// Check to see if both point's distances are both negative or both positive
        return false;						// Return false if each point has the same sign.  -1 and 1 would mean each point is on either side of the plane.  -1 -2 or 3 4 wouldn't...
    
    return true;							// The line intersected the plane, Return TRUE
}

//Snell Law
Vec3f Ray::getReflectionVector (Vec3f normal){
    
    Vec3f aux (direction.x,direction.y,direction.z);
    
    float Vi_nt = aux.dot(normal);
    float arccos = acos(Vi_nt);
    float sen = sin(arccos - (M_PI/2))*2;
    
    aux=normal*sen + aux;
    
    return aux;
};

