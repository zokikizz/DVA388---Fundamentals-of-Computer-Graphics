#include <stdlib.h>
#include "mesh.h"

#include <cmath>

#include <stdio.h>
#include <iostream>
using namespace std;

bool test = true;

float rnd() {
	return 2.0f * float(rand()) / float(RAND_MAX) - 1.0f;
}

void insertModel(Mesh **list, int nv, float * vArr, int nt, int * tArr, float scale) {
	Mesh * mesh = (Mesh *) malloc(sizeof(Mesh));
	mesh->nv = nv;
	mesh->nt = nt;
	mesh->vertices = (Vector *) malloc(nv * sizeof(Vector));
	mesh->vnorms = (Vector *)malloc(nv * sizeof(Vector));
	mesh->triangles = (Triangle *) malloc(nt * sizeof(Triangle));

	mesh->rotationX = 0.0;
	mesh->rotationY = 0.0;
	mesh->rotationZ = 0.0;
	// translation
	mesh->translationX = 0.0;
	mesh->translationY = 0.0;
	mesh->translationZ = 0.0;
	//scale
	mesh->scaleX = 1.0;
	mesh->scaleY = 1.0;
	mesh->scaleZ = 1.0;

	// set mesh vertices
	for (int i = 0; i < nv; i++) {
		mesh->vertices[i].x = vArr[i*3] * scale;
		mesh->vertices[i].y = vArr[i*3+1] * scale;
		mesh->vertices[i].z = vArr[i*3+2] * scale;

		mesh->vnorms[i].x = 0;
		mesh->vnorms[i].y = 0;
		mesh->vnorms[i].z = 0;
	}

	// set mesh triangles
	for (int i = 0; i < nt; i++) {
		mesh->triangles[i].vInds[0] = tArr[i * 3];
		mesh->triangles[i].vInds[1] = tArr[i * 3 + 1];
		mesh->triangles[i].vInds[2] = tArr[i * 3 + 2];
	}

	// Assignment 1:
	// Calculate and store suitable vertex normals for the mesh here.
	// Replace the code below that simply sets some arbitrary normal values

	Vector faceNormals[nt];
	Vector normalA;
	for(int i =0; i < nt; i++)
	{
		faceNormals[i]= {0,0,0};

		normalA = FindNormalForTriange(mesh->vertices[mesh->triangles[i].vInds[0]],
											  mesh->vertices[mesh->triangles[i].vInds[1]],
											  mesh->vertices[mesh->triangles[i].vInds[2]]);

		faceNormals[i] = normalA;

	}

	for(int i =0; i< nv; i++)
	{

		// find avg value of normals
		Vector normal = { 0.0, 0.0, 0.0};

		for (int j = 0; j < nt; ++j) {

			if (mesh->triangles[j].vInds[0] == i || mesh->triangles[j].vInds[1] == i || mesh->triangles[j].vInds[2] == i){
				normal = Add(normal, faceNormals[j]);

			}
		}
		mesh->vnorms[i] = Normalize(normal);


		// flat shading
//		mesh->vnorms[i] = faceNormals[i];
	}


	//default code

//	for(int i = 0; i < nv; i++)
//	{
//		mesh->vnorms[i].x = rnd();
//		mesh->vnorms[i].y = rnd();
//		mesh->vnorms[i].z = rnd();
//	}

	mesh->next = *list;
	*list = mesh;
}

Vector FindNormalForTriange(Vector pointA, Vector pointB, Vector pointC)
{
	Vector normal;
	Vector U, V;

	U.x = pointB.x - pointA.x;
	U.y = pointB.y - pointA.y;
	U.z = pointB.z - pointA.z;

	V.x = pointC.x - pointA.x;
	V.y = pointC.y - pointA.y;
	V.z = pointC.z - pointA.z;

	normal =  CrossProduct(U, V);

//	float sin_alpha = Length(ret) / (Length(U) * Length(V) );

	return Normalize(normal);
//	return ScalarVecMul(sin_alpha,CrossProduct(U,V));
}


