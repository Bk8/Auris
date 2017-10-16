#include "Sphere.h"

////////////////////////////////////////////////////////////////////////////////////////////

Sphere::Sphere()
{
	center = Vec3f(0, 0, 0);
	radius = 0;

}

////////////////////////////////////////////////////////////////////////////////////////////

Sphere::Sphere(const Vec3f& center, float radius):
center(center), radius(radius)
{

}
////////////////////////////////////////////////////////////////////////////////////////////

bool Sphere::getIntersection(Ray* ray, IntersectionInfo& info)
{
	Vec3f m = ray->startPos - center;
    float b = m.dot(ray->direction);
	float c = m.dot(m) - radius * radius;

	// Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}

	float discr = b*b - c;

	// A negative discriminant corresponds to ray missing sphere
	if (discr < 0.0f)
	{
		return false;
	}

	// Ray now found to intersect sphere, compute smallest t value of intersection
	float t = -b - sqrt(discr);

	// If t is negative, ray started inside sphere so clamp t to zero
	if (t < 0.0f)
        t = 0.0f;
	info.intersPoint = ray->startPos + t * ray->direction;
	info.object = this;
    info.distance=t;

	return true;
}






int Sphere::classifySphere(Vec3f &normal, Vec3f &point, float &distance)
{
    // First we need to find the distance our polygon plane is from the origin.
    // We need this for the distance formula below.
    float d = (float) point.getPlaneDistance(normal);
    
    // Here we use the famous distance formula to find the distance the center point
    // of the sphere is from the polygon's plane.
    // Remember that the formula is Ax + By + Cz + d = 0 with ABC = Normal, XYZ = Point
    distance = (normal.x * center.x + normal.y * center.y + normal.z * center.z + d);
    
    // Now we query the information just gathered.  Here is how Sphere Plane Collision works:
    // If the distance the center is from the plane is less than the radius of the sphere,
    // we know that it must be intersecting the plane.  We take the absolute value of the
    // distance when we do this check because once the center of the sphere goes behind
    // the polygon, the distance turns into negative numbers (with 0 being that the center
    // is exactly on the plane).  What do I mean when I say "behind" the polygon?  How do
    // we know which side is the front or back side?  Well, the side with the normal pointing
    // out from it is the front side, the other side is the back side.  This is all dependant
    // on the direction the vertices stored.  I recommend drawing them counter-clockwise.
    // if you go clockwise the normal with then point the opposite way and will screw up
    // everything.
    // So, if we want to find if the sphere is in front of the plane, we just make sure
    // the distance is greater than or equal to the radius.  let's say we have a radius
    // of 5, and the distance the center is from the plane is 6; it's obvious that the
    // sphere is 1 unit away from the plane.
    // If the sphere isn't intersecting or in front of the plane, it HAS to be BEHIND it.
    
    // If the absolute value of the distance we just found is less than the radius,
    // the sphere intersected the plane.
    if(fabs(distance) < radius)
        return INTERSECTS;
    // Else, if the distance is greater than or equal to the radius, the sphere is
    // completely in FRONT of the plane.
    else if(distance >= radius)
        return FRONT;
    
    // If the sphere isn't intersecting or in FRONT of the plane, it must be BEHIND
    return BEHIND;
}







bool Sphere::getEdgeSphereCollision(Triangle triangle,bool useHalfRadius)
{
    Vec3f point;
    
    // This function takes in the sphere's center, the polygon's vertices, the vertex count
    // and the radius of the sphere.  We will return true from this function if the sphere
    // is intersecting any of the edges of the polygon.  How it works is, every edge line
    // segment finds the closest point on that line to the center of the sphere.  If the
    // distance from that closest point is less than the radius of the sphere, there was
    // a collision.  Otherwise, we are definately out of reach of the polygon.  This works
    // for triangles, quads, and any other convex polygons.
    
    // Go through all of the vertices in the polygon
    for(int i = 0; i < 3; i++)
    {
        // This returns the closest point on the current edge to the center of the sphere.
        // Notice that we mod the second point of the edge by our vertex count.  This is
        // so that when we get to the last edge of the polygon, the second vertex of the
        // edge is the first vertex that we starting with.
        point = center.getClosestPointOnLine(triangle.vertexes[i], triangle.vertexes[(i + 1) % 3]);
        
        // Now, we want to calculate the distance between the closest point and the center
        float distance = point.distance(center);
        
        // If the distance is less than the radius, there must be a collision so return true
        if(useHalfRadius)
            if(distance < radius/2)
                return true;
        else
            if(distance < radius)
                return true;
    }
    
    // The was no intersection of the sphere and the edges of the polygon
    return false;
}

