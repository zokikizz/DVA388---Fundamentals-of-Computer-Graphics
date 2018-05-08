#define _USE_MATH_DEFINES // To get M_PI defined
#include <math.h>
#include <stdio.h>
#include "algebra.h"
#include <cmath>
#include <math.h>

//rotation matix
static Matrix X, Y, Z;


Vector CrossProduct(Vector a, Vector b) {
	Vector v = { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x };
	return v;
}

float DotProduct(Vector a, Vector b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector Subtract(Vector a, Vector b) {
	Vector v = { a.x-b.x, a.y-b.y, a.z-b.z };
	return v;
}

Vector Add(Vector a, Vector b) {
	Vector v = { a.x+b.x, a.y+b.y, a.z+b.z };
	return v;
}

float Length(Vector a) {
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

Vector Normalize(Vector a) {
	float len = Length(a);
	Vector v = { a.x/len, a.y/len, a.z/len };
	return v;
}

Vector ScalarVecMul(float t, Vector a) {
	Vector b = { t*a.x, t*a.y, t*a.z };
	return b;
}

HomVector MatVecMul(Matrix a, Vector b) {
	HomVector h;
	h.x = b.x*a.e[0] + b.y*a.e[4] + b.z*a.e[8] + a.e[12];
	h.y = b.x*a.e[1] + b.y*a.e[5] + b.z*a.e[9] + a.e[13];
	h.z = b.x*a.e[2] + b.y*a.e[6] + b.z*a.e[10] + a.e[14];
	h.w = b.x*a.e[3] + b.y*a.e[7] + b.z*a.e[11] + a.e[15];
	return h;
}

Vector Homogenize(HomVector h) {
	Vector a;
	if (h.w == 0.0) {
		fprintf(stderr, "Homogenize: w = 0\n");
		a.x = a.y = a.z = 9999999;
		return a;
	}
	a.x = h.x / h.w;
	a.y = h.y / h.w;
	a.z = h.z / h.w;
	return a;
}

Matrix MatMatMul(Matrix a, Matrix b) {
	Matrix c;
	int i, j, k;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			c.e[j*4+i] = 0.0;
			for (k = 0; k < 4; k++)
				c.e[j*4+i] += a.e[k*4+i] * b.e[j*4+k];
		}
	}
	return c;
}

void PrintVector(char *name, Vector a) {
	printf("%s: %6.5lf %6.5lf %6.5lf\n", name, a.x, a.y, a.z);
}

void PrintHomVector(char *name, HomVector a) {
	printf("%s: %6.5lf %6.5lf %6.5lf %6.5lf\n", name, a.x, a.y, a.z, a.w);
}

void PrintMatrix(char *name, Matrix a) {
	int i,j;

	printf("%s:\n", name);
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			printf("%6.5lf ", a.e[j*4+i]);
		}
		printf("\n");
	}
}

void Transformations(Matrix& V, float translationX, float translationY, float translationZ, float rotationX, float rotationY, float rotationZ)
{

	V.e[12] = -translationX;
	V.e[13] = -translationY;
	V.e[14] = -translationZ;


//	Matrix X;

	X.e[0] = 1.0f; X.e[4] = 0.0f; X.e[ 8] = 0.0f; X.e[12] =   0.0f;
	X.e[1] = 0.0f; X.e[5] = cos(rotationX); X.e[ 9] = -sin(rotationX); X.e[13] =   0.0f;
	X.e[2] = 0.0f; X.e[6] = sin(rotationX); X.e[10] = cos(rotationX); X.e[14] = 0.0f;
	X.e[3] = 0.0f; X.e[7] = 0.0f; X.e[11] = 0.0f; X.e[15] =   1.0f;


	V = MatMatMul(V,X);


	//	Matrix Y;


	Y.e[0] = cos(rotationY); Y.e[4] = 0.0f; Y.e[ 8] = sin(rotationY); Y.e[12] =   0.0f;
	Y.e[1] = 0.0f; Y.e[5] = 1.0f; Y.e[ 9] = 0.0f; Y.e[13] =   0.0f;
	Y.e[2] = -sin(rotationY); Y.e[6] = 0.0f; Y.e[10] = cos(rotationY); Y.e[14] = 0.0f;
	Y.e[3] = 0.0f; Y.e[7] = 0.0f; Y.e[11] = 0.0f; Y.e[15] =   1.0f;


	V = MatMatMul(V,Y);

	//	Matrix Z;


	Z.e[0] = cos(rotationZ); Z.e[4] = -sin(rotationZ); Z.e[ 8] = 0.0f; Z.e[12] =   0.0f;
	Z.e[1] = sin(rotationZ); Z.e[5] = cos(rotationZ); Z.e[ 9] = 0.0f; Z.e[13] =   0.0f;
	Z.e[2] = 0.0f; Z.e[6] = 0.0f; Z.e[10] = 1.0f; Z.e[14] = 0.0f;
	Z.e[3] = 0.0f; Z.e[7] = 0.0f; Z.e[11] = 0.0f; Z.e[15] =   1.0f;


	V = MatMatMul(V,Z);

}

