#include "platform_pc.h"

#include <iostream>
#include <climits>

#include <SDL.h>

std::unique_ptr<PlatformPC> PlatformPC::instance = nullptr;

PlatformPC::PlatformPC() = default;

PlatformPC::~PlatformPC() {

	closePlatform();
}

PlatformPC* PlatformPC::Instance() {
	return instance.get();
}

bool PlatformPC::Init() {
	instance.reset(new PlatformPC());

	return instance.get()->initPlatform();
}

void PlatformPC::Release() {
	if (instance.get() == nullptr)
		return;
	//delete instance.get();

	instance.reset(nullptr);
}

int PlatformPC::chrono()
{
	return SDL_GetTicks();
}

bool PlatformPC::tick()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				return false;
			else
				sendEvent(event);
			break;
		case SDL_QUIT:
			return false;
			break;
		default:
			sendEvent(event);
			break;
		}
	}
	return true;
}

FILE* PlatformPC::openFile(const std::string path)
{
	FILE* file;
	fopen_s(&file, path.c_str(), "rb");

	if (!file)
		std::cerr << "Failure opening file " << path << ", nullptr returned.\n";

	return file;
}

int PlatformPC::randomInteger(int lower, int upper)
{
	return std::rand() % (upper - lower) + lower;
}

float PlatformPC::randomRange(float lower, float upper) {
	return lower + static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * (upper - lower);
}

void PlatformPC::addListener(SDLEventListener* iL)
{
	listeners.push_back(iL);
}

void PlatformPC::removeObserver(SDLEventListener* iL)
{
	auto it = listeners.begin();
	while (it != listeners.end()) {
		if ((*it) == iL) {
			// delete (*it);
			it = listeners.erase(it);
			break;
		}
		else ++it;
	}
}

bool PlatformPC::initPlatform() {
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(156); // id of the new we are trying to delete correctly (use it when checkML doesn't work)
	
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cerr << SDL_GetError();
		return false;
	}

	initRandomSeed();

	return true;
}

void PlatformPC::initRandomSeed()
{
	srand((unsigned int)std::time(nullptr));
}

void PlatformPC::closePlatform() {
	SDL_Quit();
}

void PlatformPC::sendEvent(SDL_Event evt)
{
	for (SDLEventListener* iL : listeners) {
		iL->processEvent(evt);
	}
}
