#pragma once
#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

class SDL_Texture;
//class SDL_Renderer;
class RendererPC;

class Image {
	friend RendererPC;

public:
	/// <summary>
	/// Crea una imagen con los parametros dados
	/// </summary>
	/// <param name="w"> ancho de la imagen </param>
	/// <param name="h"> alto de la imagen </param>
	/// <param name="text"> tesxtura SDL de la imagen </param>
	Image(uint32_t w, uint32_t h, SDL_Texture* text);

	// Destruye la textura SDL de la imagen
	~Image();

	uint32_t getWidth() const;
	uint32_t getHeight() const;

protected:
	uint32_t t_w, t_h;
	SDL_Texture* text;
};
#endif // IMAGE_H