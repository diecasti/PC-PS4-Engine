#pragma once

#include "game_objects.h"

#define MISSILE_COLORS 4

class MissilePlayer : public GameObject {
public:
	// Constructor del misil con posición aleatoria
	MissilePlayer(int x_, int y_, MisilesScene* scene_);
	virtual ~MissilePlayer() = default;

	// Update del movimiento del círculo
	virtual void fixedUpdate(float fixedDeltaTime);
	virtual void update(float deltaTime);
	// Renderizado del círculo y la línea llamando a Renderer
	virtual void render();

	float getX();
	float getY();

	void resetMissilePlayer();
	void shoot(int xDest, int yDest, int cInd);

	bool getActive();

	bool isPointInExplosion(int x_od, int y_od);
	bool canShoot();

	uint32_t getColor();

private:
	static uint32_t avColors[MISSILE_COLORS];

	int maxRadius;
	int radius;
	int crecimiento;

	float x, y;
	int xOri, yOri;
	int finalX, finalY;
	float Xdir, Ydir;

	float speed;
	uint32_t color;
	float simTime;

	bool active;
	bool explode;

	void move(float fixedDeltaTime);
	void circleExplosion(float fixedDeltaTime);
};