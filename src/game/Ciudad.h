#pragma once

#include "game_objects.h"

#define CITY_ANIM_VEL 0.25
#define CITY_ANIM_FRAMES 7

//CITY_X_POS_PERCENT = 0.367f
//CITY_Y_POS_PERCENT = 0.355f
//CITY_W_POS_PERCENT = 0.332f
//CITY_H_POS_PERCENT = 0.378f

class Ciudad : public GameObject {
public:
	//Ciudad();


	Ciudad(int _xPos, int _yPos, int _w, int _h, const SpriteSheet* _s, MisilesScene* scene_);
	virtual ~Ciudad() = default;

	virtual void fixedUpdate(float fixedDeltaTime);
	virtual void update(float deltaTime);
	virtual void render();

	bool checkCollision(float x, float y);
	void setDestroyed(bool b);

	void ciudadReset();

	bool chechAnimEnd();

private:
	int xPos, yPos;
	int wPos, hPos;

	int xPosBound, yPosBound;
	int wPosBound, hPosBound;

	int frameActual;
	float animationElapsedTime;

	bool destroyed;
	bool animationEnded;

	const SpriteSheet* s;
};
