#include "widgets.h"


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

