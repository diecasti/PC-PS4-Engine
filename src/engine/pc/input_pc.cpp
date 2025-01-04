#include "input_pc.h"

#include <SDL.h>
#include <iostream>
#include "platform.h"
#include "user_service_pc.h"
#include "login_type.h"
#include "InputKeyboard.h"
#include "InputKeyboardMouse.h"
#include "InputGamepad.h"

std::unique_ptr<InputPC> InputPC::instance = nullptr;

InputPC::InputPC() = default;

InputPC::~InputPC() {
	closeInput();
}

InputPC* InputPC::Instance() {
	return instance.get();
}

bool InputPC::Init() {
	instance.reset(new InputPC());


	return instance.get()->initInput();
}

void InputPC::Release() {
	//El release llama al delete

	if (instance.get() == nullptr)
		return;

	instance.reset(nullptr);
}

void InputPC::tick()
{
	// No requiere de evento
	iMouse->processMouseMotion();

	// Abre los controllers de la lista de conectados en el último tick
	auto it = UserServicePC::Instance()->getConectionInfos()->begin();
	while (it != UserServicePC::Instance()->getConectionInfos()->end()) {
		usersInput[(*it)] = InputStatus();

		// Comprobamos que es un controller y no un Usuario con F1 o F2, estos tienen un which de -1
		if (UserServicePC::Instance()->GetWhichByUid((*it)) >= 0) {

			if (!openPorts((*it))) {
				// Si ha dado error bórralo del mapa
				usersInput.erase((*it));
			}
		}
		++it;
	}

	// Cierra los controllers de la lista de desconectados en el último tick y los borra del mapa usersInput
	auto aux = UserServicePC::Instance()->getDisconectionInfos()->begin();
	while (aux != UserServicePC::Instance()->getDisconectionInfos()->end()) {

		if (UserServicePC::Instance()->GetWhichByUid((*aux)) >= 0) closePorts(usersInputInfo[(*aux)].controller);
		usersInput.erase((*aux));
		++aux;
	}
}

void InputPC::processEvent(SDL_Event evt)
{
	
	switch (evt.type) {
		/// EVENTOS TECLADO ///
	case SDL_KEYDOWN:
		processKeyboardEvents(evt);
		break;
	case SDL_KEYUP:
		processKeyboardEvents(evt);
		break;

		/// EVENTOS MOUSE ///
	case SDL_MOUSEBUTTONDOWN:
		iMouse->processMouseButtons(evt);
		break;
	case SDL_MOUSEBUTTONUP:
		iMouse->processMouseButtons(evt);
		break;

		/// EVENTOS GAMEPAD ///
	case SDL_CONTROLLERAXISMOTION:
		iGamepad->processAxisEvent(evt);
		break;
	case SDL_CONTROLLERBUTTONDOWN:
		iGamepad->processButtonEvent(evt, true);
		break;
	case SDL_CONTROLLERBUTTONUP:
		iGamepad->processButtonEvent(evt, false);
		break;
	default:
		break;
	}
}




bool InputPC::initInput()
{
	int ret = SDL_SetRelativeMouseMode(SDL_TRUE);
	if (ret != 0) {
		std::cerr << SDL_GetError() << "\n";
		return false;
	}

	iKey = new InputKeyboard();
	iMouse = new InputKeyboardMouse();
	iGamepad = new InputGamepad();

	Platform::Instance()->addListener(this);
	return true;
}

void InputPC::processKeyboardEvents(SDL_Event e){
	// Eventos del control de teclado y mouse
	if (e.key.keysym.sym == SDLK_RCTRL || e.key.keysym.sym == SDLK_RSHIFT) {
		iMouse->processKeyboard(e);
	}
	// Eventos del control de solo teclado
	else if (e.key.keysym.sym == SDLK_e || e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_w
		|| e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_d) {
		iKey->processKeyboard(e);
	}
	
}


void InputPC::closeInput()
{
	delete iKey;
	delete iMouse;
	delete iGamepad;

	if (PlatformPC::Instance())
		PlatformPC::Instance()->removeObserver(this);
	}


const InputStatus InputPC::getStatus(UserId id) { 
	if(usersInput.count(id))
		return usersInput[id];
	return InputStatus();
}

void InputPC::closePorts(SDL_GameController* controller)
{
	if (controller) {
		SDL_GameControllerClose(controller);
		controller = nullptr;
	}
}

bool InputPC::openPorts(UserId userId)
{
	int nJ = SDL_NumJoysticks();
	if (nJ > 0) {
		usersInputInfo[userId] = UserIdInputInfo();
		usersInputInfo[userId].controller = SDL_GameControllerOpen(UserServicePC::Instance()->GetWhichByUid(userId));
		if (!usersInputInfo[userId].controller) {
			usersInputInfo.erase(userId);
			std::cerr << "Error al abrir el controlador del Joystick, mas info en el siguiente error: ";
			std::cerr << SDL_GetError();
			return false;
		}

			

		usersInputInfo[userId].id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(usersInputInfo[userId].controller));
		if (usersInputInfo[userId].id < 0) {
			//error, cerramos el controller abierto
			SDL_GameControllerClose(usersInputInfo[userId].controller);
			usersInputInfo[userId].controller = nullptr;
			usersInputInfo.erase(userId);
			std::cerr << "Error al encontrar la id del joystick, mas info en el siguiente error: ";
			std::cerr << SDL_GetError();
			return false;
		}
	}
	else {

		std::cerr << "Error de joysticks conectados al sistema, mas info en el siguiente error: ";
		std::cerr << SDL_GetError();
		return false;
	}

	return true;
}


