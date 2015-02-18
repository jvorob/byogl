all: display

a.out: drawing.o main.o mesh.o
	g++ $^ -o $@

main.o: main.cpp drawing.h
	g++ main.cpp -c
	
drawing.o: drawing.cpp drawing.h
	g++ drawing.cpp -c

mesh.o: mesh.cpp mesh.h
	g++ mesh.cpp -c


run: out.ppm

out.ppm: a.out
	./a.out

display: out.ppm
	display out.ppm

time: a.out
	time -v ./a.out

clean:
	rm a.out *.o out.ppm
