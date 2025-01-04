#pragma once

#include "game_objects.h"

class Suelo : public GameObject {
public:
	Suelo(int _yPos, uint32_t c, MisilesScene* scene_);
	virtual ~Suelo() = default;

	virtual void fixedUpdate(float fixedDeltaTime);
	virtual void update(float deltaTime);
	virtual void render();

private:
	int yPos;
	uint32_t color;
};