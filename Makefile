LIBS = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib `agar-config --libs`
INCLUDES = -I/usr/local/include `agar-config --cflags`



CXX_FLAGS = -c $(INCLUDES) -std=c++11

OBJECTS = main.o drawing.o mesh.o camera.o matrix.o util.o \
	ui_agar.o ui_sdl.o entity.o

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

camera.o: camera.cpp camera.h mesh.h drawing.h matrix.h util.h
	g++ camera.cpp $(CXX_FLAGS)

matrix.o: matrix.cpp matrix.h util.h
	g++ matrix.cpp $(CXX_FLAGS)

util.o: util.cpp util.h
	g++ util.cpp $(CXX_FLAGS)

ui_sdl.o: ui_sdl.cpp ui.h drawing.h util.h
	g++ ui_sdl.cpp $(CXX_FLAGS)

ui_agar.o: ui_agar.cpp ui.h drawing.h util.h
	g++ ui_agar.cpp $(CXX_FLAGS)

entity.o: entity.cpp entity.h matrix.h util.h
	g++ entity.cpp $(CXX_FLAGS)
