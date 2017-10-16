#pragma once
#include "../Math/Vectors.h"
#include <math.h>



class Ray
{
public:
	Ray();

	Ray(const Vec3f& startPos, const Vec3f& direction);
    bool intersectedPlane(Vec3f planeNormal, Vec3f planeVertex, Vec3f &vNormal, float &originDistance);
    //Snell Law
    Vec3f getReflectionVector (Vec3f planeNormal);

	Vec3f startPos;
	Vec3f direction;
};