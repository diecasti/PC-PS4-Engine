#include "Circle.h"

#include "renderer.h"
#include "platform.h"
#include "stdint.h"
#include "misilesscene.h"
#include "messages.h"
#include "render_layers.h"


uint32_t Circle::avColors[CIRCLE_TYPES] = { 0xFFFF0000, 0xFF00FF00, 0xFF4040FF, 0xFFDB7093 };

Circle::Circle(MisilesScene* scene_) : GameObject(scene_) {
	// Posición del círculo aleatoria en la pantalla
	x = Platform::Instance()->randomInteger(0, Renderer::Instance()->getWidth());
	y = Platform::Instance()->randomInteger(0, Renderer::Instance()->getHeight());
	xLine = x < Renderer::Instance()->getWidth() / 2 ? 0 : Renderer::Instance()->getWidth();
	yLine = y < Renderer::Instance()->getHeight() / 2 ? 0 : Renderer::Instance()->getHeight();

	// Radio y radio máximo, aleatorio entre 1/20 y 1/8 del alto de la pantalla
	radius = 1;
	maxRadius = Platform::Instance()->randomInteger(Renderer::Instance()->getHeight() / 20, Renderer::Instance()->getHeight() / 8);

	// Crecimiento por segundo del radio
	crecimiento = Renderer::Instance()->getHeight() / 10;

	type = Platform::Instance()->randomInteger(0, CIRCLE_TYPES);
}

Circle::Circle(int xPos, int yPos, int r, int t, MisilesScene* scene_) : GameObject(scene_)
{
	x = xPos;
	y = yPos;
	radius = r;
	type = t;

	crecimiento = 0;
	maxRadius = 0;
	xLine = 0;
	yLine = 0;
}

void Circle::fixedUpdate(float fixedDeltaTime)
{
	move(fixedDeltaTime);

	if (radius < 1)
		death = true;
}

void Circle::update(float deltaTime)
{
}

void Circle::render()
{
	scene->renderObject(Message(x, y, radius, avColors[type]), MISSILE_LAYER);

	scene->renderObject(Message(x, y, xLine, yLine, avColors[type]), MISSILE_LAYER);
}

void Circle::move(float fixedDeltaTime)
{
	radius += (int)(crecimiento * fixedDeltaTime);
	if (radius >= maxRadius) crecimiento = -crecimiento;
}
