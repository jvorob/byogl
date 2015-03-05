SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/local/include

CXX_FLAGS = -c $(SDL_INCLUDE) -std=c++11



all: run

a.out: drawing.o main.o mesh.o camera.o matrix.o util.o ui.o entity.o
	g++ $^ $(SDL_LIB) -o $@



main.o: main.cpp drawing.h mesh.h util.h
	g++ main.cpp $(CXX_FLAGS)
	
drawing.o: drawing.cpp drawing.h util.h
	g++ drawing.cpp $(CXX_FLAGS)

mesh.o: mesh.cpp mesh.h matrix.h util.h
	g++ mesh.cpp $(CXX_FLAGS)

camera.o: camera.cpp camera.h mesh.h drawing.h matrix.h util.h
	g++ camera.cpp $(CXX_FLAGS)

matrix.o: matrix.cpp matrix.h util.h
	g++ matrix.cpp $(CXX_FLAGS)

util.o: util.cpp util.h
	g++ util.cpp $(CXX_FLAGS)

ui.o: ui.cpp ui.h drawing.h util.h
	g++ ui.cpp $(CXX_FLAGS)

entity.o: entity.cpp entity.h matrix.h util.h
	g++ entity.cpp $(CXX_FLAGS)

run: a.out
	./a.out

out.ppm: a.out
	./a.out

display: out.ppm
	display out.ppm

time: a.out
	time -v ./a.out

clean:
	rm a.out *.o out.ppm
