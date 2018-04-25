//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
using namespace std;

#ifdef __linux__
#	include <GL/glew.h>
#	include <GL/freeglut.h>
#else
#	include <glew.h>
#	include <freeglut.h>
#endif

#include "algebra.h"
#include "shaders.h"
#include "mesh.h"
#include <cmath>

int screen_width = 1024;
int screen_height = 768;

int indexOfModel = 0;
int totalNumOfModel = 0;

#define PI 3.14159265

bool rep  = true;

//rotation matix
Matrix X, Y, Z;

bool orto = false;


Mesh *meshList = NULL; // Global pointer to linked list of triangle meshes

Camera cam = {{0,0,20}, {0,0,0}, 60, 1, 10000}; // Setup the global camera parameters


GLuint shprg; // Shader program id


// Global transform matrices
// V is the view transform
// P is the projection transform
// PV = P * V is the combined view-projection transform
Matrix V, P, PV;


void print_log(GLuint object) {
	GLint log_length = 0;
	if (glIsShader(object))
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else if (glIsProgram(object))
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else {
		fprintf(stderr, "printlog: Not a shader or a program\n");
		return;
	}
	char* log = (char*)malloc(log_length);
	if (glIsShader(object))
		glGetShaderInfoLog(object, log_length, NULL, log);
	else if (glIsProgram(object))
		glGetProgramInfoLog(object, log_length, NULL, log);
	fprintf(stderr, "%s", log);
	free(log);
}

void prepareShaderProgram(const char ** vs_src, const char ** fs_src) {
	GLint success = GL_FALSE;

	shprg = glCreateProgram();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_src, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if(success==GL_FALSE) {
		fprintf(stderr, "vs_src:");
		print_log(vs);
		glDeleteShader(vs);
	} else printf("Vertex shader compiled successfully!\n");

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fs_src, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if(success==GL_FALSE) {
		fprintf(stderr, "fs_src:");
		print_log(fs);
		glDeleteShader(fs);
	} else printf("Fragment shader compiled successfully!\n");

	glAttachShader(shprg, vs);
	glAttachShader(shprg, fs);
	glLinkProgram(shprg);
	GLint isLinked = GL_FALSE;
	glGetProgramiv(shprg, GL_LINK_STATUS, &isLinked);
	if(isLinked==GL_FALSE) {
		fprintf(stderr, "glLinkProgram:");
		print_log(shprg);
		glDeleteProgram(shprg);
	} else printf("Shader program linked successfully!\n");
}

void prepareMesh(Mesh *mesh) {
	int sizeVerts = mesh->nv * 3 * sizeof(float);
	int sizeCols  = mesh->nv * 3 * sizeof(float);
	int sizeTris = mesh->nt * 3 * sizeof(int);

	// For storage of state and other buffer objects needed for vertex specification
	glGenVertexArrays(1, &mesh->vao);
	glBindVertexArray(mesh->vao);

	// Allocate VBO and load mesh data (vertices and normals)
	glGenBuffers(1, &mesh->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts + sizeCols, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeVerts, (void *)mesh->vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeVerts, sizeCols, (void *)mesh->vnorms);

	// Allocate index buffer and load mesh indices
	glGenBuffers(1, &mesh->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeTris, (void *)mesh->triangles, GL_STATIC_DRAW);

	// Define the format of the vertex data
	GLint vPos = glGetAttribLocation(shprg, "vPos");
	glEnableVertexAttribArray(vPos);
	glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Define the format of the vertex data
	GLint vNorm = glGetAttribLocation(shprg, "vNorm");
	glEnableVertexAttribArray(vNorm);
	glVertexAttribPointer(vNorm, 3, GL_FLOAT, GL_FALSE, 0, (void *)(mesh->nv * 3 *sizeof(float)));

	glBindVertexArray(0);

}

