#ifdef __linux__
#	include <GL/glew.h>
#	include <GL/freeglut.h>
#else
#	include <glew.h>
#	include <freeglut.h>
#endif


#include <vector>
#include <iostream>
#include <stdio.h>
using namespace std;

#include <GL/glut.h>
#include "Vec3.h"
#include "Image.h"
#include "Ray.h"
#include "Sphere.h"
#include "Camera.h"

Camera cam;
Light light;


class Scene {
public:
	vector<Sphere> spheres;		

	Scene(void) {
		 
	}
	void add(const Sphere & s) {
		spheres.push_back(s); 
		cout << "Sphere added: " << "r = " << spheres[spheres.size()-1].r << endl;
	}

	void load(char * fileName) {
		// load a file with spheres for your scene here ...
	}

};


void glSetPixel(int x, int y, const Vec3f & c) {
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

class SimpleRayTracer {
private: 
	Scene * scene;
	Image * image;

	Vec3f getEyeRayDirection(int x, int y) {
		//Uses a fix camera looking along the negative z-axis
		static float z = -5.0f;		
		static float sizeX = 4.0f; 
		static float sizeY = 3.0f; 
		static float left = -sizeX * 0.5f;
		static float bottom = -sizeY * 0.5f;
		static float dx =  sizeX / float(image->getWidth());  
		static float dy =  sizeY / float(image->getHeight());
	
		return Vec3f(left + x * dx, bottom + y * dy, z).normalize();
	}


public:
	SimpleRayTracer(Scene * scene, Image * image) {
		this->scene = scene;
		this->image = image;
	}

	void searchClosestHit(const Ray & ray, HitRec & hitRec) {
		for (int i = 0; i < scene->spheres.size(); i++) {
			scene->spheres[i].hit(ray, hitRec);			
		}
	}

	void fireRays(void) { 
		Ray ray;
		HitRec hitRec;
		//bool hit = false;
		//ray.o = Vec3f(0.0f, 0.0f, 0.0f); //Set the start position of the eye rays to the origin
		ray.o = cam.position; 

		for (int y = 0; y < image->getHeight(); y++) {
			for (int x = 0; x < image->getWidth(); x++) {
				ray.d = getEyeRayDirection(x, y);
				hitRec.anyHit = false;
				searchClosestHit(ray, hitRec);
				if (hitRec.anyHit) {

					Vec3f color = scene->spheres[hitRec.primIndex].color(hitRec, light, cam);
				 
					image->setPixel(x, y, color);
					glSetPixel(x, y, color);
				} else {
					// background black
					image->setPixel(x, y, Vec3f(0.0f, 0.0f, 0.0f));
					glSetPixel(x, y, Vec3f(0.0f, 0.0f, 0.0f));
				}
			}
		}
	}
};


SimpleRayTracer * rayTracer;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glTranslatef(cam.position.x,cam.position.y,cam.position.z);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	rayTracer->fireRays();
	
	max(0.0f, 1.0f);
	glFlush();
}

void changeSize(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glViewport(0,0,w,h);
}

void keypress(unsigned char key, int x, int y)
{
	switch(key) {
		case 'x':
			cam.position.x += 1.0f;
			break;
		case 'X':
			cam.position.x -= 1.0f;
			break;
			
		case 'y':
			cam.position.y += 1.0f;
			break;
		case 'Y':
			cam.position.y -= 1.0f;
			break;
			
		case 'z':
			cam.position.z += 1.0f;
			break;
		case 'Z':
			cam.position.z -= 1.0f;
			break;
		case 'Q':
		case 'q':
			glutLeaveMainLoop();
			break;

	}

	glutPostRedisplay();
}

void init(void)
{
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("SimpleRayTracer");
	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(keypress);

	// camera position
	
	cam.position = Vec3f(0.0f, 0.0f, 0.0f);

	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

	Scene * scene = new Scene;
	scene->add(Sphere(Vec3f(0.0f, 0.0f, -10.0f), 2.0f, 32.f, Vec3f(1.0f, 0.5f, 0.31f), Vec3f(1.0f, 0.5f, 0.31f), Vec3f(0.5f, 0.5f, 0.5f)));

	light.position = cam.position;
	light.Ia = Vec3f(0.2f, 0.2f, 0.2f);
	light.Id = Vec3f(0.5f, 0.5f, 0.5f);
	light.Is = Vec3f( 1.0f, 1.0f, 1.0f);

	Image * image = new Image(640, 480);	
	
	rayTracer = new SimpleRayTracer(scene, image);

}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	init();
	glutMainLoop();

	return 0;
}
