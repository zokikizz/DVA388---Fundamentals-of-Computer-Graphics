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
int ray_count = 0;


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
		float min = ray.tMax;
		HitRec temp;
		for (int i = 0; i < scene->spheres.size(); i++) {
			temp.anyHit = false;
			scene->spheres[i].hit(ray, temp);

			if (temp.anyHit && temp.tHit < min)
			{
				min = temp.tHit;
				hitRec = temp;
				hitRec.primIndex = i;
			}
		}
	}

	Vec3f reflection(const Ray & ray, HitRec & hr, int count) {
		
		ray_count++;

		if(count <= 3)
		{
			// hit
			Vec3f l = (light.position - hr.p).normalize();
			Vec3f r = (hr.n * (l.dot(hr.n)) * 2.0f - l).normalize();
			Ray rr;
			HitRec hrr;
			rr.o = hr.p;
			rr.epsMoveStartAlongSurfaceNormal(hr.n);
			rr.d = r;
			hrr.anyHit = false;
			searchClosestHit(rr,hrr);

			if(hrr.anyHit)
			{
				Sphere s = scene->spheres[hrr.primIndex];
				Vec3f color = Vec3f(0.0f,0.0f,0.0f);
				Ray rrr;
				HitRec hrrr;
				rrr.o = hrr.p;
				rrr.epsMoveStartAlongSurfaceNormal(hrr.n);
				rrr.d = (light.position - rrr.o).normalize();
				hrrr.anyHit = false;
				searchClosestHit(rrr,hrrr);

				if(!hrrr.anyHit)
					color += Phong(rr,hrr);

				return color + reflection(rr, hrr, count + 1) * pow(max(ray.d.dot(r), 0.0f), s.shininess * 128.0f);
			}
			else
			{
				return Vec3f(0.0f, 0.0f, 0.0f);
			}

		}
		else // lighting(ray, hr);
		{
			Vec3f color = light.Ia.multCoordwise(scene->spheres[hr.primIndex].Ka);
			Ray rr;
			HitRec hrr;
			rr.o = hr.p;
			rr.epsMoveStartAlongSurfaceNormal(hr.n);
			rr.d = (light.position - rr.o).normalize();
			hrr.anyHit = false;
			searchClosestHit(rr, hrr);

			if(hrr.anyHit)
				color += Phong(ray,hr);

			return color;
		}
	}


	Vec3f Phong(const Ray & ray, HitRec & hr)
	{
		Sphere s = scene->spheres[hr.primIndex];
		Vec3f l = (light.position - hr.p).normalize();
		Vec3f r = ((hr.n * (l.dot(hr.n) * 2.0f)) - l).normalize();
		Vec3f v = (cam.position - hr.p).normalize();
		
		Vec3f diffuse = (light.Id.multCoordwise(s.Kd * max(hr.n.dot(l), 0.0f)));
		Vec3f specular = (light.Is.multCoordwise(s.Ks * pow(max(r.dot(v), 0.0f), s.shininess)));

		return diffuse + specular;
	}

	Vec3f lighting(const Ray & ray, HitRec hr)
	{
		Ray shadowr;
		HitRec shadow;
		shadowr.o = hr.p;
		shadowr.epsMoveStartAlongSurfaceNormal(hr.n);
		shadowr.d = (light.position - shadowr.o).normalize();
		shadow.anyHit = false;
		searchClosestHit(shadowr, shadow);

		Vec3f color = light.Ia.multCoordwise(scene->spheres[hr.primIndex].Ka);
		ray_count++;
	
		if (!shadow.anyHit)
			color += Phong(ray, hr);
		
		color += reflection(ray,hr, 1);
		return color;
	}

	void fireRays(void) {
		Ray ray;
		HitRec hitRec;
		//bool hit = false;
		ray.o = cam.position; //Set the start position of the eye rays to the origin

		for (int y = 0; y < image->getHeight(); y++)
		{
			for (int x = 0; x < image->getWidth(); x++)
			{
				ray.d = getEyeRayDirection(x, y);
				hitRec.anyHit = false;
				searchClosestHit(ray, hitRec);
				Vec3f color = Vec3f(0.0f, 0.0f, 0.0f);
				ray_count++;

				if (hitRec.anyHit) {
					color += lighting(ray, hitRec);
					image->setPixel(x, y, color);
					glSetPixel(x, y, color);
				}
				else
				{
					image->setPixel(x, y, Vec3f(0,0,0));
					glSetPixel(x, y, Vec3f(0,0,0));
			
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

	ray_count = 0;
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

		case 'a':
			light.position.x += 1.0f;
			break;
		case 'd':
			light.position.x -= 1.0f;
			break;
		
		case 'w':
			light.position.z += 1.0f;
			break;
		case 's':
			light.position.z -= 1.0f;
			break;
	
		case 'r':
			light.position.z += 1.0f;
			break;
		case 'e':
			light.position.z -= 1.0f;
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
	scene->add(Sphere(Vec3f(-1.0f, -1.0f, -8.0f), 1.0f, 32.f, Vec3f(1.0f, 0.5f, 0.31f), Vec3f(1.0f, 0.5f, 0.31f), Vec3f(0.5f, 0.5f, 0.5f)));

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
