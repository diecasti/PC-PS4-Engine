#include "Missile.h"


#include "renderer.h"
#include "platform.h"
#include "misilesscene.h"
#include "messages.h"
//#include <iostream>

#include "render_layers.h"
#include <iostream>

Missile::Missile(MisilesScene* scene_, int deathY) : GameObject(scene_), deadZone(deathY) {
	// Posición aleatoria en la pantalla desde arriba
	resetMissile();
	maxRadius = Renderer::Instance()->getWidth() / 40;
	// 1/6 de segundo en crecer, otro 1/6 de segundo en decrecer
	crecimiento = maxRadius * 6;
	
	color = 0xFFFFFFFF;
	expColor = color;
}

void Missile::fixedUpdate(float fixedDeltaTime)
{
	if (!toExplode) {
		move(fixedDeltaTime);
		if (y > deadZone) {

			simTime = (y - Renderer::Instance()->getHeight()) / (-Ydir * speed);
			resetMissile();
			move(simTime);
		}
	}
	else {
		circleExplosion(fixedDeltaTime);
		if (radius < 1) resetMissile();
	}
	
		
}

void Missile::update(float deltaTime)
{
}

void Missile::render()
{
	if(!toExplode)
		scene->renderObject(Message((int)xOri, (int)yOri, (int)x, (int)y, color), MISSILE_LAYER);
	else
		scene->renderObject(Message((int)x, (int)y, radius, expColor), MISSILE_LAYER);
}

float Missile::getX()
{
	return x;
}

float Missile::getY()
{
	return y;
}

void Missile::resetMissile()
{
	// Posición aleatoria en la pantalla desde arriba
	xOri = Platform::Instance()->randomInteger(0, Renderer::Instance()->getWidth());
	x = (float)xOri;
	yOri = 0;
	y = (float)yOri;
	finalX = Platform::Instance()->randomInteger(0, Renderer::Instance()->getWidth());
	finalY = Renderer::Instance()->getHeight();

	toExplode = false;
	radius = 1;
	crecimiento = abs(crecimiento);

	speed = (float)finalY / 12;

	double vecX = (double)finalX - x;
	double vecY = (double)finalY - y;

	double vecMod = sqrt((vecX * vecX) + (vecY * vecY));
	Xdir = (float)(vecX / vecMod);
	Ydir = (float)(vecY / vecMod);
}

void Missile::setToExplode(uint32_t color_)
{
	expColor = color_;
	toExplode = true;
}

bool Missile::isPointInExplosion(int x_od, int y_od)
{
	if (!toExplode) return false;
	return ((x_od - x) * (x_od - x) + (y_od - y) * (y_od - y)) < radius * radius;
}

bool Missile::getExploding()
{
	return toExplode;
}

uint32_t Missile::getExplodingColor()
{
	return expColor;
}

void Missile::move(float fixedDeltaTime)
{
	 x += (Xdir * speed * fixedDeltaTime);
	 y += (Ydir * speed * fixedDeltaTime);
}

void Missile::circleExplosion(float fixedDeltaTime)
{
	radius += (int)(crecimiento * fixedDeltaTime);
	if (radius >= maxRadius) crecimiento = -crecimiento;
}
