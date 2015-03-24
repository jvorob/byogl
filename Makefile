LIBS = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib 
INCLUDES = -I/usr/local/include 



CXX_FLAGS = -c $(INCLUDES) -std=c++11

OBJECTS = main.o drawing.o mesh.o camera.o matrix.o util.o \
	ui_sdl.o entity.o world.o widgets.o

OUTFILE = a.out


default: a.out

run: $(OUTFILE)
	./$(OUTFILE)

clean:
	rm $(OBJECTS) $(OUTFILE)



$(OUTFILE): $(OBJECTS);
	g++ $(OBJECTS) $(LIBS)

main.o: main.cpp drawing.h mesh.h util.h
	g++ main.cpp $(CXX_FLAGS)
	
drawing.o: drawing.cpp drawing.h util.h
	g++ drawing.cpp $(CXX_FLAGS)

mesh.o: mesh.cpp mesh.h matrix.h util.h
	g++ mesh.cpp $(CXX_FLAGS)

camera.o: camera.cpp camera.h mesh.h drawing.h matrix.h util.h world.h
	g++ camera.cpp $(CXX_FLAGS)

matrix.o: matrix.cpp matrix.h util.h
	g++ matrix.cpp $(CXX_FLAGS)

util.o: util.cpp util.h
	g++ util.cpp $(CXX_FLAGS)

ui_sdl.o: ui_sdl.cpp ui.h drawing.h util.h widgets.h
	g++ ui_sdl.cpp $(CXX_FLAGS)

entity.o: entity.cpp entity.h matrix.h util.h
	g++ entity.cpp $(CXX_FLAGS)

world.o: world.cpp world.h mesh.h camera.h util.h
	g++ world.cpp $(CXX_FLAGS)

widgets.o: widgets.cpp widgets.h
	g++ widgets.cpp $(CXX_FLAGS)