void RotationOfLight(Vector& V, float rotationX, float rotationY, float rotationZ)
{
//	Matrix X;

	X.e[0] = 1.0f; X.e[4] = 0.0f; X.e[ 8] = 0.0f; X.e[12] =   0.0f;
	X.e[1] = 0.0f; X.e[5] = cos(rotationX); X.e[ 9] = -sin(rotationX); X.e[13] =   0.0f;
	X.e[2] = 0.0f; X.e[6] = sin(rotationX); X.e[10] = cos(rotationX); X.e[14] = 0.0f;
	X.e[3] = 0.0f; X.e[7] = 0.0f; X.e[11] = 0.0f; X.e[15] =   1.0f;


	HomVector r = MatVecMul(X,V);

	V.x = r.x;
	V.y = r.y;
	V.z = r.z;


	//	Matrix Y;


	Y.e[0] = cos(rotationY); Y.e[4] = 0.0f; Y.e[ 8] = sin(rotationY); Y.e[12] =   0.0f;
	Y.e[1] = 0.0f; Y.e[5] = 1.0f; Y.e[ 9] = 0.0f; Y.e[13] =   0.0f;
	Y.e[2] = -sin(rotationY); Y.e[6] = 0.0f; Y.e[10] = cos(rotationY); Y.e[14] = 0.0f;
	Y.e[3] = 0.0f; Y.e[7] = 0.0f; Y.e[11] = 0.0f; Y.e[15] =   1.0f;


	r = MatVecMul(Y,V);


	V.x = r.x;
	V.y = r.y;
	V.z = r.z;
	//	Matrix Z;


	Z.e[0] = cos(rotationZ); Z.e[4] = -sin(rotationZ); Z.e[ 8] = 0.0f; Z.e[12] =   0.0f;
	Z.e[1] = sin(rotationZ); Z.e[5] = cos(rotationZ); Z.e[ 9] = 0.0f; Z.e[13] =   0.0f;
	Z.e[2] = 0.0f; Z.e[6] = 0.0f; Z.e[10] = 1.0f; Z.e[14] = 0.0f;
	Z.e[3] = 0.0f; Z.e[7] = 0.0f; Z.e[11] = 0.0f; Z.e[15] =   1.0f;


	r = MatVecMul(Z,V);


	V.x = r.x;
	V.y = r.y;
	V.z = r.z;
}

void PerspectiveProjection(Matrix &P, double fov, int screen_width, int screen_height, double nearPlane,
						   double farPlane)
{

	double aspect = (double)screen_width/(double)screen_height;
	double cont = 1.0/tan(fov/2/180*M_PI);

	P.e[0] = cont/aspect; P.e[4] = 0.000000f; P.e[ 8] =  0.000000f; P.e[12] =  0.0f;
	P.e[1] = 0.000000f; P.e[5] = cont; P.e[ 9] =  0.000000f; P.e[13] =  0.0f;
	P.e[2] = 0.000000f; P.e[6] = 0.000000f; P.e[10] = (farPlane+nearPlane)/(nearPlane-farPlane); P.e[14] = (2*farPlane*nearPlane)/(nearPlane-farPlane);
	P.e[3] = 0.000000f; P.e[7] = 0.000000f; P.e[11] = -1.000000f; P.e[15] =  0.0f;

}


