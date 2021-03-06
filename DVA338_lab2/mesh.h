#ifndef _MESH_H_
#define _MESH_H_

#include "algebra.h"

typedef struct _Triangle {
	int vInds[3]; //vertex indices
} Triangle;

typedef struct _Light {
	Vector Ia = { 0, 0, 0 };
	Vector Id = { 0, 0, 0 };
	Vector Is = { 0, 0, 0 };


	Vector position = { 0, 0, 0};

	Vector color = { 1.0f, 1.0f, 1.0f};
	float strength = 0.1;

} Light;

typedef struct _Mesh {
	// added for 1.5
	//rotation
	double rotationX = 0.0;
	double rotationY = 0.0;
	double rotationZ = 0.0;
	// translation
	double translationX = 0.0;
	double translationY = 0.0;
	double translationZ = 0.0;
	//scale
	double scaleX = 1.0;
	double scaleY = 1.0;
	double scaleZ = 1.0;
	// end added for 1.5

	// material of mesh

	Vector ambient;
	Vector diffuse;
	Vector specular;
	float shininess = 32.0;
	//end material

	int nv;
	Vector *vertices;
	Vector *vnorms;
	int nt;
	Triangle *triangles;
	struct _Mesh *next;

	unsigned int vbo, ibo, vao; // OpenGL handles for rendering
} Mesh;

typedef struct _Camera {
	Vector position;
	Vector rotation;
	double fov;
	double nearPlane;
	double farPlane;
} Camera;

void insertModel(Mesh ** objlist, int nv, float * vArr, int nt, int * tArr, float scale = 1.0);

Vector FindNormalForTriange(Vector pointA, Vector pointB, Vector pointC);
Vector FindNormal(Triangle polygon, Mesh *mesh);

char** loadShaderFromFile(char* filename);
void printShader(char** shader);

#endif
