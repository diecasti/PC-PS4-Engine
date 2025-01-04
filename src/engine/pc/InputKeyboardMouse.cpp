#include "InputKeyboardMouse.h"
#include "SDL.h"
#include "input_pc.h"
#include "user_service_pc.h"
#include "login_type.h"
#include <math.h>

InputKeyboardMouse::InputKeyboardMouse()
{
	input = InputPC::Instance();
}

void InputKeyboardMouse::processMouseMotion()
{
	// Busqueda del jugador que utiliza mouse, el que ha iniciado con F1
	auto it = UserServicePC::Instance()->usersBegin();
	while (it != UserServicePC::Instance()->usersEnd()) {
		if (UserServicePC::Instance()->GetLoginTypeByUid((*it)) == LoginType::LOGIN_F1) break;
		++it;
	}
	if (it != UserServicePC::Instance()->usersEnd()) {
		int x = 0, y = 0;
		// Porque no estamos usando el evento para e.motion.xrel, bien, como se puede leer en GetRelativeMouseState,
		// este nos devuelve el delta, es decir la acumulacion de e.motion.xrel y e.motion.yrel ocurridos des la
		// inicializacion del sistema, o desde la ultima vez que se llamo al sistema, est emetodo nos permite ahorrarnos
		// hacer su trabajo, el cual seria guardarnos unas variables en la que acumularamos cada uno de os eventos de
		// mouse motion para conseguir sus xrel e y rel, y luego tener que aplicarlas en un algun punto de nuestra ejecucion,
		// nosotros hemos escogido que sea en el tick.
		SDL_GetRelativeMouseState(&x, &y);


		InputStatus inSt = input->usersInput[(*it)];

		inSt.angularVelY = -x * MOUSE_RELATIVE;
		inSt.angularVelX = -y * MOUSE_RELATIVE;



		input->usersInput[(*it)] = inSt;
	}
}

void InputKeyboardMouse::processMouseButtons(SDL_Event evt)
{
	// Busqueda del jugador que utiliza mouse, el que ha iniciado con F1
	auto it = UserServicePC::Instance()->usersBegin();
	while (it != UserServicePC::Instance()->usersEnd()) {
		if (UserServicePC::Instance()->GetLoginTypeByUid((*it)) == LoginType::LOGIN_F1) break;
		++it;
	}
	if (it != UserServicePC::Instance()->usersEnd()) {
		InputStatus inSt = input->usersInput[(*it)];
		if (evt.type == SDL_MOUSEBUTTONDOWN)
		{
			if (evt.button.button == SDL_BUTTON_LEFT)
				inSt.action_1 = true;
			else if (evt.button.button == SDL_BUTTON_RIGHT)
				inSt.action_2 = true;
		}
		if (evt.type == SDL_MOUSEBUTTONUP)
		{
			if (evt.button.button == SDL_BUTTON_LEFT)
				inSt.action_1 = false;
			else if (evt.button.button == SDL_BUTTON_RIGHT)
				inSt.action_2 = false;
		}


		input->usersInput[(*it)] = inSt;
	}

}

void InputKeyboardMouse::processKeyboard(SDL_Event evt)
{
	auto it = UserServicePC::Instance()->usersBegin();
	while (it != UserServicePC::Instance()->usersEnd()) {
		if (UserServicePC::Instance()->GetLoginTypeByUid((*it)) == LoginType::LOGIN_F1) break;
		++it;
	}
	if (it != UserServicePC::Instance()->usersEnd()) {
		InputStatus inSt = input->usersInput[(*it)];
		switch (evt.key.keysym.sym) {
		case SDLK_RCTRL:
			if (evt.type == SDL_KEYDOWN) {
				inSt.action_1 = true;
			}
			else {
				inSt.action_1 = false;
			}
			break;
		case SDLK_RSHIFT:
			if (evt.type == SDL_KEYDOWN) {
				inSt.action_2 = true;
			}
			else {
				inSt.action_2 = false;
			}
			break;
		}
		input->usersInput[(*it)] = inSt;
	}
}
