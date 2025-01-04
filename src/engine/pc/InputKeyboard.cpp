#include "InputKeyboard.h"
#include "SDL.h"
#include "input_pc.h"
#include "user_service_pc.h"
#include "login_type.h"

InputKeyboard::InputKeyboard()
{
	input = InputPC::Instance();
}

void InputKeyboard::processKeyboard(SDL_Event evt)
{
	auto it = UserServicePC::Instance()->usersBegin();
	while (it != UserServicePC::Instance()->usersEnd()) {
		if (UserServicePC::Instance()->GetLoginTypeByUid((*it)) == LoginType::LOGIN_F2) break;
		++it;
	}
	if (it != UserServicePC::Instance()->usersEnd()) {
		InputStatus inSt = input->usersInput[(*it)];
		switch (evt.key.keysym.sym) {

			//the other cases are SDL_KEYUPS
		case SDLK_e:
			if (evt.type == SDL_KEYDOWN) {
				inSt.action_1 = true;
			}
			else {
				inSt.action_1 = false;
			}
			break;
		case SDLK_q:
			if (evt.type == SDL_KEYDOWN) {
				inSt.action_2 = true;
			}
			else {
				inSt.action_2 = false;
			}
			break;
		case SDLK_w:
			if (evt.type == SDL_KEYDOWN) {
				inSt.verticalAxis = -1;
			}
			else if (inSt.verticalAxis < 0) {
				inSt.verticalAxis += 1;
			}
			break;
		case SDLK_a:
			if (evt.type == SDL_KEYDOWN) {
				inSt.horizontalAxis = -1;
			}
			else if (inSt.horizontalAxis < 0) {
				inSt.horizontalAxis += 1;
			}
			break;
		case SDLK_s:
			if (evt.type == SDL_KEYDOWN) {
				inSt.verticalAxis = 1;
			}
			else if (inSt.verticalAxis > 0) {
				inSt.verticalAxis += -1;
			}
			break;
		case SDLK_d:
			if (evt.type == SDL_KEYDOWN) {
				inSt.horizontalAxis = 1;
			}
			else if (inSt.horizontalAxis > 0) {
				inSt.horizontalAxis += -1;
			}
			break;
		}
		input->usersInput[(*it)] = inSt;
	}
}
