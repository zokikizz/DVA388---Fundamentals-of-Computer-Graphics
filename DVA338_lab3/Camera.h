#pragma once
#include "Vec3.h"

typedef struct _Camera {
	Vec3f position;
	Vec3f rotation;
	double fov;
	double nearPlane;
	double farPlane;
} Camera;

typedef struct _Light {
    Vec3f position;
    
    Vec3f Ia;
    Vec3f Id;
    Vec3f Is;

} Light;