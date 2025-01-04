#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "input_status.h"
#include "SDLEventListener.h"


#include "SDL_gamecontroller.h"
#include "userid_def.h"

// usamos este valor para intentar "aproximar" la cantidad de movimiento percibida por el ratón
// con aquella recibida por el giroscopio en el módulo de PS4. Y diremos, ¿como hemos hayado 
// semejante valor arbitrario?. Pues probando a ver cual se sentia bien //esto venia del año pasado, reinventar la rueda no hace falta
#define DEADZONE 0.1 

//Este valor sirve para hacer de forma relativamente aproximada el movimiento del raton con el de los joysticks del mando
#define MOUSE_RELATIVE (0.06)


//class SDL_GameController;
class InputKeyboard;
class InputKeyboardMouse;
class InputGamepad;


class InputPC : public SDLEventListener
{
public:
	InputPC();
	~InputPC();

	static InputPC* Instance();

	/// <summary>
	/// Inicializacion de la instancia Input
	/// </summary>
	/// <returns> False si falla alguna inicializacion, True en caso contrario </returns>
	static bool Init();
	//Cierra la instancia
	static void Release();

	/// Gestión del input con SDL
	/// Actualiza el movimiento del mouse y abre o cierra los puertos de los conectados y desconectados
	void tick();


	// Procesamiento de eventos con SDL, se mandan a las clases de input correspondientes
	virtual void processEvent(SDL_Event evt) override;

	/// <summary>
	/// Método a usar por el juego para obtener el input de un usuario
	/// </summary>
	/// <returns> InputStatus del usuario id </returns>
	const InputStatus getStatus(UserId id);

	friend class InputKeyboard;
	friend class InputKeyboardMouse;
	friend class InputGamepad;

private:
	// Clases separadas que gestionan el input
	InputKeyboard* iKey;
	InputKeyboardMouse* iMouse;
	InputGamepad* iGamepad;


	static std::unique_ptr<InputPC> instance;


	//struct con la informacion que contiene Input, es exclusiov de aqui y no se va autilizar en otra parte
	struct UserIdInputInfo {
		SDL_JoystickID id;
		SDL_GameController* controller = nullptr;
	};

	// Mapa de los Inputstatus, lo rellenan las clases amigas y lo pide la lógica
	std::unordered_map<UserId, InputStatus> usersInput;

	// Mapa para guardar información del controller
	std::unordered_map<UserId, UserIdInputInfo> usersInputInfo;


	/// <summary>
	/// Inicializacion privada del input de SDL
	/// </summary>
	/// <returns>True si todo va bien, False si falla alguna inicializacion</returns>
	bool initInput();

	// Procesa los eventos de teclado mandándolos a la clase correspondiente
	void processKeyboardEvents(SDL_Event e);


	// Métodos privados para abrir y cerrar los puertos de los controllers
	void closePorts(SDL_GameController* controller);
	bool openPorts(UserId userId);
	

	// Método interno para limpiar memoria de la instancia
	void closeInput();
};

