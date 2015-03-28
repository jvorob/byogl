#include <iostream>
#include <fstream>
#include <cstdlib>

#include "drawing.h"
#include "util.h"

using std::cout;
using std::cerr;
using std::endl;

PointZ::PointZ(int X, int Y, double Z): x(X), y(Y), z(Z)
{}

PointZ::PointZ() {}


//===================
//Surface Definitions

	Surface::Surface(int width, int height) {
		this->width = width;
		this->height = height;

		raster = new Pixel[width * height];
		zbuf = new double[width * height];
	}

	Surface::~Surface() {
		delete raster;
		delete zbuf;
	}

	void Surface::clear(char c) {//Fills raster with given char
		std::memset(raster, c, raster_size());

		for(int i = 0; i < width * height; i++) {
			zbuf[i] = -10000;
		}
	}

	void Surface::clear(char r, char g, char b) {
		for(int i = 0; i < width * height; i++) {
			raster[i].col[C_R] = r;
			raster[i].col[C_G] = g;
			raster[i].col[C_B] = b;
			zbuf[i] = -1000;
		}
	}

	inline Surface::Pixel *Surface::index(int x, int y) {
		if(x < 0 || y < 0 || x >= width || y >= height) {
			//cerr << "ArrayIndexOutOfBoundsException: " << printf("%d,%d", x, y) << endl;
			//exit(-1);
			return NULL;
		}
		return raster + (width * y + x);
	}

	inline int Surface::indexI(int x, int y) {
		if(x < 0 || y < 0 || x >= width || y >= height) {
			//cerr << "ArrayIndexOutOfBoundsException: " << printf("%d,%d", x, y) << endl;
			//exit(-1);
			return -1;
		}
		return (width * y + x);
	}

	int Surface::raster_size() {
		return width * height * sizeof(Surface::Pixel);
	}


	void Surface::write_to_file(const char *filename) {
		std::ofstream fs;
		fs.open(filename);
		fs << "P6 " << width << " " << height << " 255\n";

		fs.write((char *)raster, raster_size());
		fs.close();
	}

//End Surface Definitions
//=======================


