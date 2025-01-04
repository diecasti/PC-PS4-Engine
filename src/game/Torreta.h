#pragma once

#include "game_objects.h"
#include "userid_def.h"

#define COLOR_EXPLOSION 0xffffffff

class MissilePlayer;

class Torreta : public GameObject {
public:
	/*
	* Constructor de la torreta
	* @param xPos, yPos: Centro de la posición en pantalla de la torreta
	* @param w, h: Ancho y alto de la torreta
	* @param t: Índice del spriteSheet
	* @param _s: SpriteSheet de las torretas
	*/
	Torreta(int xPos, int yPos, int w, int h, int t, const SpriteSheet* _s,  MissilePlayer* m, MisilesScene* scene_);
	virtual ~Torreta() = default;

	virtual void fixedUpdate(float fixedDeltaTime);
	virtual void update(float deltaTime);
	// Renderizado de la torreta y la línea llamando a Renderer
	virtual void render();

	bool checkCollision(float xOther, float yOther);
	void setDestroyed(bool b);
	void turretReset();
	bool shoot(int y, int x, int type);
	void setNextTurret(Torreta* t);
	void setType(UserId type);

	void setToExplode();

private:
	int x, y;
	int width, height;

	int type;

	// Variables para la explosión de torretas //
	bool toExplode;
	int maxRadius;
	int radius;
	int crecimiento;

	bool destroyed;

	const SpriteSheet* s;
	MissilePlayer* missile;
	Torreta* nextTurret;


	void move(float fixedDeltaTime);
};