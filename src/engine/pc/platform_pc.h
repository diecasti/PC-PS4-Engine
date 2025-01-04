#pragma once
#ifndef PLATFORMPC_H
#define PLATFORMPC_H


#include <memory>
#include <string>
#include <vector>
#include "SDLEventListener.h"

class PlatformPC {
public:
	PlatformPC();
	~PlatformPC();

	static PlatformPC* Instance();

	/// <summary>
	/// Inicializacion de la instancia Platform
	/// </summary>
	/// <returns> False si falla alguna inicializacion, True en caso contrario </returns>
	static bool Init();
	//Cierra la instancia y cierra SDL
	static void Release();

	// Gestión de eventos con SDL
	// @returns True si se debe continuar la ejecucion, False de recibir un caso que termine la ejecucion
	bool tick();

	// @returns Milisegundos desde la inicialización de SDL
	int chrono();

	// @returns Puntero al fichero abierto
	FILE* openFile(const std::string path);

	int randomInteger(int lower, int upper);
	float randomRange(float lower, float upper);

	// Añade el Listener de PC
	void addListener(SDLEventListener* iL);

	// Quita del vector de listeners 
	void removeObserver(SDLEventListener* iL);

private:
	static std::unique_ptr<PlatformPC> instance;

	// Listeneras activos en PC
	std::vector<SDLEventListener*> listeners;

	/// <summary>
	/// Inicializacion privada de SDL
	/// </summary>
	/// <returns>True si todo va bien, False si falla alguna inicializacion</returns>
	bool initPlatform();
	void initRandomSeed();
	// Cierre de SDL
	void closePlatform();

	void sendEvent(SDL_Event evt);
};
#endif // PLATFORMPC_H