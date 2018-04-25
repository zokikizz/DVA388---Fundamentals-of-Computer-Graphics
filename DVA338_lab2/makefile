cc:
	mkdir -p -m 700 /dev/shm/DVA338_bin
	g++ -std=c++11 -O3 -c -o /dev/shm/DVA338_bin/algebra.o algebra.cpp
	g++ -std=c++11 -O3 -c -o /dev/shm/DVA338_bin/mesh.o mesh.cpp
	g++ -std=c++11 -O3 -o /dev/shm/DVA338_bin/main /dev/shm/DVA338_bin/*.o main.cpp -lGLEW -lglut -lGL

clean:
	rm -f /dev/shm/DVA338_bin/*

run:
	/dev/shm/DVA338_bin/main

install:
	sudo apt-get install libglew-dev freeglut3-dev build-essential
