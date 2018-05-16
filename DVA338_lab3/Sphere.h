#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Vec3.h"
#include "Ray.h"
#include "Camera.h"

class Sphere {
public:
	Vec3f c;
	float r;
	Vec3f Ka;
	Vec3f Kd;
	Vec3f Ks;
	float shininess;
public:
	Sphere(const Vec3f & cen, float rad, float shine, Vec3f ka, Vec3f kd, Vec3f ks) 
	: c(cen), r(rad), shininess(shine), Ka(ka), Kd(kd), Ks(ks) { }

	bool hit(const Ray & r, HitRec & rec) const;
	void computeSurfaceHitFields(const Ray & r, HitRec & rec) const;
	Vec3f color(HitRec &rec, Light &light, Camera &cam);
};

#endif