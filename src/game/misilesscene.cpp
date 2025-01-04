#include "misilesscene.h"

#include <iostream>
#include <chrono>

#include "renderer.h"
#include "platform.h"
#include "input.h"

#include "userService.h"


#include "game_objects.h"
#include "spritesheet.h"
#include "sprite.h"


#include "render_thread.h"

//GO
#include "Ciudad.h"
#include "Circle.h"
#include "Missile.h"
#include "Suelo.h"
#include "Torreta.h"
#include "MissilePlayer.h"
#include "Cursor.h"

// Para que se llame al destructor de la imagen
#include "image.h"


//endianes iria separado de scene.h en el caso de que hcieisemos mas escenas distintas.
#include "endianess.h" 
#include "convert_tools.h"


MisilesScene::MisilesScene() : ciudadSheet(nullptr), turretsSheet(nullptr), cursoresSheet(nullptr), lastFrameTime(0),
	accFrameTime(0), refreshFrameTime(0), countFrames(0), renderThread(nullptr), toReset(false), timerToReset(0), lastCity(nullptr),
	citiesStanding(0), turretsStanding(0)
{
	lastFrameTime = accFrameTime = refreshFrameTime = countFrames = 0;
	ciudadSheet = nullptr;
}

bool MisilesScene::setup()
{
	if (!Platform::Init()) {
		return false;
	}
	if (!Renderer::Init(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT)) {
		Platform::Release();
		return false;
	}
	if (!UserService::Init()) {
		Renderer::Release();
		Platform::Release();
		return false;
	}
	if (!Input::Init()) {
		Renderer::Release();
		Platform::Release();
		UserService::Release();
		return false;
	}
	

	renderThread = new RenderThread();
	renderThread->init();
	objsToRender = std::priority_queue<RenderObject>();

	return true;
}

bool MisilesScene::loadScene()
{
	toReset = false;
	timerToReset = 0;
	lastFrameTime = Platform::Instance()->chrono();
	accFrameTime = refreshFrameTime = countFrames = 0;
	std::string files[NUM_FILES] = { "assets/ciudad.rgba", "assets/torretas.rgba", "assets/cursores.rgba"};

	for (size_t i = 0; i < NUM_FILES; i++)
		if (!loadImage(files[i]))
			return false;

	// SpriteSheet único que va a servir para todas las ciudades
	ciudadSheet = new SpriteSheet(images[0], 1, 8);
	turretsSheet = new SpriteSheet(images[1], 1, 4);
	cursoresSheet = new SpriteSheet(images[2], 1, 4);

	initSceneGOs();
	

	return true;
}

