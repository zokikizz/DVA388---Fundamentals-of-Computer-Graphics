
#pragma once
#include "Sphere.h"

#include "stdio.h"
#include <iostream>
using namespace std;

#include <math.h>
#include <algorithm>


bool Sphere::hit(const Ray & r, HitRec & rec) const {	
	
	Vec3f v = c - r.o;
	float s = v.dot(r.d);
	float vLenSq = v.dot(v);
	float radSq = this->r * this->r; 
	if (s < 0 && vLenSq > radSq) {
		return false;
	}
	float mSq = vLenSq - s * s;
	if (mSq > radSq) {
		return false;
	}

	float t0,t1;
	float B, C, A;
	
	A = r.d.dot(r.d); // one
	Vec3f o_c = r.o - c;
	B = 2 * o_c.dot(r.d);
	C = o_c.dot(o_c) - radSq;
	
	double D = sqrt(B*B - 4*A*C);

	if ( D == 0.0f || D < 0)
	{
		return false;
	}
	
	t0 = (-B + D)/2*A;
	t1 = (-B - D)/2*A;	

	HitRec p0, p1;
	p0.tHit = t0;
	p1.tHit = t1;
	computeSurfaceHitFields(r, p0);
	computeSurfaceHitFields(r, p1);

	if(p0.tHit < p1.tHit && p0.tHit >= 0.0f)
	{
		rec.p = p0.p;
		rec.n = p0.n;
		rec.tHit = p0.tHit;
	}
	else if (p1.tHit >= 0.0f)
	{
		rec.p = p1.p;
		rec.n = p1.n;
		rec.tHit = p1.tHit;
	}

	
	rec.anyHit = true;

	return true;
}


void Sphere::computeSurfaceHitFields(const Ray & r, HitRec & rec) const {
	rec.p = r.o + r.d * rec.tHit;
	rec.n = (rec.p - c).normalize();
}

Vec3f Sphere::Phong(HitRec &hitRec, Light &light, Camera &cam)
{
	Vec3f ambient = light.Ia.multCoordwise(this->Ka);
			
	Vec3f lightDir =(light.position - hitRec.p).getNormalized();
	float diff = max(hitRec.n.dot(lightDir), 0.0f);
	Vec3f diffuse =(light.Id.multCoordwise(this->Kd))*diff;


	Vec3f viewDir = (cam.position - hitRec.p).getNormalized();
	lightDir.flip();
	Vec3f reflectionDir = lightDir - ( hitRec.n * 2.0f * hitRec.n.dot(lightDir));
	lightDir.flip();
	float spec = pow(max(viewDir.dot(reflectionDir), 0.0f), this->shininess);
	Vec3f specular = (this->Ks * spec).multCoordwise(light.Is);

	Vec3f res = ambient + diffuse + specular;
	return res;
}


Vec3f Sphere::GetColor(Vec3f normal)
{
	if(textrure == NULL)
		return Ka;
	else
	{
		Vec3f color;
		
		// float u = atan2(normal.x, normal.z) / (2*M_PI) + 0.5;
		// float v = normal.y * 0.5 + 0.5;

		float phi= atan2(normal.x, normal.z);

        if(phi < 0.0)
            phi += 2*M_PI;

		float u = phi / (2*M_PI);
		float v = ( M_PI  - acos(normal.y)) / M_PI;

		// fprintf(stderr, "%f %f \n", u, v);

		int row= (int) ((txtH - 1) * v);
		int column = (int) ((txtW - 1) * u);

		int value = 3*512*row + 3*column;


		// fprintf(stderr, "%d %d %d \n", textrure[value], textrure[value+1],textrure[value+2]);

		color.x = textrure[value]/(float)255;
		color.y = textrure[value + 1]/(float)255;
		color.z = textrure[value + 2]/(float)255;



		return color;
	}
}

MappedCoords Sphere::MapObjectCoords(Vec3f normal) {
    float u = 0.5 + atan2(normal.z, normal.x)/(2 * M_PI);
    float v = 0.5 - asin(normal.y)/M_PI;
    return MappedCoords{u, v};
}


