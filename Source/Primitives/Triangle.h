//
//  Triangle.hpp
//  OpenGL
//
//  Created by Alex on 12/9/16.
//
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include "../Math/Vectors.h"
#include "../Primitives/Ray.h"
#include <math.h>


class Triangle
{
public:
    
    Triangle(Vec3f v1, Vec3f v2,Vec3f v3)
    {
        vertexes[0]=v1;
        vertexes[1]=v2;
        vertexes[2]=v3;
    }
    
    Triangle()
    {
        vertexes[0].x=0;
        vertexes[1].y=0;
        vertexes[2].z=0;
        
        vertexes[0].x=0;
        vertexes[1].y=0;
        vertexes[2].z=0;
        
        vertexes[0].x=0;
        vertexes[1].y=0;
        vertexes[2].z=0;
    }
    
    bool intersectsTriangle(Ray* ray, double & dist,Vec3f& intersection){
        /*MöllerTrumbore intersection algorithm
         thanks wikipedia :)
         https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
         */
        double e = 0.00001;
        
        Vec3f v1 = vertexes[0];
        Vec3f v2 = vertexes[1];
        Vec3f v3 = vertexes[2];
        
        Vec3f e1 = v2- v1;
        Vec3f e2 = v3- v1;
        
        //Begin calculating determinant - also used to calculate u parameter
        Vec3f p = ray->direction.cross(e2);
        double det = e1.dot(p);
        
        //If determinant is near zero, ray lies in plane of triangle
        if (det > -e && det < e)
            return false;
        double inv_det = 1.0 / det;
        
        Vec3f t = ray->startPos-v1;
        double u = t.dot(p) * inv_det;
        //The intersection lies outside of the triangle
        if (u < 0.0 || u > 1.0)
            return false;
        
        Vec3f q = t.cross(e1);
        double v = ray->direction.dot(q) * inv_det;
        //The intersection lies outside of the triangle
        if (v < 0.0 || u + v > 1.0)
            return false;
        
        double a = e2.dot(q) * inv_det;
        
        //http://gamedev.stackexchange.com/questions/114955/m%C3%B6ller-trumbore-intersection-point
        
        intersection = v1+u*e2+v*e1;
        
        if (a > e){
            dist = a;
            return true;
        }
        
        return false;
    }

	Vec3f getNormal()
	{														// Get 2 vectors from the polygon (2 sides), Remember the order!
		Vec3f vector1 = vertexes[2] - vertexes[0];
		Vec3f vector2 = vertexes[1] - vertexes[0];

		Vec3f normal =  vector1.cross(vector2);		// Take the cross product of our 2 vectors to get a perpendicular vector

														// Now we have a normal, but it's at a strange length, so let's make it length 1.

		normal = normal.normalize();						// Use our function we created to normalize the normal (Makes it a length of one)

		return normal;										// Return our normal at our desired length
	}
    
    bool isInside(Vec3f vertex)
    {
            const double MATCH_FACTOR = 0.99;		// Used to cover up the error in floating point
            double Angle = 0.0;						// Initialize the angle
            Vec3f vA, vB;						// Create temp vectors
            
            for (int i = 0; i < 3; i++)		// Go in a circle to each vertex and get the angle between
            {
                vA = vertexes[i] - vertex;			// Subtract the intersection point from the current vertex
                // Subtract the point from the next vertex
                vB = vertexes[(i + 1) % 3] - vertex;
                
                Angle += vA.getAngleBetweenVectors(vB);	// Find the angle between the 2 vectors and add them all up as we go along
            }
            
            if(Angle >= (MATCH_FACTOR * (2.0 * M_PI)) )	// If the angle is greater than 2 PI, (360 degrees)
                return true;							// The point is inside of the polygon
            
            return false;								// If you get here, it obviously wasn't inside the polygon, so Return FALSE
        
    }
    
    Vec3f vertexes[3];
};
#endif /* Triangle_hpp */