//====================
//Graphics Definitions

	Graphics::Graphics(Surface *target) { //Inits graphics object pointing to target
		this->target = target;
		setColor(0, 0, 0);

		xmin = 0;
		ymin = 0;
		xmax = target->width;
		ymax = target->height;
	}

	inline void Graphics::setColor(char newR, char newG, char newB) {
		r = newR;
		g = newG;
		b = newB;
	}

	inline void Graphics::setPixel(Surface::Pixel* pixel) {
		if(!pixel)
			return;
		pixel->col[C_R] = r;
		pixel->col[C_G] = g;
		pixel->col[C_B] = b;
	}

	inline void Graphics::setPixelZ(int x, int y, double z) {
		int i = target->indexI(x, y);

		if(i == -1) {
			//cerr << "Setting invalid pixel\n";
			return;
		}

		if(target->zbuf[i] > z) {
			return;
		}
		
		//Only draw it if my z is greater (i.e. closer to the camera at 0);


		target->zbuf[i] = z;

		Surface::Pixel *pixel = target->raster + i;
		pixel->col[C_R] = r;
		pixel->col[C_G] = g;
		pixel->col[C_B] = b;
	}

	void Graphics::drawLine(int x1, int y1, int x2, int y2) {
		CohenSutherlandLineClipAndDraw(x1, y1, x2, y2);
	}

	//THIS PART RIPPED WHOLESALE FROM WIKIPEDIA
		 
		// Compute the bit code for a point (x, y) using the clip rectangle
		// bounded diagonally by (xmin, ymin), and (xmax, ymax)
		 
		// ASSUME THAT xmax, xmin, ymax and ymin are global constants.
		 
		int Graphics::ComputeOutCode(int x, int y)
		{
			int code;
		 
			code = INSIDE;          // initialised as being inside of clip window
		 
			if (x < xmin)           // to the left of clip window
				code |= LEFT;
			else if (x > xmax)      // to the right of clip window
				code |= RIGHT;
			if (y < ymin)           // below the clip window
				code |= BOTTOM;
			else if (y > ymax)      // above the clip window
				code |= TOP;
		 
			return code;
		}
		 
		// Cohen–Sutherland clipping algorithm clips a line from
		// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
		// diagonal from (xmin, ymin) to (xmax, ymax).
		void Graphics::CohenSutherlandLineClipAndDraw(int x0, int y0, int x1, int y1)
		{
			// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
			int outcode0 = ComputeOutCode(x0, y0);
			int outcode1 = ComputeOutCode(x1, y1);
			bool accept = false;
		 
			while (true) {
				if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
					accept = true;
					break;
				} else if (outcode0 & outcode1) { // Bitwise AND is not 0. Trivially reject and get out of loop
					break;
				} else {
					// failed both tests, so calculate the line segment to clip
					// from an outside point to an intersection with clip edge
					int x, y;
		 
					// At least one endpoint is outside the clip rectangle; pick it.
					int outcodeOut = outcode0 ? outcode0 : outcode1;
		 
					// Now find the intersection point;
					// use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
					if (outcodeOut & TOP) {           // point is above the clip rectangle
						x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
						y = ymax;
					} else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
						x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
						y = ymin;
					} else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
						y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
						x = xmax;
					} else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
						y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
						x = xmin;
					}
		 
					// Now we move outside point to intersection point to clip
					// and get ready for next pass.
					if (outcodeOut == outcode0) {
						x0 = x;
						y0 = y;
						outcode0 = ComputeOutCode(x0, y0);
					} else {
						x1 = x;
						y1 = y;
						outcode1 = ComputeOutCode(x1, y1);
					}
				}
			}
			if (accept) {
				drawLinePreProc(x0, y0, x1, y1);
			}
		}
	//END OF WIKI CODE

	//Quadrants:
	/*
		-------|------- +y
	      Q4  .|.  Q1     
	         . | .        
	        .Q3|Q2.       
	       .   |   .      
	           +x          
	*/
	void Graphics::drawLinePreProc(int x1, int y1, int x2, int y2) {
		if(y2 < y1) {
			drawLinePreProc(x2, y2, x1, y1);
			return;
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

	//Z buffered versions of all line drawing
	void Graphics::drawLineZ(int x1, int y1, double z1, int x2, int y2, double z2) {
		CohenSutherlandLineClipAndDrawZ(x1, y1, z1, x2, y2, z2);
	}

	//THIS PART RIPPED WHOLESALE FROM WIKIPEDIA
		 
		// Compute the bit code for a point (x, y) using the clip rectangle
		// bounded diagonally by (xmin, ymin), and (xmax, ymax)
		 
		// ASSUME THAT xmax, xmin, ymax and ymin are global constants.
		 
		 
		// Cohen–Sutherland clipping algorithm clips a line from
		// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
		// diagonal from (xmin, ymin) to (xmax, ymax).
		void Graphics::CohenSutherlandLineClipAndDrawZ(int x0, int y0, double z0, int x1, int y1, double z1)
		{
			// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
			int outcode0 = ComputeOutCode(x0, y0);
			int outcode1 = ComputeOutCode(x1, y1);
			bool accept = false;
		 
			while (true) {
				if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
					accept = true;
					break;
				} else if (outcode0 & outcode1) { // Bitwise AND is not 0. Trivially reject and get out of loop
					break;
				} else {
					// failed both tests, so calculate the line segment to clip
					// from an outside point to an intersection with clip edge
					int x, y;
					double z;
		 
					// At least one endpoint is outside the clip rectangle; pick it.
					int outcodeOut = outcode0 ? outcode0 : outcode1;
		 
					// Now find the intersection point;
					// use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
					if (outcodeOut & TOP) {           // point is above the clip rectangle
						x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
						y = ymax;
						z = z0 + (z1 - z0) * (ymax - y0) / (y1 - y0);
					} else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
						x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
						y = ymin;
						z = z0 + (z1 - z0) * (ymax - y0) / (y1 - y0);
					} else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
						y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
						x = xmax;
						z = z0 + (z1 - z0) * (xmax - x0) / (x1 - x0);
					} else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
						y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
						x = xmin;
						z = z0 + (z1 - z0) * (xmax - x0) / (x1 - x0);
					}
		 
					// Now we move outside point to intersection point to clip
					// and get ready for next pass.
					if (outcodeOut == outcode0) {
						x0 = x;
						y0 = y;
						z0 = z;
						outcode0 = ComputeOutCode(x0, y0);
					} else {
						x1 = x;
						y1 = y;
						z1 = z;
						outcode1 = ComputeOutCode(x1, y1);
					}
				}
			}
			if (accept) {
				drawLinePreProcZ(x0, y0, z0, x1, y1, z1);
			}
		}
	//END OF WIKI CODE

	//Quadrants:
	/*
		-------|------- +y
	      Q4  .|.  Q1     
	         . | .        
	        .Q3|Q2.       
	       .   |   .      
	           +x          
	*/
	void Graphics::drawLinePreProcZ(int x1, int y1, double z1, int x2, int y2, double z2) {
		if(y2 < y1) {
			drawLinePreProcZ(x2, y2, z2, x1, y1, z1);
			return;
		}

		int dx = x2 - x1;
		int dy = y2 - y1;
		//dy always nonnegative

		//Trivial lines
		if(dx == 0) {
			//dy always positive
			for(int i = y1; i <= y1 + dy; i++)
				setPixelZ(x1, i, lerp(z1, z2, (double)(i - y1) / dy));
			return;
		}
		if(dy == 0) {
			//dx can be negative
			if(dx > 0) 
				for(int i = x1; i <= x1 + dx; i++)
					setPixelZ(i, y1, lerp(z1, z2, (double)(i - x1) / dx));
			else 
				for(int i = x1; i >= x1 + dx; i--)
					setPixelZ(i, y1, lerp(z1, z2, (double)(i - x1) / dx));
			return;
		}


		//Sloped lines
		if(dx > 0) {
			if(dy > dx) {
				drawLineQ2Z(x1, y1, dx, dy, z1, z2);
			} else {
				drawLineQ1Z(x1, y1, dx, dy, z1, z2);
			}
		} else {
			if(dy > -1 * dx) {
				drawLineQ3Z(x1, y1, dx, dy, z1, z2);
			} else {
				drawLineQ4Z(x1, y1, dx, dy, z1, z2);
			}
		}
		return;
	}


	void Graphics::drawLineQ1Z(int x1, int y1, int dx, int dy, double z1, double z2) {
		int offset = dx / dy / 2;
		int error = offset;
		for(int i = x1; i <= x1 + dx; i++) {
			error += dy;
			if(error > dx ) {
				y1++;
				error -= dx;
			}

			setPixelZ(i, y1, lerp(z1, z2, (double)(i - x1) / dx));
		}
	}

	void Graphics::drawLineQ2Z(int x1, int y1, int dx, int dy, double z1, double z2) {
		int offset = dx / dy / 2;
		int error = offset;
		for(int i = y1; i <= y1 + dy; i++) {
			error += dx;
			if(error > dy ) {
				x1++;
				error -= dy;
			}
			setPixelZ(x1, i, lerp(z1, z2, (double)(i - y1) / dy));
		}
	}

	void Graphics::drawLineQ3Z(int x1, int y1, int dx, int dy, double z1, double z2) {
		int offset = dx / dy / 2;
		int error = offset;
		for(int i = y1; i <= y1 + dy; i++) {
			error -= dx;
			if(error > dy ) {
				x1--;
				error -= dy;
			}
			setPixelZ(x1, i, lerp(z1, z2, (double)(i - y1) / dy));
		}
	}

	void Graphics::drawLineQ4Z(int x1, int y1, int dx, int dy, double z1, double z2) {
		int offset = dx / dy / 2;
		int error = offset;
		for(int i = x1; i >= x1 + dx; i--) {
			error -= dy;
			if(error < dx ) {
				y1++;
				error -= dx;
			}
			setPixelZ(i, y1, lerp(z1, z2, (double)(i - x1) / dx));
		}
	}

	int Graphics::leftBound() {return 0;}
	int Graphics::rightBound() {return target->width - 1;}
	int Graphics::topBound() {return 0;}
	int Graphics::bottomBound() {return target->height - 1;}

	//Triangles
	void Graphics::fillTri(PointZ a, PointZ b, PointZ c) {
		PointZ h, m, l;
		
		if(a.z > -0.8)
			return;
		if(b.z > -0.8)
			return;
		if(c.z > -0.8)
			return;

		//Sort points by y
		if(a.y < b.y) {
			if(a.y < c.y) {
				h = a;
				if(b.y < c.y) {
					m = b;
					l = c;
				} else {
					m = c;
					l = b;
				}
			} else {
				h = c;
				m = a;
				l = b;
			}
		} else {
			if(b.y < c.y) {
				h = b;
				if(a.y < c.y) {
					m = a;
					l = c;
				} else {
					m = c;
					l = a;
				}
			} else {
				h = c;
				m = b;
				l = a;
			}
		}
		//Done sorting

		if(h.y == m.y && m.y == l.y) {
			//degenerate tri
			//should do something, but not now
			//cerr <<"Degenerate triangle\n";
			return;
		}

		if(h.y == m.y) { //is flat-topped
			PointZ tl, tr;
			if(h.x < m.x) {
				tl = h;
				tr = m;
			} else {
				tl = m;
				tr = h;
			}

			fillTrap(tl.y, l.y, tl.x, l.x - tl.x, tr.x, l.x - tr.x,
				tl.z, tr.z, l.z, l.z);
			return;
		}
		if(m.y == l.y) { //is flat-bottomed
			PointZ bl, br;
			if(m.x < l.x) {
				bl = m;
				br = l;
			} else {
				bl = l;
				br = m;
			}

			fillTrap(h.y, bl.y, h.x, bl.x - h.x, h.x, br.x - h.x,
				h.z, h.z, bl.z, br.z);
			return;
		}

		int dxl, dxt, dxb; //deltax long, deltax top, deltax bot
		int dyl, dyt, dyb;

		dxl = l.x - h.x;
		dxt = m.x - h.x;
		dxb = b.x - m.x;

		dyl = l.y - h.y;
		dyt = m.y - h.y;
		dyb = b.y - m.y;

		PointZ m2 (h.x + dxl * dyt / dyl , h.y + dyt, lerp(h.z, l.z, (double)dyt / dyl));

		PointZ ml, mr; //mid left, mid right
	
		if(m2.x < m.x) {
			ml = m2;
			mr = m;
		} else {
			ml = m;
			mr = m2;
		}


		//Top triangle
		fillTrap(h.y, m.y, h.x, ml.x - h.x, h.x, mr.x - h.x, 
				h.z, h.z, ml.z, mr.z);

		//Bottom triangle
		fillTrap(ml.y, l.y, ml.x, l.x - ml.x, mr.x, l.x - mr.x,
				ml.z, mr.z, l.z, l.z);
	}

