#pragma once
#ifndef MISILESSCENE_H
#define MISILESSCENE_H

#include <vector>
#include <string>

#include <queue>

#include "messages.h"
#include "userid_def.h"



#define WINDOW_NAME "Missiles"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080


#define NUM_CIRCLES 5
#define NUM_MISSILES 5
#define NUM_CIUDADES 5
#define NUM_TORRETAS 4

#define NUM_FILES 3

#define BACKGROUND_COLOR 0xFF202020
#define FIXED_DELTA_TIME 16 // 33 ms => 1/30 s; 16 ms => 1/60 s
#define REFRESH_CHECK 5000 // ms

#define RESET_TIME 3000


//frames que la logica haya adelantado máximos, si el renderThread tiene estos (o mas lo cual no deberia), la logica se mantendra en espera Activa
//preguntando constantemente, si puede continuar o no, ya que es un videojuego no nos resulta ningun problmea que ocurra esto ya que queremos ir lo mas rápido posible, de esta forma logramos evitar la latencia
#define FRAME_WAIT 4


class Image;
class GameObject;
class SpriteSheet;
class Ciudad;
class Missile;
class Torreta;
class MissilePlayer;
class Cursor;

class RenderThread;


class MisilesScene {

	//RenderObject se utiliza para poder ordenar y elegir que objetos se deben renderizar y en que orden (render layers)
	// Contiene un mensaje de renderizado de un objeto, y la capa en la que se debe renderizar
	struct RenderObject
	{
		Message message;
		int layer;
		RenderObject(const Message& m, const int& l) : message(m), layer(l) {};
		bool operator<(const RenderObject& other) const
		{
			// La comparación se hace al revés (>) para que las capas más bajas se pinten primero
			return layer > other.layer;
		}
	};



public:
	MisilesScene();
	~MisilesScene() {};

	/*
	* Inicialización de los singletons
	* @returns False si ha habido algún error en el Init de los singletons, True en caso contrario
	*/
	bool setup();

	/// <summary>
	/// Carga de los assets y objetos de la escena
	/// </summary>
	/// <returns>False si ha habido algun problema en la lectura o inicializacion de los objetos, True en caso contrario</returns>
	bool loadScene();

	//Bucle principal
	void run();
	//destruccion y lioberacion de memoria de los objetos y assets de la escena
	void unloadScene();
	//Liberacion de las instancias PLatform y Renderer
	void shutdown();

	/// <summary>
/// Coloca un mensaje para renderizar un objeto en una lista ordenada segun su renderLayer
/// </summary>
/// <param name="m"> Mensaje del tipo con la informacion que se quiera renderizar </param>
/// <param name="l"> Capa de renderizado, las imagenes con un valor mayor se renderizaran encima, cosulta las capas existentes en render_layers.h </param>
	void renderObject(const Message m, const int& l);

protected:
	std::vector<GameObject*> objs;
	std::vector<const Image*> images;
	std::vector<Ciudad*> cities;
	std::vector<Missile*> missiles;
	std::vector<MissilePlayer*> playerMissiles;
	std::vector<Torreta*> turrets;
	std::vector<Cursor*> cursores;
	SpriteSheet* ciudadSheet;
	SpriteSheet* turretsSheet;
	SpriteSheet* cursoresSheet;

	int lastFrameTime, accFrameTime, refreshFrameTime, countFrames;



	RenderThread* renderThread;

	// Crea los GameObjects de la escena
	void initSceneGOs();
	//Resigna las torretas a los usuarios
	void reasingTurrets();

	/// <summary>
/// Carga una imagen y la añade la lista de imagenes (images)
/// </summary>
/// <param name="filepath"> Ruta de la imagen a leer </param>
/// <returns>True si la lectura es correcta, False si la lectura falla</returns>
	bool loadImage(std::string filepath);

	/// <summary>
/// Recorre la lsita de objetos en busca de aquellos marcados para destruirse
/// </summary>
	void checkDestroy();

	/// <summary>
	/// Ejecuta el render de todos los objetos, colocando un mensaje ordenado por su renderLayer dependiendo del obejto,
	///  luego los manda para que el renderthread lo pueda procesar
	/// </summary>
	void renderFromQueue();

	void checkCollisions();

private:
	bool toReset;
	float timerToReset;

	// Cola de objetos a renderizar en este frame, ordenados por capa de render
	std::priority_queue<RenderObject> objsToRender;


	//Para el reinicio de la escena cuando todas las ciudades se han destruido
	Ciudad* lastCity;
	int citiesStanding;
	int turretsStanding;
	void resetScene();
};
#endif // MISILESSCENE_H