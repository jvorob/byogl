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

class Surface {
	public:
		int width, height;
		const int bpp = 24;
		char *raster;

		Surface(int width, int height);
		~Surface();

		void clear(char c);//Fills raster with given char
		void clear(char r, char g, char b);

		char *index(int x, int y); //Indexes into the raster

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
		inline void setPixel(char* pixel);
		void drawLine(int x1, int y1, int x2, int y2);

		int leftBound();
		int rightBound();
		int topBound();
		int bottomBound();
		
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
};

void make_hsv_to_rgb(int *ptr); //converts the pixel being pointed to

#endif
