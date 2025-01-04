//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include <iostream>


#include "misilesscene.h"


int main(int ac, char* av[])
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	MisilesScene* scene = new MisilesScene();

	if (!scene->setup()) {
		scene->shutdown();
		delete scene; scene = nullptr;
		return -1;
	}

	if (!scene->loadScene()) {
		scene->unloadScene();
		scene->shutdown();
		delete scene; scene = nullptr;
		return -1;
	}

	scene->run();

	scene->unloadScene();
	scene->shutdown();

	delete scene; scene = nullptr;
	return 0;
}