#ifndef DRAWING_H
#define DRAWING_H

#define MAX_HSV 1536

#include <cstring>


enum e_color {
	C_R = 0, C_G, C_B
};

enum e_hsv {
	C_H = 0, C_S, C_V
};

class PointZ {
	public: 
		int x; 
		int y; 
		double z;

		PointZ(int X, int Y, double Z);
		PointZ();
};

class Surface {
	public:
		static const int bytesPerPixel = 3;

		struct Pixel {
			char col[bytesPerPixel];
		};

		int width, height;
		Pixel *raster;
		double *zbuf;

		Surface(int width, int height);
		~Surface();

		void clear(char c);//Fills raster with given char
		void clear(char r, char g, char b);

		Pixel *index(int x, int y); //Indexes into the raster
		int indexI(int x, int y); //gives an integer index

		int raster_size();

		void write_to_file(const char *filename); //Writes rgb data to file
	
};

class Graphics {
	Surface *target;
	char r;
	char g;
	char b;

	public:
		Graphics(Surface *target); //Inits graphics object pointing to target

		void setColor(char r, char g, char b);
		inline void setPixel(Surface::Pixel* pixel);
		inline void setPixelZ(int x, int y, double z);
		void drawLine(int x1, int y1, int x2, int y2);
		void drawLineZ(int x1, int y1, double z1, int x2, int y2, double z2);


		int leftBound();
		int rightBound();
		int topBound();
		int bottomBound();
		
		void fillTri(PointZ a, PointZ b, PointZ c);
	private:
		const int INSIDE = 0; // 0000
		const int LEFT = 1;   // 0001
		const int RIGHT = 2;  // 0010
		const int BOTTOM = 4; // 0100
		const int TOP = 8;    // 1000

		int xmin, xmax, ymin, ymax;
		int ComputeOutCode(int x, int y);
		void CohenSutherlandLineClipAndDraw(int x0, int y0, int x1, int y1);
 

		void drawLinePreProc(int x1, int y1, int x2, int y2);
		void drawLineQ1(int x1, int y1, int dx, int dy);
		void drawLineQ2(int x1, int y1, int dx, int dy);
		void drawLineQ3(int x1, int y1, int dx, int dy);
		void drawLineQ4(int x1, int y1, int dx, int dy);

		void CohenSutherlandLineClipAndDrawZ(int x0, int y0, double z0, int x1, int y1, double z1);
		void drawLinePreProcZ(int x1, int y1, double z1, int x2, int y2, double z2);
		void drawLineQ1Z(int x1, int y1, int dx, int dy, double z1, double z2);
		void drawLineQ2Z(int x1, int y1, int dx, int dy, double z1, double z2);
		void drawLineQ3Z(int x1, int y1, int dx, int dy, double z1, double z2);
		void drawLineQ4Z(int x1, int y1, int dx, int dy, double z1, double z2);

		//Fills a trapezoid
		//top y, bot y, x left(top), delta x left (top->bottom), x right (top), delta x right
		//z parameters are z values at corners
		void fillTrap(int ty, int by, int xl, int dxl, int xr, int dxr, 
				double ztl, double ztr, double zbl, double zbr);
};

void make_hsv_to_rgb(int *ptr); //converts the pixel being pointed to

#endif
