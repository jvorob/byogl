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

		virtual void gainFocus() {};
		virtual void loseFocus() {};
};

class Label : public Widget {
	public:
		Label(SDL_Point p, const std::string& s);

		void resize();
		void setText(const std::string &s);

		void draw(SDL_Renderer *r);

	protected:
		const int padding = 2;
		const int minHeight = Font::charh + 2 * padding;

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
	protected:
		const int padding = 8;
		const int minHeight = Font::charh + 2 * padding;

		enum State { UP, DOWN };
		State state;

		std::string text;
		ButtonHandler* bh;
		
};

class TextBox : public Widget {
	public:
		TextBox(SDL_Point p, const std::string& s);

		void setText(const std::string &s);
		std::string getText();

		void doEvent(SDL_Event e);
		void draw(SDL_Renderer *r);
		void gainFocus();
		void loseFocus();

	protected:
		const int padding = 4;
		const int minHeight = Font::charh + 2 * padding;

		int cursorpos;

		std::string text;
};

class NumBox : public TextBox {
	public:
		NumBox(SDL_Point p, double d);

		double getNum();
		void setNum(double d);

		void gainFocus();
		void loseFocus();

		void doEvent(SDL_Event e);
	protected:
		double num;
		//Either sets number from inputed text or resets to last valid value
		void validate();
};

#endif
