#pragma once
#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

//para poder utilizar uint32_t en todos los gameObejcts, asi poder indicarles su color, dependiendo su tipo, etc etc.
#include <stdint.h>

class SpriteSheet;
class MisilesScene;

class GameObject {
public:
	GameObject(MisilesScene* scene_);
	virtual ~GameObject() = default;

	virtual void fixedUpdate(float fixedDeltaTime) = 0;
	virtual void update(float deltaTime) = 0;

	/// <summary>
	/// Crea el mensaje de renderizado
	/// </summary>
	virtual void render() = 0;

	bool destroy() { return death; };

protected:

	MisilesScene* scene;

	bool death = false;
};

#endif // GAMEOBJECTS_H