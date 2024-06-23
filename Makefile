CC = g++
CFLAGS = -Wall -g -std=c++17

main: main.o 
	$(CC) $(CFLAGS) -o main main.o

main.o: main.cc vec3.h color.h ray.h rtweekend.h sphere.h hittable_list.h hittable.h interval.h 
	$(CC) $(CFLAGS) -c main.cc

.PHONY: clean
clean:
	rm main *.o
