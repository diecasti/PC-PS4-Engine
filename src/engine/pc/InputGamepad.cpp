#include "InputGamepad.h"
#include "SDL.h"
#include "input_pc.h"
#include "user_service_pc.h"
#include "login_type.h"

InputGamepad::InputGamepad()
{
	input = InputPC::Instance();
}

void InputGamepad::processAxisEvent(SDL_Event evt)
{
	auto it = UserServicePC::Instance()->usersBegin();
	while (it != UserServicePC::Instance()->usersEnd()) {
		if (UserServicePC::Instance()->GetLoginTypeByUid((*it)) == LoginType::LOGIN_JOYSTICK && input->usersInputInfo[(*it)].id == evt.caxis.which) break;
		++it;
	}
	if (it != UserServicePC::Instance()->usersEnd()) {
		float axis_offset = (float)evt.caxis.value / (float)SDL_MAX_SINT16;

		InputStatus inSt = input->usersInput[(*it)];

		switch (evt.caxis.axis)
		{
		case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
			// Para los triggers comprimimos la información entre 0 y 1, estaba en -1 a 1
			inSt.leftTrigger = axis_offset;
			break;
		case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
			inSt.rightTrigger = axis_offset;
			break;
		case SDL_CONTROLLER_AXIS_LEFTX:
			if(axis_offset >= 0)
				inSt.horizontalAxis = std::abs(axis_offset) > DEADZONE ? ((axis_offset - DEADZONE) / (1 - DEADZONE)) : 0;
			else
				inSt.horizontalAxis = std::abs(axis_offset) > DEADZONE ? ((axis_offset + DEADZONE) / (1 - DEADZONE)) : 0;
			break;
		case SDL_CONTROLLER_AXIS_LEFTY:
			if(axis_offset >= 0)
				inSt.verticalAxis = std::abs(axis_offset) > DEADZONE ? ((axis_offset - DEADZONE) / (1 - DEADZONE)) : 0;
			else
				inSt.verticalAxis = std::abs(axis_offset) > DEADZONE ? ((axis_offset + DEADZONE) / (1 - DEADZONE)) : 0;

			break;
		}

		input->usersInput[(*it)] = inSt;
	}
}

void InputGamepad::processButtonEvent(SDL_Event evt, bool down)
{
	auto it = UserServicePC::Instance()->usersBegin();
	while (it != UserServicePC::Instance()->usersEnd()) {
		if (UserServicePC::Instance()->GetLoginTypeByUid((*it)) == LoginType::LOGIN_JOYSTICK && input->usersInputInfo[(*it)].id == evt.cbutton.which) break;
		++it;
	}
	if (it != UserServicePC::Instance()->usersEnd()) {

		InputStatus inSt = input->usersInput[(*it)];



		switch (evt.cbutton.button)
		{
		case SDL_CONTROLLER_BUTTON_A:
			inSt.action_1 = down;
			break;
		case SDL_CONTROLLER_BUTTON_B:
			inSt.action_2 = down;
			break;
		}

		input->usersInput[(*it)] = inSt;
	}
}
