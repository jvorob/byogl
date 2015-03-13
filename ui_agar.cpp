#include "ui.h"
#include <iostream>

UI_Agar::UI_Agar(Surface *s) {
	//Init sdl, window, renderer
	if (AG_InitCore(NULL, 0) == -1 ||
		AG_InitGraphics(NULL) == -1)
		//AG_InitVideoSDL(s, 0) == -1)
			return;

	surf = s;

	win = AG_WindowNew(AG_WINDOW_MAIN);

	AG_LabelNew(win, 0, "Shenanigans");

	AG_Box *boxbox = AG_BoxNewHoriz(win, 0);

	AG_Box *transbox = AG_BoxNewVert	(boxbox, 0);
	AG_Box *scalebox = AG_BoxNewVert	(boxbox, 0);
	AG_Box *rotbox = AG_BoxNewVert		(boxbox, 0);

	transform[3] = 1;
	scale[3] = 1;
	rotate[3] = 1;

	scale[0] = 1;
	scale[1] = 1;
	scale[2] = 1;

	AG_LabelNew(transbox, 0, "Translate");
	AG_NumericalNewDbl	(transbox, 0, NULL, "X", &transform[0]);
	AG_NumericalNewDbl	(transbox, 0, NULL, "Y", &transform[1]);
	AG_NumericalNewDbl	(transbox, 0, NULL, "Z", &transform[2]);

	AG_LabelNew(scalebox, 0, "Scale");
	AG_NumericalNewDbl	(scalebox, 0, NULL, "X", &scale[0]);
	AG_NumericalNewDbl	(scalebox, 0, NULL, "Y", &scale[1]);
	AG_NumericalNewDbl	(scalebox, 0, NULL, "Z", &scale[2]);

	AG_LabelNew(rotbox, 0, "Rotate");
	AG_NumericalNewDbl	(rotbox, 0, NULL, "X", &rotate[0]);
	AG_NumericalNewDbl	(rotbox, 0, NULL, "Y", &rotate[1]);
	AG_NumericalNewDbl	(rotbox, 0, NULL, "Z", &rotate[2]);

	renBuffer = AG_SurfaceFromPixelsRGB (
		surf->raster, 
		surf->width, 
		surf->height, 
		surf->bpp, 
		0xFF0000,
		0x00FF00,
		0x0000FF);
	/*
	pixBuffer = AG_SurfaceFromPixelsRGB (
		surf->raster, 
		surf->width, 
		surf->height, 
		surf->bpp, 
		0xFF0000,
		0x00FF00,
		0x0000FF);
	*/
	//pixBuffer = AG_SurfaceDup(renBuffer);

	boop(surf->width);
	boop(surf->bpp);

	pixfrmt = AG_PixelFormatRGBA(
			32, 
			0xFF000000, 
			0x00FF0000, 
			0x0000FF00, 
			0x00000000);

	pixBuffer = AG_SurfaceConvert(renBuffer, pixfrmt);

	//AG_Box *pixbox = AG_BoxNewHoriz(win, AG_BOX_FRAME);
	pixmap = AG_PixmapFromSurfaceNODUP(win, 0, pixBuffer);
	//pixmap = AG_PixmapFromSurfaceNODUP(pixbox, AG_PIXMAP_RESCALE, pixBuffer);
	//pixmap = AG_PixmapFromSurfaceNODUP(pixbox, 0, pixBuffer);
	//pixmap = AG_PixmapFromSurface(pixbox, 0, pixBuffer);

	//AG_WidgetSetSize(pixmap, 800, 800);

	boop(pixBuffer->w);
	boop(pixBuffer->h);

	AG_WindowSetGeometry(win, 0,0,800,1000);
	AG_WindowUpdate(win);
	AG_WindowShow(win);

}

void UI_Agar::draw() {
	//Grab rendered data
	AG_SurfaceFree(renBuffer);
	renBuffer = AG_SurfaceFromPixelsRGB (
		surf->raster, 
		surf->width, 
		surf->height, 
		surf->bpp, 
		0xFF0000,
		0x00FF00,
		0x0000FF);

	//Copy it to pixmap
	AG_SurfaceCopy(pixBuffer, renBuffer);
	AG_PixmapUpdateSurface(pixmap, pixmap->n);
	
	//AG_WindowUpdate(win);
	//AG_Redraw(win);
}

void UI_Agar::cleanup() {
	AG_DestroyGraphics();
	AG_Destroy();
}

void UI_Agar::mainloop() {
	AG_EventLoop();
}

void UI_Agar::addTimedEvent(Uint32 (*fn)(AG_Timer *, AG_Event *)) {
	timer = AG_AddTimerAuto (NULL, 20, fn, "");
} 
