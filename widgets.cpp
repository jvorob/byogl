#include "widgets.h"

#include <string>
#include <stdexcept>

//Font
SDL_Texture* Font::font = NULL;

void Font::init(SDL_Renderer *r) {
	if(font) {
		std::cerr << "Double init of font\n";
		return;
	}

	SDL_Surface *surf = SDL_LoadBMP(fontFile);
	if(!surf)
		printSDLError();

	font = SDL_CreateTextureFromSurface(r, surf);
	if(!font)
		printSDLError();
}

void Font::drawString(SDL_Renderer *r, SDL_Point p, const std::string& s) {
	SDL_Rect src, dst;

	src.x = 0;
	src.y = 0;
	src.w = charw;
	src.h = charh;

	dst.x = p.x;
	dst.y = p.y;
	dst.w = charw;
	dst.h = charh;

	for(const char *cs = s.c_str(); *cs; cs++) {
		src.x = charw * (*cs / 16);
		src.y = charh * (*cs % 16);

		SDL_RenderCopy(r, font, &src, &dst);

		dst.x += charw;
	}

}

int Font::renderedWidth(const std::string& s) {
	return charw * s.size();
}

//Widgets =======

//Label
Label::Label(SDL_Point p, const std::string& s) {
	text = std::string(s);

	bounds.x = p.x;
	bounds.y = p.y;
	bounds.h = minHeight;

	resize();
}

void Label::resize() {
	bounds.w = Font::renderedWidth(text) + 2 * padding;
}

void Label::setText(const std::string &s) {
	text = std::string(s);
}

void Label::draw(SDL_Renderer *r) {
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);

	Font::drawString(r, {bounds.x + padding, bounds.y + padding}, text);
	//SDL_RenderDrawRect(r, &bounds);
}

//Button
Button::Button(SDL_Point p, const std::string& s) {
	text = std::string(s);

	bounds.x = p.x;
	bounds.y = p.y;
	bounds.w = Font::renderedWidth(text) + 2 * padding;
	bounds.h = minHeight;

	state = UP;
}

void Button::setHandler(ButtonHandler *b) {
	bh = b;
}

void Button::doEvent(SDL_Event e) {
	switch (e.type) {
		case SDL_MOUSEMOTION:
			SDL_Point p;
			p.x = e.motion.x;
			p.y = e.motion.y;

			if(!SDL_PointInRect(&p, &bounds))
				state = UP;
			break;
		case SDL_MOUSEBUTTONDOWN:
			state = DOWN;
			break;
		case SDL_MOUSEBUTTONUP:
			if(state == DOWN) {
				state = UP;
				bh->handleButton(this);
			}
			break;
		case SDL_WINDOWEVENT:
			switch (e.window.event) {
				case SDL_WINDOWEVENT_ENTER:
					break;
				case SDL_WINDOWEVENT_LEAVE:
					state = UP;
					break;
			}
			break;
		default:
			//if(DEBUGPRINT) 
				fprintf(stderr, "UNKNOWN EVENT TYPE\n");
			break;
	}

}

void Button::draw(SDL_Renderer *r) {

	if(state == UP) {
		SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
	} else {
		SDL_SetRenderDrawColor(r, 150, 150, 150, 255);
	}


	SDL_RenderFillRect(r, &bounds);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawRect(r, &bounds);
	Font::drawString(r, {bounds.x + padding, bounds.y + padding}, text);
}

//TextBox
TextBox::TextBox(SDL_Point p, const std::string& s) {
	text = std::string(s);

	bounds.x = p.x;
	bounds.y = p.y;
	bounds.w = 300;//Font::renderedWidth(text) + 2 * padding;
	bounds.h = minHeight;
	
	cursorpos = -1;
}

void TextBox::setText(const std::string &s) {
	text = std::string(s);
}

std::string TextBox::getText() {
	return std::string(text);
}

void TextBox::doEvent(SDL_Event e) {
	switch (e.type) {
		case SDL_MOUSEBUTTONDOWN:
			cursorpos = (e.button.x - bounds.x - padding + (Font::charw / 2)) / Font::charw;
			if(cursorpos > text.length())
				cursorpos = text.length();
			if(cursorpos < 0)
				cursorpos = 0;
			break;
		case SDL_TEXTEDITING:
			std::cerr << "Editing: " << e.edit.text << '\n';
			break;
		case SDL_TEXTINPUT:
			//std::cerr << "Input: " << e.text.text << '\n';
			text.insert(cursorpos, e.text.text);
			cursorpos++;
			break;
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym) {
				case SDLK_DELETE:
					//Don't do anything if at end
					if(cursorpos == text.length())
						break;
						
					//Otherwise move right and backspace
					cursorpos++;
					//NOTE: Not breaking
				case SDLK_BACKSPACE:
					if(cursorpos) {
						if(cursorpos == -1)
							std::cerr << "Something went horribly wrong with textbox events\n";

						cursorpos--;
						text.erase(cursorpos, 1);
					}
					break;
				case SDLK_LEFT:
					if(cursorpos)
						cursorpos--;
					break;
				case SDLK_RIGHT:
					if(cursorpos != text.length())
						cursorpos++;
					break;
			}
			break;
	}
}

void TextBox::draw(SDL_Renderer *r) {
	if(cursorpos != -1) {
		SDL_SetRenderDrawColor(r, 200, 200, 200, 255);
		SDL_RenderFillRect(r, &bounds);
	}

	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	Font::drawString(r, {bounds.x + padding, bounds.y + padding}, text);

	SDL_RenderDrawRect(r, &bounds);

	if(cursorpos != -1) {
		SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
		SDL_RenderDrawLine(r, 
				bounds.x + padding + cursorpos * Font::charw, 
				bounds.y + padding + Font::charh, 
				bounds.x + padding + (cursorpos + 1) * Font::charw, 
				bounds.y + padding + Font::charh);
	}
}

void TextBox::gainFocus() {
	SDL_StartTextInput();
	cursorpos = 0;
}

void TextBox::loseFocus() {
	SDL_StopTextInput();
	cursorpos = -1;
}

//Numbox

NumBox::NumBox(SDL_Point p, double d) :
	TextBox(p, "") {

	bounds.w = 100;
	
	setNum(d);
}

double NumBox::getNum() {
	return num;
}

void NumBox::setNum(double d) {
	num = d;
	text = std::to_string(d);
}

void NumBox::gainFocus() {
	SDL_StartTextInput();
	cursorpos = 0;
}

void NumBox::loseFocus() {
	SDL_StopTextInput();
	cursorpos = -1;

	validate();
}

void NumBox::doEvent(SDL_Event e) {
	TextBox::doEvent(e);

	if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
		validate();
}

void NumBox::validate() {
	double temp;
	try {
		temp = std::stod(text);
	} catch (const std::invalid_argument &ia) {
		setNum(num);
		return;
	}

	setNum(temp);
}