void OrtoProjection(Matrix &P, Vector* vertices, int nv, double nearPlane, double farPlane)
{
	double minX, maxX, minY, maxY;
	minX = maxX = vertices[0].x;
	minY = maxY = vertices[0].y;

	for (int i = 1; i < nv ; ++i) {
		if(vertices[i].x > maxX)
			maxX = vertices[i].x;
		else if(vertices[i].x < minX)
			minX = vertices[i].x;

		if(vertices[i].y > maxY)
			maxY = vertices[i].y;
		else if(vertices[i].y < minY)
			minY = vertices[i].y;
	}



	P.e[0] = 2/(maxX-minX); P.e[4] = 0.000000f; P.e[ 8] =  0.000000f; P.e[12] =  -(maxX+minX)/(maxX-minX);
	P.e[1] = 0.000000f; P.e[5] = 2/(maxY-minY); P.e[ 9] =  0.000000f; P.e[13] =  -(maxY+minY)/(maxY-minY);
	P.e[2] = 0.000000f; P.e[6] = 0.000000f; P.e[10] = 2/(nearPlane-farPlane); P.e[14] = -(farPlane+nearPlane)/(farPlane-nearPlane);
	P.e[3] = 0.000000f; P.e[7] = 0.000000f; P.e[11] = 0.000000f; P.e[15] =  1.000000f;

}



void ModelTransforamtions(Matrix &W, double x, double y, double z, double rotationX, double rotationY, double rotationZ,
							double ScaleX,double ScaleY,double ScaleZ)
{
	Matrix T, R, S;
	for(int i = 0; i < 16; i++)
	{
		T.e[i] = 0.0f;
		R.e[i] = 0.0f;
	}

	T.e[0]= 1.0f;
	T.e[5]= 1.0f;
	T.e[10]= 1.0f;
	T.e[15]= 1.0f;


	T.e[12]= -x;
	T.e[13]= -y;
	T.e[14]= -z;

	//	Matrix X;

	X.e[0] = 1.0f; X.e[4] = 0.0f; X.e[ 8] = 0.0f; X.e[12] =   0.0f;
	X.e[1] = 0.0f; X.e[5] = cos(rotationX); X.e[ 9] = -sin(rotationX); X.e[13] =   0.0f;
	X.e[2] = 0.0f; X.e[6] = sin(rotationX); X.e[10] = cos(rotationX); X.e[14] = 0.0f;
	X.e[3] = 0.0f; X.e[7] = 0.0f; X.e[11] = 0.0f; X.e[15] =   1.0f;



	//	Matrix Y;


	Y.e[0] = cos(rotationY); Y.e[4] = 0.0f; Y.e[ 8] = sin(rotationY); Y.e[12] =   0.0f;
	Y.e[1] = 0.0f; Y.e[5] = 1.0f; Y.e[ 9] = 0.0f; Y.e[13] =   0.0f;
	Y.e[2] = -sin(rotationY); Y.e[6] = 0.0f; Y.e[10] = cos(rotationY); Y.e[14] = 0.0f;
	Y.e[3] = 0.0f; Y.e[7] = 0.0f; Y.e[11] = 0.0f; Y.e[15] =   1.0f;



	//	Matrix Z;


	Z.e[0] = cos(rotationZ); Z.e[4] = -sin(rotationZ); Z.e[ 8] = 0.0f; Z.e[12] =   0.0f;
	Z.e[1] = sin(rotationZ); Z.e[5] = cos(rotationZ); Z.e[ 9] = 0.0f; Z.e[13] =   0.0f;
	Z.e[2] = 0.0f; Z.e[6] = 0.0f; Z.e[10] = 1.0f; Z.e[14] = 0.0f;
	Z.e[3] = 0.0f; Z.e[7] = 0.0f; Z.e[11] = 0.0f; Z.e[15] =   1.0f;

	ScaleModel(S, ScaleX,ScaleY,ScaleZ);

	R = MatMatMul(Z,MatMatMul(Y,X));
	W = MatMatMul(T, MatMatMul(R,S));
}


void ScaleModel(Matrix& S,double x, double y, double z)
{
	for (int i = 0; i < 16; ++i) {
		if( i == 0)
			S.e[i] = x;
		else if (i==5)
			S.e[i]= y;
		else if (i==10)
			S.e[i] = z;
		else if(i == 15)
			S.e[i]= 1.0f;
		else
			S.e[i] = 0.0;
	}

}

