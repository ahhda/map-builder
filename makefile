all: grid.o map.o
	g++ -g -o driver grid.o map.o driver.cpp `pkg-config --libs opencv`

grid.o: grid.cpp grid.h
	g++ -g -c grid.cpp

map.o: map.cpp map.h
	g++ -g -c map.cpp

clean:
	rm -rf driver
	rm -rf grid.o
	rm -rf map.o
