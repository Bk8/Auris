#pragma once
#include "../Math/Vectors.h"

class Sphere;

struct IntersectionInfo 
{
	Sphere* object;
	Vec3f intersPoint;
    float distance;
};