#pragma once
#ifndef RENDERERPC_H
#define RENDERERPC_H

#include <memory>
#include <string>

#define VSYNC true

class Image;
class SDL_Window;
class SDL_Renderer;
class SDL_Texture;

class RendererPC {
public:
	RendererPC();
	~RendererPC();

	static RendererPC* Instance();

	/// <summary>
	/// Crea el la instancia del Renderer de SDL con los parametros otorgados
	/// </summary>
	/// <param name="name"> Nombre de la ventana</param>
	/// <param name="winWidth"> Ancho de la ventana </param>
	/// <param name="winHeight"> Alto de la ventana </param>
	/// <returns> False en caso de fallar en alguna inicializacion, True en caso contrario </returns>
	static bool Init(const std::string name, int winWidth, int winHeight);

	// libera la instancia de Renderer
	static void Release();

	// limpia de forma eficiente el buffer de renderizado dado el color base
	//color esta representado en ARGB
	void clear(uint32_t color);

	// presenta el frame actual por pantalla
	void present();

	void putPixel(int x, int y, uint32_t color);
	// Dibuja un círculo relleno con midpoint circle algorithm
	void drawCircle(int x, int y, int radius, uint32_t color);
	// Dibuja línea con SDL
	void drawLine(int x0, int y0, int x1, int y1, uint32_t color);
	// Método para renderizar rectangulos
	void drawRect(int x, int y, uint32_t width, uint32_t height, uint32_t color);
	// Dibuja una imagen con los rectangulos source y texture proporcionados
	void drawImage(const Image* image, int sx1, int sy1, int sx2, int sy2, int tx1, int ty1, int tx2, int ty2);

	/// <summary>
	///  Crea una ImagenPS4 dado un array de Pixeles
	/// </summary>
	/// <param name="w"> ancho de la imagen </param>
	/// <param name="h"> alto de la imagen </param>
	/// <param name="pxls"> --> Puntero al array de pixeles que constituyen al imagen, se presume en ARGB. createImage() no gestiona este puntero, recuerden liberar el puntero de pixeles posterior a esta funcion </param>
	/// <returns> Punto a Imagen de haberse creado correctamente, nullptr de fallar (metodos internos SDL)</returns>
	Image* createImage(uint32_t w, uint32_t h, uint32_t* pxls);

	const int getHeight();
	const int getWidth();

private:
	static std::unique_ptr<RendererPC> instance;

	SDL_Window* sdlWindow;
	SDL_Renderer* sdlRenderer;

	int windowWidth, windowHeight;

	/// <summary>
	/// Crea el las instancias Window y Renderer del Renderer de SDL con los parametros otorgados
	/// </summary>
	/// <param name="name"> Nombre de la ventana</param>
	/// <param name="winWidth"> Ancho de la ventana </param>
	/// <param name="winHeight"> Alto de la ventana </param>
	/// <returns> False en caso de fallar en alguna inicializacion, True en caso contrario </returns>
	bool initRender(std::string name, int winWidth, int winHeight);

	// Destruye sdlWindow y sdlRenderer
	void closeWindow();

	// Método de ayuda a drawCircle
	void drawInnerCircle(int x, int y, int xAct, int yAct, uint32_t color);
};
#endif // RENDERERPC_H