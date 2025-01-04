#pragma once

union SDL_Event;

class SDLEventListener {
private:
protected:
public:
	virtual ~SDLEventListener() {};

	virtual void processEvent(SDL_Event evt) = 0;
};