void renderMesh(Mesh *mesh) {

	// Assignment 1: Apply the transforms from local mesh coordinates to world coordinates here
	// Combine it with the viewing transform that is passed to the shader below
	Matrix M; // model matrix
	Matrix W;

	fprintf(stderr, "\nmesh param: %d, %d\n",indexOfModel, totalNumOfModel);
	ModelTransforamtions(W, mesh->translationX,mesh->translationY, mesh->translationZ, mesh->rotationX,
							 mesh->rotationY,mesh->rotationZ,mesh->scaleX, mesh->scaleY, mesh->scaleZ);
	M = MatMatMul(PV,W);

	// Pass the viewing transform to the shader
	GLint loc_PV = glGetUniformLocation(shprg, "PV");
//	glUniformMatrix4fv(loc_PV, 1, GL_FALSE, PV.e);

	glUniformMatrix4fv(loc_PV, 1, GL_FALSE, M.e);

	// Select current resources
	glBindVertexArray(mesh->vao);

	// To accomplish wireframe rendering (can be removed to get filled triangles)
	// z buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Draw all triangles
	glDrawElements(GL_TRIANGLES, mesh->nt * 3, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}


void display(void) {
	Mesh *mesh;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Assignment 1: Calculate the transform to view coordinates yourself
	// The matrix V should be calculated from camera parameters
	// Therefore, you need to replace this hard-coded transform.
	V.e[0] = 1.0f; V.e[4] = 0.0f; V.e[ 8] = 0.0f; V.e[12] = 0.0f;
	V.e[1] = 0.0f; V.e[5] = 1.0f; V.e[ 9] = 0.0f; V.e[13] = 0.0f;
	V.e[2] = 0.0f; V.e[6] = 0.0f; V.e[10] = 1.0f; V.e[14] = 0.0f;
	V.e[3] = 0.0f; V.e[7] = 0.0f; V.e[11] = 0.0f; V.e[15] =   1.0f;

	Transformations(V, cam.position.x, cam.position.y, cam.position.z, cam.rotation.x, cam. rotation.y, cam.rotation.z);


	// Assignment 1: Calculate the projection transform yourself
	// The matrix P should be calculated from camera parameters
	// Therefore, you need to replace this hard-coded transform.

	// aspect ratio is connected to resolution (if it is 640x480 then it is 4:3) for full hd is 16:9
//	P.e[0] = 1.299038f; P.e[4] = 0.000000f; P.e[ 8] =  0.000000f; P.e[12] =  0.0f;
//	P.e[1] = 0.000000f; P.e[5] = 1.732051f; P.e[ 9] =  0.000000f; P.e[13] =  0.0f;
//	P.e[2] = 0.000000f; P.e[6] = 0.000000f; P.e[10] = -1.000200f; P.e[14] = -2.00000f;
//	P.e[3] = 0.000000f; P.e[7] = 0.000000f; P.e[11] = -1.000000f; P.e[15] =  0.0f;

	if(orto)
		OrtoProjection(P, meshList->vertices, meshList->nv, cam.nearPlane, cam.farPlane);
	else
		PerspectiveProjection(P, cam.fov, screen_width, screen_height, cam.nearPlane, cam.farPlane);


	// This finds the combined view-projection matrix
	PV = MatMatMul(P, V);

	// Select the shader program to be used during rendering
	glUseProgram(shprg);

	// Render all meshes in the scene
	mesh = meshList;

	while (mesh != NULL) {
		renderMesh(mesh);
		mesh = mesh->next;
	}

	glFlush();
}

void changeSize(int w, int h) {
	screen_width = w;
	screen_height = h;
	glViewport(0, 0, screen_width, screen_height);

}

void keypress(unsigned char key, int x, int y) {
	Mesh *currentModel = meshList;
	int i = 0;
	while (i < indexOfModel)
	{
		currentModel = currentModel->next;
		i++;
	}
	switch(key) {
	case 'z':
		cam.position.z -= 0.2f;
		break;
	case 'Z':
		cam.position.z += 0.2f;
		break;
	case 'x':
		cam.position.x -= 0.2f;
		break;
	case 'X':
		cam.position.x += 0.2f;
		break;
	case 'y':
		cam.position.y -= 0.2f;
		break;
	case 'Y':
		cam.position.y += 0.2f;
		break;
	case 'i':
		cam.rotation.x -= 0.2f;
		break;
	case 'I':
		cam.rotation.x += 0.2f;
		break;
	case 'j':
		cam.rotation.y -= 0.2f;
		break;
	case 'J':
		cam.rotation.y += 0.2f;
		break;
	case 'k':
		cam.rotation.z -= 0.2f;
		break;
	case 'K':
		cam.rotation.z += 0.2f;
		break;
	case 'p':
		orto = !orto;
		break;
	case 'w':
		currentModel->translationX += 1.0;
		break;
	case 'W':
		currentModel->translationX -= 1.0;
		break;
	case 's':
		currentModel->translationY += 1.0;
		break;
	case 'S':
		currentModel->translationY -= 1.0;
		break;
	case 'a':
		currentModel->translationZ += 1.0;
		break;
	case 'A':
		currentModel->translationZ -= 1.0;
		break;
	case 'r':
		currentModel->rotationX += 0.1;
		break;
	case 'R':
		currentModel->rotationX -= 0.1;
		break;
	case 'e':
		currentModel->rotationY += 0.1;
		break;
	case 'E':
		currentModel->rotationY -= 0.1;
		break;
	case 'd':
		currentModel->rotationZ += 0.1;
		break;
	case 'D':
		currentModel->rotationZ -= 0.1;
		break;
	case 't':
		currentModel->scaleX += 0.1;
		currentModel->scaleY += 0.1;
		currentModel->scaleZ += 0.1;
		break;
	case 'T':
		currentModel->scaleX -= 0.1;
		currentModel->scaleY -= 0.1;
		currentModel->scaleZ -= 0.1;

		break;
	case 'm':
		indexOfModel = (indexOfModel + 1) % totalNumOfModel;
		break;
	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;

	}
	glutPostRedisplay();
}

void init(void) {
	// Compile and link the given shader program (vertex shader and fragment shader)
	prepareShaderProgram(vs_n2c_src, fs_ci_src);
	indexOfModel = 0;
	totalNumOfModel = 0;

	// Setup OpenGL buffers for rendering of the meshes
	Mesh * mesh = meshList;
	while (mesh != NULL) {
		fprintf(stderr, "%d\n", totalNumOfModel);
		totalNumOfModel = totalNumOfModel+ 1;
		prepareMesh(mesh);
		mesh = mesh->next;
	}
}

void cleanUp(void) {
	printf("Running cleanUp function... ");
	// Free openGL resources
	// ...

	// Free meshes
	// ...
	printf("Done!\n\n");
}

// Include data for some triangle meshes (hard coded in struct variables)
#include "./models/mesh_bunny.h"
#include "./models/mesh_cow.h"
#include "./models/mesh_cube.h"
#include "./models/mesh_frog.h"
#include "./models/mesh_knot.h"
#include "./models/mesh_sphere.h"
#include "./models/mesh_teapot.h"
#include "./models/mesh_triceratops.h"


int main(int argc, char **argv) {

	// Setup freeGLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("DVA338 Programming Assignments");
	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(keypress);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	
	// Specify your preferred OpenGL version and profile
	glutInitContextVersion(3, 0);
	//glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitContextProfile(GLUT_CORE_PROFILE);


	// Uses GLEW as OpenGL Loading Library to get access to modern core features as well as extensions
	GLenum err = glewInit();
	if (GLEW_OK != err) { fprintf(stdout, "Error: %s\n", glewGetErrorString(err)); return 1; }

	// Output OpenGL version info
	fprintf(stdout, "GLEW version: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, "OpenGL version: %s\n", (const char *)glGetString(GL_VERSION));
	fprintf(stdout, "OpenGL vendor: %s\n\n", glGetString(GL_VENDOR));


	// Insert the 3D models you want in your scene here in a linked list of meshes
	// Note that "meshList" is a pointer to the first mesh and new meshes are added to the front of the list
	insertModel(&meshList, cow.nov, cow.verts, cow.nof, cow.faces, 20.0);
	//insertModel(&meshList, triceratops.nov, triceratops.verts, triceratops.nof, triceratops.faces, 3.0);
//	insertModel(&meshList, bunny.nov, bunny.verts, bunny.nof, bunny.faces, 60.0);
	insertModel(&meshList, cube.nov, cube.verts, cube.nof, cube.faces, 5.0);
	 //insertModel(&meshList, frog.nov, frog.verts, frog.nof, frog.faces, 2.5);
//	insertModel(&meshList, knot.nov, knot.verts, knot.nof, knot.faces, 1.0);
//	insertModel(&meshList, sphere.nov, sphere.verts, sphere.nof, sphere.faces, 5.0);
	//insertModel(&meshList, teapot.nov, teapot.verts, teapot.nof, teapot.faces, 3.0);


	init();
	glutMainLoop();

	cleanUp();
	return 0;
}
