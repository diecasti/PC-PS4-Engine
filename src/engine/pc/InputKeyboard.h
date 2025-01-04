#pragma once

union SDL_Event;
class InputPC;
class InputKeyboard
{
public:
	InputKeyboard();
	~InputKeyboard() {}

	/// Procesa la entrada de teclado
	void processKeyboard(SDL_Event evt);

private:
	InputPC* input;
};