//top y, bot y, x left(top), delta x left (top->bottom), x right (top), delta x right
void Graphics::fillTrap(int ty, int by, int xl, int dxl, int xr, int dxr,
							double ztl, double ztr, double zbl, double zbr) {
	int dy = by - ty;

	int offl = dxl / dy / 2;
	int offr = dxr / dy / 2;

	int errl = offl;
	int errr = offr;

	int udxl = (dxl > 0) ? 1 : -1;
	int udxr = (dxr > 0) ? 1 : -1;
	int pdxl = dxl > 0 ? dxl : -1 * dxl;
	int pdxr = dxr > 0 ? dxr : -1 * dxr;

	double zl, zr;


	//Loops over scanlines
	for(int i = ty; i <= ty + dy; i++) {
		zl = lerp(ztl, zbl, (double)(i - ty) / dy);
		zr = lerp(ztr, zbr, (double)(i - ty) / dy);

		if(pdxl < dy) { //Sloping more downwards
			errl += pdxl;
			if(errl > dy ) {
				xl += udxl;
				errl -= dy;
			}
		} else { //sloping more horizontally
			while(errl >= pdxl) {
				errl -= dy;
				xl += udxl;
			}
			errl += pdxl;
		}

		if(pdxr < dy) { //Sloping more downwards
			errr += pdxr;
			if(errr > dy ) {
				xr += udxr;
				errr -= dy;
			}
		} else { //sloping more horizontally
			while(errr >= pdxr) {
				errr -= dy;
				xr += udxr;
			}
			errr += pdxr;
		}

		//Loops over x'es in scanline
		for(int j = xl; j <= xr; j++) {
			double zcurr = lerp(zl, zr, (double)(j - xl) / (xr - xl + 1));
			//setColor(0, 0, (int)(zcurr * 150) %256);
			//setPixel(target->index(j, i));
			setPixelZ(j, i, zcurr);
		}
	}
}

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

