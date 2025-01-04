#pragma once

union SDL_Event;
class InputPC;
class InputGamepad
{
public:
	InputGamepad();
	~InputGamepad() {}

	/// Procesa los eventos de ejes de los controller
	/// Los triggers y los joysticks
	void processAxisEvent(SDL_Event evt);

	/// Procesa los eventos de botones de los controller
	void processButtonEvent(SDL_Event evt, bool down);

private:
	InputPC* input;
};

