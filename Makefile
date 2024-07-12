CC = g++ 
CFLAGS = -Wall -g -std=c++17

main: main.o 
	$(CC) $(CFLAGS) -o main main.o -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a


main.o: main.cc vec3.h color.h ray.h rtweekend.h sphere.h hittable_list.h hittable.h interval.h camera.h material.h bvh.h aabb.h 
	$(CC) $(CFLAGS) -c main.cc

.PHONY: clean
clean:
	rm main *o
