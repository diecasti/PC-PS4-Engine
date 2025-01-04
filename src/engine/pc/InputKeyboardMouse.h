#pragma once
union SDL_Event;
class InputPC;

class InputKeyboardMouse
{
public:
	InputKeyboardMouse();
	~InputKeyboardMouse() {}

	/// Aplica el mouse realative acumulado por SDL, no requiere de evento
	void processMouseMotion();

	/// Procesa los botones del mouse
	void processMouseButtons(SDL_Event evt);

	/// Procesa los eventos de teclado
	/// El RightControl y RightShift
	void processKeyboard(SDL_Event evt);

private:
	InputPC* input;
};
