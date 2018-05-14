#include <stdlib.h>
#include "mesh.h"

#include <cmath>

#include<stdio.h>
#include<string.h>
#include <iostream>
using namespace std;

bool test = true;

#include <fstream>
ifstream inFile;

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

	Vector normalA;
	for(int i =0; i < nt; i++)
	{
		normalA = FindNormalForTriange(mesh->vertices[mesh->triangles[i].vInds[1]],
									   mesh->vertices[mesh->triangles[i].vInds[2]],
									   mesh->vertices[mesh->triangles[i].vInds[0]]);

		mesh->vnorms[mesh->triangles[i].vInds[0]] = Add(mesh->vnorms[mesh->triangles[i].vInds[0]], normalA);
		mesh->vnorms[mesh->triangles[i].vInds[1]] = Add(mesh->vnorms[mesh->triangles[i].vInds[1]], normalA);
		mesh->vnorms[mesh->triangles[i].vInds[2]] = Add(mesh->vnorms[mesh->triangles[i].vInds[2]], normalA);

	}


	for(int i =0; i< nv; i++)
	{
		mesh->vnorms[i] = Normalize(mesh->vnorms[i]);
	}

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

char** loadShaderFromFile(char * filename) {
	char line[256];
	char **shader;
	int numberOfLines = 0;


	inFile.open(filename);

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}


	while((inFile.getline(line,256)))
		numberOfLines++;


	shader = new char*[1];
	shader[0] = new char[numberOfLines * 256];
	for(int i = 0; i < (numberOfLines * 256); i++)
		shader[0][i] = '\0';


    inFile.close();
	inFile.open(filename);

	int currentLine;
	while((inFile.getline(line,256)))
	{
		strcat(shader[0],line);
		strcat(shader[0],"\n");
	}

	inFile.close();

	return shader;
}

void printShader(char** shader)
{
	cout << shader[0] << "\n";
}


