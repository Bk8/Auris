#pragma once
#include "../Math/Vectors.h"
#include "Ray.h"
#include "IntersectionInfo.h"
#include "../Primitives/Triangle.h"
#include <math.h>

#define BEHIND		0	// This is returned if the sphere is completely behind the plane
#define INTERSECTS	1	// This is returned if the sphere intersects the plane
#define FRONT		2	// This is returned if the sphere is completely in front of the plane



class Sphere
{
public:
	Sphere();

	Sphere(const Vec3f& center, float radius);

    bool getIntersection(Ray* ray, IntersectionInfo& info);
    
    int  classifySphere(Vec3f &normal, Vec3f &point, float &distance);
    
    bool getEdgeSphereCollision(Triangle triangle,bool useHalfRadius);
    
    bool getSpherePolygonCollision(Triangle triangle);


    //http://stackoverflow.com/questions/1568091/why-use-getters-and-setters
	float radius;
	Vec3f center;
    
};