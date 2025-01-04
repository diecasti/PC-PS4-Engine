#pragma once

#include "game_objects.h"

/// -------------------- MISSILE --------------------


class Missile : public GameObject {
public:
	// Constructor del misil con posición aleatoria
	Missile(MisilesScene* scene_, int deathY);
	virtual ~Missile() = default;

	// Update del movimiento del círculo
	virtual void fixedUpdate(float fixedDeltaTime);
	virtual void update(float deltaTime);
	// Renderizado del círculo y la línea llamando a Renderer
	virtual void render();

	float getX();
	float getY();

	void resetMissile();

	void setToExplode(uint32_t color);
	bool isPointInExplosion(int x_od, int y_od);

	bool getExploding();

	uint32_t getExplodingColor();

private:
	int deadZone;
	float x, y;
	int xOri, yOri;
	int finalX, finalY;
	float Xdir, Ydir;

	bool toExplode;
	int maxRadius;
	int radius;
	int crecimiento;

	float speed;
	uint32_t color;
	uint32_t expColor;
	float simTime;

	void move(float fixedDeltaTime);
	void circleExplosion(float fixedDeltaTime);
};