bool Sphere::getSpherePolygonCollision(Triangle triangle)
{
    // This function is the only function we need to call for testing if a sphere
    // collides with a polygon.  The rest are just helper functions called within here.
    // The theory is actually quite difficult to understand, especially if you are
    // a beginner to collision detection and are following the tutorials in order, but
    // I will try to be as gentle and descriptive as possible.  Let go!
    // Basically, here is the overview:
    //
    // 1) First you want to check if the sphere collides with the polygon's plane.
    //    Remember, that planes are infinite and you could be 500 units from the
    //    polygon and it's still going to trigger this first test.  We want to
    //    write a function that classifies the sphere.  Either it's completely
    //    in front of the plane (the side the normal is on), intersecting the
    //    plane or completely behind the plane.  Got it so far?  We created a
    //    function called ClassifySphere() that returns BEHIND, FRONT or INTERSECTS.
    //    If ClassifySphere() returns INTERSECTS, then we move on to step 2, otherwise
    //    we did not collide with the polygon.
    //
    // 2) The second step is to get an intersection point right in front of the sphere.
    //    This one of the tricky parts.  We know that once we have an intersection point
    //    on the plane of the polygon, we just need to use the InsidePolygon() function
    //    to see if that point is inside the dimensions of the polygon, just like we
    //    did with the Ray to Polygon Collision tutorial.  So, how do we get the point
    //    of intersection?  It's not as simple as it might sound.  Since a sphere infinite
    //    points, there would be a million points that it collided at.  You can't just
    //    draw a ray in the direction the sphere was moving because it could have just
    //    nicked the bottom of the polygon and your ray would find an intersection
    //    point that is outside of the polygon.  Well, it turns out that we need to
    //    first try and give it a shot.  We will try the first attempt a different way though.
    //    We know that we can find the normal vector of the polygon, which in essence
    //    tells us the direction that the polygon is facing.  From ClassifyPoly(),
    //	  it also returns the distance the center our sphere is from the plane.  That
    //    means we have a distance our sphere center is from the plane, and the normal
    //    tells us the direction the plane is in.  If we multiply the normal by the
    //    distance from the plane we get an offset.  This offset can then be subtracted
    //    from the center of the sphere.  Believe it or not, but we now have a position
    //    on the plane in the direction of the plane.  Usually, this intersection points
    //    works fine, but if we get around the edges of the polygon, this does not work.
    //    What we just did is also called "projecting the center of the sphere onto the plane".
    //    Another way to do this is to shoot out a ray from the center of the sphere in
    //    the opposite direction of the normal, then we find the intersection of that line
    //    and the plane.  My way just takes 3 multiplies and a subtraction.  You choose.
    //
    // 3) Once we have our psuedo intersection point, we just pass it into InsidePolygon(),
    //    along with the polygon vertices and the vertex count.  This will then return
    //    true if the intersection point was inside of the polygon, otherwise false.
    //    Remember, just because this returns false doesn't mean we stop there!  If
    //    we didn't collide yet, we need to skip to step 4.
    //
    // 4) If we get here, it's assumed that we tried our intersection point and it
    //    wasn't in the polygon's perimeter.  No fear!  There is hope!  If we get to step
    //    4, that's means that our center point is outside of the polygon's perimeter. Since
    //    we are dealing with a sphere, we could still be colliding because of the sphere's radius.
    //	  This last check requires us to find the point on each of the polygon's edges that
    //    is closest to the sphere's center.  We have a tutorial on finding this, so make sure
    //    you have read it or are comfortable with the concept.  If we are dealing with a
    //    triangle, we go through every side and get an edge vector, and calculate the closest
    //    point on those lines to our sphere's center.  After getting each closest point, we
    //    calculate the distance that point is from our sphere center.  If the distance is
    //    less than the radius of the sphere, there was a collision.  This way is pretty fast.
    //    You don't need to calculate all three sides evey time, since the first closest point's
    //    distance could be less than the radius and you return "true".
    //
    // That's the overview, *phew!*.  I bet you are reading this just wanting to cry because
    // that seems like so much math and theory to digest, so the code must be atrocious!
    // Well, you are partially right :)  It's not that bad actually, quite straight forward.
    // I will label the steps in the code so you can go back and forth to the overview and code.
    // I might mention that by having our CVector3 class operator overloaded it cuts down the
    // code tremendously.  If you are confused with this concept of C++, just create functions
    // to add, subtract and multiply vectors or scalars together.
    //
    
    // 1) STEP ONE - Finding the sphere's classification
    
    // Let's use our Normal() function to return us the normal to this polygon
    Vec3f normal = triangle.getNormal();
    
    // This will store the distance our sphere is from the plane
    float distance = 0.0f;
    
    // This is where we determine if the sphere is in FRONT, BEHIND, or INTERSECTS the plane
    // of the polygon.  We pass is our sphere center, the polygon's normal, a point on
    // the plane (vertex), the sphere's radius and an empty float to fill the distance with.
    int classification = classifySphere(normal, triangle.vertexes[0],distance);
    
    // If the sphere intersects the polygon's plane, then we need to check further,
    // otherwise the sphere did NOT intersect the polygon.  Pretty fast so far huh?
    if(classification == INTERSECTS)
    {
        // 2) STEP TWO - Finding the psuedo intersection point on the plane
        
        // Now we want to project the sphere's center onto the polygon's plane,
        // in the direction of the normal.  This is done by multiplying the "normal"
        // by the "distance" the sphere center is from the plane.  We got the distance
        // from the ClassifySphere() function call up above.  2 return values were given
        // through the "distance" variable being passed in as a reference.  If projecting
        // is confusing to you, just think of it as this: "I am starting at the center
        // of the sphere and I am going to just run into the plane.  I will move in the
        // direction that is reverse from the normal.  When do I know when to stop?  Well,
        // I just go in that direction until my distance from the center is the same as
        // the distance the center of the sphere is from the plane."  By doing this
        // we get an offset to subtract from the center of the sphere.
        Vec3f offset = normal * distance;
        
        // Once we have the offset to the plane, we just subtract it from the center
        // of the sphere.  "position" now a point that lies on the plane of the polygon.
        // Whether it is inside the polygon's perimeter is another story.  Usually it
        // is though, unless we get near the edges of the polygon.
        Vec3f position = center - offset;
        
        // 3) STEP THREE - Check if the intersection point is inside the polygons perimeter
        
        // This is the same function used in our previous tutorial on Ray to Polygon Collision.
        // If the intersection point is inside the perimeter of the polygon, it returns true.
        // We pass in the intersection point, the list of vertices and vertex count of the poly.
        if(triangle.isInside(position))
            return true;	// We collided!
        else
        {
            // 4) STEP FOUR - Check the sphere to see if it intersects the polygon edges
            
            // If we get here, we didn't find an intersection point in the perimeter.
            // There is still one more chance to redeem our sphere that it can hit the mark.
            // If any part of the sphere intersects the edges of the polygon, we collided.
            // This is only checked if the sphere's center point is outside the edges of the
            // polygon. We pass in the center of the sphere, the list of verts, the polygon 
            // vertex count and the sphere's radius.  If this returns true we have a collision.
            if(getEdgeSphereCollision(triangle,false))
            {
                return true;	// We collided! "And you doubted me..." - Sphere
            }
        }
    }
    
    // If we get here, there is obviously no collision happening up in this crib
    return false;
}