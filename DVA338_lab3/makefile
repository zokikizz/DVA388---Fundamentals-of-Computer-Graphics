cc:
	g++ -fopenmp -lm -o raytrace -O3 *.cpp -lGL  -lgomp -lGLU -lglut

clean:
	rm -f ./raytrace

run:
	./raytrace

install:
	sudo apt-get install libglew-dev freeglut3-dev build-essential
