#include <iostream>
#include <fstream>

#include <cmath>
#include <cstdlib>
#include <cstdio>

#include "drawing.h"

#define MAX_HSV 1536
#define PI 3.14159265358979323846264
#define TWOPI (2 * PI)

using namespace std;

double max(double a, double b) {
	if(a < b) 
		return b;
	else 
		return a;
}
double min(double a, double b) {
	if(a < b) 
		return a;
	else 
		return b;
}


int main() {
	int width = 1000;
	int height = 1000;

	Surface s (width, height);

	s.clear(255);


	Graphics g (&s);

	//Draw circle
	{
		double x1, y1, x2, y2;
		double lastX, lastY;
		int cx = 500, cy = 500;//center
		double r1 = 100; //radius
		double extendRatio = 5; //radius

		int numsteps = 10;

		//Set up previous segment
		{
			int i = numsteps - 1;
			double t1 = i * TWOPI / numsteps;
			double t2 = (i + 1) * TWOPI / numsteps;

			x1 = r1 * cos(t1) + cx;
			y1 = r1 * sin(t1) + cy;
			double tx = r1 * cos(t2) + cx;
			double ty = r1 * sin(t2) + cy;

			x2 = tx + (tx - x1) * extendRatio;
			y2 = ty + (ty - y1) * extendRatio;

			lastX = x2;
			lastY = y2;
		}

		for(int i = 0; i < numsteps; i++) {
			double t1 = i * TWOPI / numsteps;
			double t2 = (i + 1) * TWOPI / numsteps;

			x1 = r1 * cos(t1) + cx;
			y1 = r1 * sin(t1) + cy;
			double tx = r1 * cos(t2) + cx;
			double ty = r1 * sin(t2) + cy;

			x2 = tx + (tx - x1) * extendRatio;
			y2 = ty + (ty - y1) * extendRatio;

			g.drawLine(x1, y1, x2, y2);

			g.drawLine(x2, y2, lastX, lastY);

			lastX = x2;
			lastY = y2;
		}
		
	}

	s.write_to_file("out.ppm");
	cout << "Done" << endl;
}
