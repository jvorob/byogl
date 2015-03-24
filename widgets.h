#ifndef WIDGETS_H
#define WIDGETS_H

#include <SDL2/SDL.h>
#include <iostream>

#include "util.h"

class Font {
	public:
		static void init(SDL_Renderer *r);
		
		static void drawString(SDL_Renderer *r, SDL_Point p, const std::string& s);

		static int renderedWidth(const std::string& s);

		static const int charw = 8;
		static const int charh = 8;
	private:
		static constexpr const char* const fontFile = "font.bmp";
		static SDL_Texture *font;
};

class Widget {
	public:

		SDL_Rect bounds;


		virtual void doEvent(SDL_Event e) {};
		virtual void draw(SDL_Renderer *r) {};
};

class Label : public Widget {
	public:
		Label(SDL_Point p, const std::string& s);

		void resize();
		void setText(const std::string &s);

		void draw(SDL_Renderer *r);

	private:
		const int minHeight = 12;
		const int padding = 2;

		std::string text;
};

class Button;
class ButtonHandler {
	public:
		virtual void handleButton(Button *) = 0;
};

class Button : public Widget {
	public:
		Button(SDL_Point p, const std::string& s);

		void setHandler(ButtonHandler *);

		void doEvent(SDL_Event e);
		void draw(SDL_Renderer *r);
	private:
		const int padding = 8;
		const int minHeight = Font::charh + 2 * padding;

		enum State { UP, DOWN };
		State state;

		std::string text;
		ButtonHandler* bh;
		
};

#endif
