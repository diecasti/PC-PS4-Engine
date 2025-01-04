#include "renderer_pc.h"

#include <iostream>

#include <SDL.h>
#include "image.h"

std::unique_ptr<RendererPC> RendererPC::instance = nullptr;

RendererPC::RendererPC() = default;

RendererPC::~RendererPC() { closeWindow(); }

RendererPC* RendererPC::Instance() {
	return instance.get();
}

bool RendererPC::Init(const std::string name, int winWidth, int winHeight) {
	instance.reset(new RendererPC());

	if (!instance.get()->initRender(name, winWidth, winHeight)) {
		instance.reset(nullptr);
		return false;
	}

	return true;
}

void RendererPC::Release() {
	if (instance.get() == nullptr)
		return;
	//delete instance.get();

	instance.reset(nullptr);
}

void RendererPC::clear(uint32_t color) {
	uint8_t a = (color & 0xFF000000) >> 24, 
		r = (color & 0x00FF0000) >> 16, 
		g = (color & 0x0000FF00) >> 8, 
		b = color & 0x000000FF;
	SDL_SetRenderDrawColor(sdlRenderer, r, g, b, a);
	SDL_RenderClear(sdlRenderer);
}

void RendererPC::present() {
	SDL_RenderPresent(sdlRenderer);
}

void RendererPC::putPixel(int x, int y, uint32_t color) {
	uint8_t prev_r, prev_g, prev_b, prev_a;
	SDL_GetRenderDrawColor(sdlRenderer, &prev_r, &prev_g, &prev_b, &prev_a);

	uint8_t a = (color & 0xFF000000) >> 24,
		r = (color & 0x00FF0000) >> 16,
		g = (color & 0x0000FF00) >> 8,
		b = color & 0x000000FF;
	SDL_SetRenderDrawColor(sdlRenderer, r, g, b, a);
	SDL_RenderDrawPoint(sdlRenderer, x, y);
	SDL_SetRenderDrawColor(sdlRenderer, prev_r, prev_g, prev_b, prev_a);
}

void RendererPC::drawCircle(int x, int y, int radius, uint32_t color)
{
	if (radius == 1) {
		putPixel(x, y, color);
		return;
	}
	int xAct = radius;
	int yAct = 0;
	// dAct contiene la resta de 1/4 que lo convertiría en hAct, de esta manera evitamos usar numeros reales
	int dAct = 1 - radius; 


	while (xAct >= yAct) {
		// Método pintar 8 puntos simétricos
		drawInnerCircle(x, y, xAct, yAct, color);

		++yAct;

		if (dAct < 0) // Midpoint dentro o justo en la circunferencia, elegimos NORTE dsgi = dact +2y +3
			dAct = dAct + 2 * yAct + 3;

		else // Midpoint fuera de la circunferencia, elegimos NOROESTE dsig = dact + 2y -2x + 1
		{
			--xAct;
			dAct = dAct + 2 * yAct - 2 * xAct + 1;
		}
	}
}

void RendererPC::drawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
	uint8_t a = (color & 0xFF000000) >> 24,
		r = (color & 0x00FF0000) >> 16,
		g = (color & 0x0000FF00) >> 8,
		b = color & 0x000000FF;
	SDL_SetRenderDrawColor(sdlRenderer, r, g, b, a);
	SDL_RenderDrawLine(sdlRenderer, x0, y0, x1, y1);
}

Image* RendererPC::createImage(uint32_t w, uint32_t h, uint32_t* pxls)
{

	// Inicializacion en dos pasos porque las funciones de SDL pueden fallar

	Uint32 rmask = 0x00FF0000, gmask = 0x0000FF00, bmask = 0x000000FF, amask = 0xFF000000;

	SDL_Surface* surf = SDL_CreateRGBSurfaceFrom(pxls, w, h, 32, 4 * w, rmask, gmask, bmask, amask);

	if (!surf) {
		std::cerr << "Error al crear RGBSurface de SDL\n";
		return nullptr;
	}


	SDL_Texture* text = SDL_CreateTextureFromSurface(sdlRenderer, surf);

	if (!text) {
		std::cerr << "Error al crear la textura a partir de una Surface de SDL\n";
		SDL_FreeSurface(surf);
		return nullptr;
	}

	SDL_FreeSurface(surf);

	return new Image(w, h, text);
}

void RendererPC::drawImage(const Image* image, int sx1, int sy1, int sx2, int sy2, int tx1, int ty1, int tx2, int ty2)
{
	SDL_Rect src = { sx1, sy1, sx2, sy2 };
	SDL_Rect dst = { tx1, ty1, tx2, ty2 };
	SDL_RenderCopy(sdlRenderer, image->text, &src, &dst);
}

const int RendererPC::getHeight() {
	return windowHeight;
}

const int RendererPC::getWidth() {
	return windowWidth;
}


void RendererPC::closeWindow() {
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(sdlWindow);

	sdlWindow = nullptr; sdlRenderer = nullptr;
}

void RendererPC::drawInnerCircle(int x, int y, int xAct, int yAct, uint32_t color)
{
	uint8_t a = (color & 0xFF000000) >> 24,
		r = (color & 0x00FF0000) >> 16,
		g = (color & 0x0000FF00) >> 8,
		b = color & 0x000000FF;
	SDL_SetRenderDrawColor(sdlRenderer, r, g, b, a);

	SDL_RenderDrawLine(sdlRenderer, x - yAct, y + xAct, x + yAct, y + xAct);
	SDL_RenderDrawLine(sdlRenderer, x - xAct, y + yAct, x + xAct, y + yAct);
	SDL_RenderDrawLine(sdlRenderer, x - xAct, y - yAct, x + xAct, y - yAct);
	SDL_RenderDrawLine(sdlRenderer, x - yAct, y - xAct, x + yAct, y - xAct);
}



void RendererPC::drawRect(int x, int y, uint32_t width, uint32_t height, uint32_t color) {
	uint8_t a = (color & 0xFF000000) >> 24,
		r = (color & 0x00FF0000) >> 16,
		g = (color & 0x0000FF00) >> 8,
		b = color & 0x000000FF;
	SDL_SetRenderDrawColor(sdlRenderer, r, g, b, a);
	SDL_Rect rect = { x, y, (int)width, (int)height };
	SDL_RenderFillRect(sdlRenderer, &rect);
}



bool RendererPC::initRender(std::string name, int winWidth, int winHeight) {

	Uint32 flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
	sdlWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight, flags);
	if (!sdlWindow) {
		std::cerr << SDL_GetError();
		return false;
	}
	//ENABLES/DISABLES VSYNC VERY IMPORTANT
	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, VSYNC ? SDL_RENDERER_PRESENTVSYNC : 0);
	if (!sdlRenderer) {
		SDL_DestroyWindow(sdlWindow);

		sdlWindow = nullptr;
		std::cerr << SDL_GetError();
		return false;
	}

	SDL_ShowCursor(false);

	windowWidth = winWidth;
	windowHeight = winHeight;




	return true;


}