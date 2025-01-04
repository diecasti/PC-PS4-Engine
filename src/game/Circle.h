#pragma once

#include "game_objects.h"

/// -------------------- CIRCLE --------------------

#define CIRCLE_TYPES 4

class Circle : public GameObject {
public:
	// Constructor del círculo con posición aleatoria
	Circle(MisilesScene* scene_);

	/*
	* Constructor del círculo con posición definida y sin movimiento
	* @param xPos, yPos: Posición en pantalla
	* @param r: Radio
	* @param t: Índice del color del círculo [0,3]
	*/
	Circle(int xPos, int yPos, int r, int t, MisilesScene* scene_);
	virtual ~Circle() = default;

	// Update del movimiento del círculo
	virtual void fixedUpdate(float fixedDeltaTime);
	virtual void update(float deltaTime);
	// Renderizado del círculo y la línea llamando a Renderer
	virtual void render();

private:
	static uint32_t avColors[CIRCLE_TYPES];

	int x, y;
	int xLine, yLine;
	int radius, maxRadius, crecimiento;

	int type;

	void move(float fixedDeltaTime);
};