#pragma once

#include "game_objects.h"
#include "userid_def.h"

#define CURSOR_SPEED 15

class Torreta;
class Cursor : public GameObject {
public:
	/*
	* Constructor del Cursor
	* @param xPos, yPos: Centro de la posición en pantalla del Cursor
	* @param w, h: Ancho y alto del Cursor
	* @param t: Índice del spriteSheet
	* @param _s: SpriteSheet de las Cursores
	*/
	Cursor(int xPos, int yPos, int w, int h, int t, Torreta* torret, const SpriteSheet* _s, MisilesScene* scene_);
	virtual ~Cursor() = default;

	virtual void fixedUpdate(float fixedDeltaTime);
	virtual void update(float deltaTime);
	// Renderizado del Cursor y la línea llamando a Renderer
	virtual void render();

	void setDestroyed(bool b);

	void setActive(bool a);
	void setPosition(int x, int y);

	bool getActive();

	void setUserId(UserId id_);
	UserId getUserId();
	
	//int getShootX() { return x; }
	//int getShootY() { return y; }
private:
	int xOri, yOri;
	int x, y;
	int width, height;

	int type; 

	UserId id; //Usuario actual

	bool active;
	bool shooted;

	bool lastRTrigger;
	bool lastLTrigger;

	const SpriteSheet* s;
	Torreta* torreta;

	float relativeSpeed;

	void move(float fixedDeltaTime);
};