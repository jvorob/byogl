#include <iostream>
#include <fstream>
#include <cstdlib>

#include "drawing.h"

using std::cout;
using std::cerr;
using std::endl;


//===================
//Surface Definitions

	Surface::Surface(int width, int height) {
		this->width = width;
		this->height = height;

		raster = new char[raster_size()];
	}

	Surface::~Surface() {
		delete raster;
	}

	void Surface::clear(char c) {//Fills raster with given char
		std::memset(raster, c, raster_size());
	}

	void Surface::clear(char r, char g, char b) {
		for(int i = 0; i < raster_size(); i += 3) {
			raster[i + C_R] = r;
			raster[i + C_G] = g;
			raster[i + C_B] = b;
		}
	}

	inline char *Surface::index(int x, int y) {
		if(x < 0 || y < 0 || x >= width || y >= height) {
			//cerr << "ArrayIndexOutOfBoundsException: " << printf("%d,%d", x, y) << endl;
			//exit(-1);
			return NULL;
		}
		return raster + (3 * (width * y + x));
	}

	int Surface::raster_size() {
		return width * height * 3;
	}


	void Surface::write_to_file(const char *filename) {
		std::ofstream fs;
		fs.open(filename);
		fs << "P6 " << width << " " << height << " 255\n";

		fs.write(raster, raster_size());
		fs.close();
	}

//End Surface Definitions
//=======================


//====================
//Graphics Definitions

	Graphics::Graphics(Surface *target) { //Inits graphics object pointing to target
		this->target = target;
		setColor(0, 0, 0);
	}

	inline void Graphics::setColor(char newR, char newG, char newB) {
		r = newR;
		g = newG;
		b = newB;
	}

	inline void Graphics::setPixel(char* pixel) {
		if(!pixel)
			return;
		pixel[C_R] = r;
		pixel[C_G] = g;
		pixel[C_B] = b;
	}

	void Graphics::drawLine(int x1, int y1, int x2, int y2) {
		if(y2 < y1) {
			drawLine(x2, y2, x1, y1);
			return;
		}

		if(x1 < leftBound()) {
			if(x2 < leftBound())
				return;
			int leftIntercept = y1 + ((double)(leftBound() - x1) / (x2 - x1)) * (y2 - y1);
			if(leftIntercept >= topBound() && leftIntercept <= bottomBound()) {
				x1 = leftBound();
				y1 = leftIntercept;
			}
		}

		int dx = x2 - x1;
		int dy = y2 - y1;
		//dy always nonnegative

		//Trivial lines
		if(dx == 0) {
			//dy always positive
			for(int i = y1; i <= y1 + dy; i++)
				setPixel(target->index(x1, i));
			return;
		}
		if(dy == 0) {
			//dx can be negative
			if(dx > 0) 
				for(int i = x1; i <= x1 + dx; i++)
					setPixel(target->index(i, y1));
			else 
				for(int i = x1; i >= x1 + dx; i--)
					setPixel(target->index(i, y1));
			return;
		}


		//Sloped lines
		if(dx > 0) {
			if(dy > dx) {
				drawLineQ2(x1, y1, dx, dy);
			} else {
				drawLineQ1(x1, y1, dx, dy);
			}
		} else {
			if(dy > -1 * dx) {
				drawLineQ3(x1, y1, dx, dy);
			} else {
				drawLineQ4(x1, y1, dx, dy);
			}
		}
		return;

	}


	//Quadrants:
	/*
		-------|------- +y
	      Q4  .|.  Q1     
	         . | .        
	        .Q3|Q2.       
	       .   |   .      
	           +x          
	*/

	void Graphics::drawLineQ1(int x1, int y1, int dx, int dy) {
		int offset = dx / dy / 2;
		int error = offset;
		for(int i = x1; i <= x1 + dx; i++) {
			error += dy;
			if(error > dx ) {
				y1++;
				error -= dx;
			}
			setPixel(target->index(i, y1));
		}
	}

	void Graphics::drawLineQ2(int x1, int y1, int dx, int dy) {
		int offset = dx / dy / 2;
		int error = offset;
		for(int i = y1; i <= y1 + dy; i++) {
			error += dx;
			if(error > dy ) {
				x1++;
				error -= dy;
			}
			setPixel(target->index(x1, i));
		}
	}

	void Graphics::drawLineQ3(int x1, int y1, int dx, int dy) {
		int offset = dx / dy / 2;
		int error = offset;
		for(int i = y1; i <= y1 + dy; i++) {
			error -= dx;
			if(error > dy ) {
				x1--;
				error -= dy;
			}
			setPixel(target->index(x1, i));
		}
	}

	void Graphics::drawLineQ4(int x1, int y1, int dx, int dy) {
		int offset = dx / dy / 2;
		int error = offset;
		for(int i = x1; i >= x1 + dx; i--) {
			error -= dy;
			if(error < dx ) {
				y1++;
				error -= dx;
			}
			setPixel(target->index(i, y1));
		}
	}

	int Graphics::leftBound() {return 0;}
	int Graphics::rightBound() {return target->width - 1;}
	int Graphics::topBound() {return 0;}
	int Graphics::bottomBound() {return target->height - 1;}

//End Graphics Definitions
//========================



//First sets rgb by hues
//Then scales down by value
//Then scales up by saturation
void make_hsv_to_rgb(int *ptr) { //converts the pixel being pointed to
	int hue = ptr[C_H];
	int sat = ptr[C_S];
	int val = ptr[C_V];
	if(hue < 0 || hue >= MAX_HSV) {
		cerr << "Invalid hue" << hue << endl;
		hue %= MAX_HSV;
	}
	if(sat < 0 || sat > 255) {
		cerr << "Invalid saturation" << endl;
		sat %= 255;
	}

	for(int col = 0; col < 3; col++) {

		//Set Hue
		{
			int sextant = hue / 256;//0-5
			int remainder = hue % 256;

			//Red rises in sextant 4
			//Green rises in sextant 0
			//Blue rises in sextant 2
			sextant -= (col + 2) * 2;
			sextant += 12;
			sextant %= 6;

			switch (sextant) {
				case 0: {	//rises
					ptr[col] = remainder;
				} break;
				case 1:		//high
				case 2: {
					ptr[col] = 255;
				} break;
				case 3: {	//falls
					ptr[col] = 255 - remainder;
				} break;
				case 4:
				case 5: {	//low
					ptr[col] = 0;
				} break;
			}
		}

		//Adjust for Value and saturation
		{
			ptr[col] = ptr[col] * val / 256;


			if(val != 0) {
				ptr[col] = val - ptr[col];
				ptr[col] = ptr[col] * sat / 256;
				ptr[col] = val - ptr[col];
			}
		}
	}
}

