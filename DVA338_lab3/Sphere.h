#ifndef _SPHERE_H_
#define _SPHERE_H_


#pragma once
#include "Vec3.h"
#include "Ray.h"
#include "./EasyBMP/EasyBMP.h"
#include "Camera.h"
// #include "common.h"

typedef struct _MappedCoords {
    float u, v;
} MappedCoords;

class Sphere {
public:
	Vec3f c;
	float r;
	Vec3f Ka;
	Vec3f Kd;
	Vec3f Ks;
	float shininess;
	int txtW;
	int txtH;
	
	unsigned char * textrure = NULL;

public:
	Sphere(const Vec3f & cen, float rad, float shine, Vec3f ka, Vec3f kd, Vec3f ks) 
	: c(cen), r(rad), shininess(shine), Ka(ka), Kd(kd), Ks(ks) { textrure = NULL; }

	bool hit(const Ray & r, HitRec & rec) const;
	void computeSurfaceHitFields(const Ray & r, HitRec & rec) const;
	Vec3f color(HitRec &rec, Light &light, Camera &cam);
	Vec3f GetColor(Vec3f direction);
	MappedCoords MapObjectCoords(Vec3f normal);
};

#endif