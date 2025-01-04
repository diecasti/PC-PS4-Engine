#include "image.h"

#include <SDL.h>
#include <iostream>

// asumimos pixeles en ARGB siempre, recibe un puntero a un array de pixeles, la plataforma no libera ese espacio, es responsabilidad de la lógica
Image::Image(uint32_t w, uint32_t h, SDL_Texture* text_) : t_w(w), t_h(h), text(text_) {}

Image::~Image()
{
	SDL_DestroyTexture(text);
	text = nullptr;
}

uint32_t Image::getWidth() const {
	return t_w;
}

uint32_t Image::getHeight() const {
	return t_h;
}