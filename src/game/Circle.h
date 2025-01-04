#pragma once

#include "game_objects.h"

/// -------------------- CIRCLE --------------------

#define CIRCLE_TYPES 4

class Circle : public GameObject {
public:
	// Constructor del c�rculo con posici�n aleatoria
	Circle(MisilesScene* scene_);

	/*
	* Constructor del c�rculo con posici�n definida y sin movimiento
	* @param xPos, yPos: Posici�n en pantalla
	* @param r: Radio
	* @param t: �ndice del color del c�rculo [0,3]
	*/
	Circle(int xPos, int yPos, int r, int t, MisilesScene* scene_);
	virtual ~Circle() = default;

	// Update del movimiento del c�rculo
	virtual void fixedUpdate(float fixedDeltaTime);
	virtual void update(float deltaTime);
	// Renderizado del c�rculo y la l�nea llamando a Renderer
	virtual void render();

private:
	static uint32_t avColors[CIRCLE_TYPES];

	int x, y;
	int xLine, yLine;
	int radius, maxRadius, crecimiento;

	int type;

	void move(float fixedDeltaTime);
};