void MisilesScene::run()
{
	//mensaje para limpiar el renderer
	Message clear = Message(BACKGROUND_COLOR), present = Message(MessageType::PRESENT);
	while (Platform::Instance()->tick()) {
		Input::Instance()->tick();
		auto it = UserService::Instance()->getConectionInfos();
		auto it2 = UserService::Instance()->getDisconectionInfos();

		//std::cout << "in game vacia? " << (*it).empty() << std::endl;

		if (!(*it).empty() || !(*it2).empty()) {
			reasingTurrets();
		}

		// El tick de Userservice borra las listas de conexiones y desconexiones en el último frame
		// Es por ello que se debe hacer despúes de comprobar estas listas
		UserService::Instance()->tick();


		// time checks
		int currentTime = Platform::Instance()->chrono();
		int frameTime = currentTime - lastFrameTime;
		lastFrameTime += frameTime; accFrameTime += frameTime; refreshFrameTime += frameTime; countFrames++;

		if (refreshFrameTime > REFRESH_CHECK) {
			std::cout << "Current FPS: " << std::round(countFrames * 1000.0 / REFRESH_CHECK) << std::endl;
			refreshFrameTime = countFrames = 0;
		}

		
		

		// limpia el renderer
		renderThread->enqueue(clear);

		//// physics update
		while (accFrameTime > FIXED_DELTA_TIME) {
			for (size_t i = 0; i < objs.size(); i++)
				objs[i]->fixedUpdate(FIXED_DELTA_TIME / 1000.0f);
			accFrameTime -= FIXED_DELTA_TIME;

			checkCollisions();

			//logica para el reseteo de la escena
			if (lastCity != NULL && lastCity->chechAnimEnd())
				toReset = true;
		}

		if (toReset) {
			timerToReset += frameTime;
			if (timerToReset > RESET_TIME) resetScene();
		}
		//// check objetos marcados para destruirse
		//actualmente los objetos se resetean en lugar de destruirse y volve a crearse
		checkDestroy();

		//// update + render objs
		for (size_t i = 0; i < objs.size(); i++) {
				objs[i]->update(frameTime / 1000.0f);

			//objs[i]->render(); //ahora ya no se hace el render aqui si no que se ahce una vez se han actualizado todos los GO
		}


		//aqui se renderizan los objetos
		//segun el renderLayer (ordenados)
		renderFromQueue();

		// present buffer on display
				// presentar el buffer
		renderThread->enqueue(present);

		while (renderThread->pending() >= FRAME_WAIT) {
			// Espera de 10ms mientras el renderthread pinta ()
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
}

void MisilesScene::unloadScene()
{
	renderThread->close();
	delete renderThread;

	for(size_t i = 0; i < objs.size(); i++)
		delete objs[i];
	objs.clear();

	for (size_t i = 0; i < images.size(); i++)
		delete images[i];
	images.clear();

	delete ciudadSheet;
	delete turretsSheet;
	delete cursoresSheet;


	while (!objsToRender.empty())
		objsToRender.pop();
}

void MisilesScene::shutdown()
{
	Input::Release();
	UserService::Release();
	Renderer::Release();
	Platform::Release();
}

void MisilesScene::initSceneGOs()
{
	// Conseguimos el ancho para que ocupen todo el espacio posible según el numero de ciudades dado
	// Al ser gameObjects cuadrados, nos sirve tambien para la altura
	int cWidth = Renderer::Instance()->getWidth() / NUM_CIUDADES;

	// La posicion Y es la más baja sin que se salga de la pantalla
	int posY = Renderer::Instance()->getHeight() - cWidth;
	for (int i = 0; i < NUM_CIUDADES; ++i) {
		Ciudad* ciudad = new Ciudad(cWidth * i, posY, cWidth, cWidth, ciudadSheet, this);
		objs.push_back(ciudad);
		cities.push_back(ciudad);
	}
	//contador para la logica del reseteo de la escena
	citiesStanding = NUM_CIUDADES;


	// Generación Torretas
	int tWidth = cWidth / 4;
	int tHeight = tWidth / 2;

	// La posicion Y es el final del suelo
	posY = Renderer::Instance()->getHeight() - (cWidth * 2 / 3);

	Torreta* prevT = nullptr;
	for (int i = 0; i < NUM_TORRETAS; ++i) {

		MissilePlayer* m = new MissilePlayer(cWidth * (i + 1), posY, this);
		
		Torreta* t = new Torreta(cWidth * (i + 1), posY, tWidth, tHeight, 0, turretsSheet, m, this);
		
		objs.push_back(t);
		objs.push_back(m);
		turrets.push_back(t);
		playerMissiles.push_back(m);

		if (prevT) {
			prevT->setNextTurret(t);
		}
		prevT = t;
		
	}

	for (int i = 0; i < NUM_TORRETAS; ++i) {

		Cursor* c = new Cursor(Renderer::Instance()->getWidth()/2, Renderer::Instance()->getHeight()/2, Renderer::Instance()->getWidth() / 20, Renderer::Instance()->getWidth() / 20, i, turrets[0], cursoresSheet, this);
		objs.push_back(c);
		cursores.push_back(c);

	}

	//contador para la logica del reseteo de la escena
	turretsStanding = NUM_TORRETAS;

	// generation
	for (int i = 0; i < NUM_MISSILES; ++i)
	{
		Missile* m = new Missile(this, Renderer::Instance()->getHeight() - (cWidth / 3));
		objs.push_back(m);
		missiles.push_back(m);
	}

	Suelo* suelo = new Suelo(Renderer::Instance()->getHeight() - (cWidth * 2 / 3), 0xFF008000, this);

	for (Cursor* c : cursores) {
		c->setActive(false);
	}

	objs.push_back(suelo);
}

void MisilesScene::reasingTurrets()
{
	std::cout << "reasigning turrets \n";


	
	////cursores
	auto c = UserService::Instance()->getInfos();
	auto con = UserService::Instance()->getConectionInfos();
	auto dis = UserService::Instance()->getDisconectionInfos();



	for (UserId id : *dis) {
		for (Cursor* cu : cursores) {
			if (cu->getUserId() == id) {
				cu->setActive(false);
				break;
			}
		}
	}

	for (UserId id : *con) {
		//elegir el primero que este desactivado
		for (Cursor* cu : cursores) {
			if (!cu->getActive()) {
				cu->setActive(true);
				cu->setUserId(id);
				//setear un cursor en la posicion inicial
				break;
			}
		}
	}





	//Torretas

	if (c->empty()) {
		//que todas se vuelvan a ver azules //por estetica
		for (Torreta* t : turrets)
			t->setType(0);
		return;
	}

	int sobra = NUM_TORRETAS % c->size();
	

	if (sobra != 0) {
		turrets[0]->setType(c->at(0));
	}

	int steps = (NUM_TORRETAS - sobra) / c->size();


	for (int i = 0; i < c->size(); i++) {
		for (int x = 0; x < steps; x++) {
			turrets[(i*steps) + x + sobra]->setType(c->at(i));
		}
	}
}

bool MisilesScene::loadImage(std::string filepath)
{
	uint32_t w, h, * pxls;

	FILE* file = Platform::Instance()->openFile(filepath);
	if (!file)
		return false;

	size_t ret = fread(&w, sizeof(uint32_t), 1, file);
	if (ret == 0) {
		std::cerr << "Error al leer el ancho la imagen " << filepath << "\n";
		fclose(file);
		return false;
	}
	ret = fread(&h, sizeof(uint32_t), 1, file);
	if (ret == 0) {
		std::cerr << "Error al leer la altura la imagen " << filepath << "\n";
		fclose(file);
		return false;
	}
#ifdef LIL_ENDIAN
	w = CONVERT_ENDIANESS_32(w);
	h = CONVERT_ENDIANESS_32(h);
#endif // LITTLE_ENDIAN

	// Se pide la memoria para el array de píxeles y se rellena de una sola lectura con fread
	pxls = (uint32_t*)malloc(sizeof(uint32_t) * w * h);
	if (pxls == NULL) {
		std::cerr << "No hay suficiente memoria disponible para la imagen " << filepath << " \n";
		fclose(file);
		return false;
	}
	ret = fread(pxls, sizeof(uint32_t), (size_t)w * (size_t)h, file);
	if (ret == 0) {
		std::cerr << "Error al leer los pixeles de la imagen " << filepath << "\n";
		free(pxls);
		fclose(file);
		return false;
	}

	for (uint32_t i = 0; i < w * h; ++i) {
#ifdef LIL_ENDIAN
		pxls[i] = CONVERT_ENDIANESS_32(pxls[i]);
#endif
		pxls[i] = CONVERT_RGBA_TO_ARGB(pxls[i]);	// todos los píxeles en ARGB
	}

	// asumimos pixeles en ARGB siempre, independientemente de la plataforma
	// además, la imagen se encargará de eliminar la memoria que haya reservado ella para guardar su copia de los pixeles
	const Image* imag = Renderer::Instance()->createImage(w, h, pxls);
		if (imag == nullptr) {
			std::cerr << "Error al crear la Imagen " << filepath << "  --> Scene::LoadImage\n";
			free(pxls);
			fclose(file);
			return false;
		}
	images.push_back(imag);

	fclose(file);


	//liberamos los pixeles que hemos leido ya que a partir de aqui los gestionan las imagenes
	free(pxls);

	return true;
}


void MisilesScene::checkDestroy()
{
	auto it = objs.begin();
	while (it != objs.end()) {
		if ((*it)->destroy()) {
			delete* it;
			it = objs.erase(it);
		}
		else ++it;
	}
}

void MisilesScene::renderObject(const Message m, const int& l)
{
	objsToRender.push(RenderObject(m, l));
}

void MisilesScene::renderFromQueue()
{
	for (auto gO : objs)
		gO->render();

	while (!objsToRender.empty())
	{
		renderThread->enqueue(objsToRender.top().message);
		objsToRender.pop();
	}
}

void MisilesScene::checkCollisions()
{
	//chekeo de las colisones misil --> ciudad y torretas
	for (Missile* m : missiles)
	{
		for (Ciudad* c : cities)
		{
			if (c->checkCollision(m->getX(), m->getY()))
			{
				c->setDestroyed(true);
				m->resetMissile();

				citiesStanding--;
				if (citiesStanding <= 0) {
					lastCity = c;
				}
			}
		}

		for (Torreta* t : turrets)
		{
			if (t->checkCollision(m->getX(), m->getY()))
			{
				t->setToExplode();
				m->resetMissile();

				turretsStanding--;
				if (turretsStanding <= 0) {
					toReset = true;
				}
			}
		}

		for (MissilePlayer* mp : playerMissiles) {
			if (mp->isPointInExplosion((int)m->getX(), (int)m->getY())) m->setToExplode(mp->getColor());
		}

		for (Missile* mExploded : missiles) {
			if(mExploded->getExploding())
				if (mExploded->isPointInExplosion((int)m->getX(), (int)m->getY())) m->setToExplode(mExploded->getExplodingColor());
		}
	}
}

/// <summary>
/// Coloca los objetos ya existentes en sus estados base
/// 5 ciudades y 5 misiles cayendo desde el borde de la pantalla
/// </summary>
void MisilesScene::resetScene()
{
	toReset = false;
	timerToReset = 0;
	citiesStanding = NUM_CIUDADES;
	turretsStanding = NUM_TORRETAS;
	lastCity = NULL;
	for (Missile* m : missiles)
		m->resetMissile();
	for (Ciudad* c : cities)
		c->ciudadReset();
	for (Torreta* t : turrets)
		t->turretReset();

}
