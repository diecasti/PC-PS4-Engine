#include "MissilePlayer.h"


#include "renderer.h"
#include "platform.h"
#include "misilesscene.h"
#include "messages.h"

#include "render_layers.h"
#include <iostream>

uint32_t MissilePlayer::avColors[MISSILE_COLORS] = { 0xFF0000FF, 0xFFFF0000, 0xFF00FF00, 0xFFFFC0CB };

MissilePlayer::MissilePlayer(int x_, int y_, MisilesScene* scene_) : GameObject(scene_), xOri(x_), yOri(y_) {
	speed = (float)Renderer::Instance()->getHeight() / 3;
	// Radio 1/20 del ancho de la pantalla
	maxRadius = Renderer::Instance()->getWidth() / 20;
	// Tarda 1/3 de segundo en llegar a crecimiento y otro en volver
	crecimiento = (maxRadius) * 3;

	resetMissilePlayer();
}

void MissilePlayer::fixedUpdate(float fixedDeltaTime)
{
	if (active) {
		move(fixedDeltaTime);

		if (y <= finalY) {
			active = false;
			explode = true;
		}
	}
	else if (explode) {
		circleExplosion(fixedDeltaTime);
		if (radius < 1) {
			radius = 1;
			crecimiento = -crecimiento;
			explode = false;
		}
	}
}

void MissilePlayer::update(float deltaTime)
{
}

void MissilePlayer::render()
{
	if (active)
		scene->renderObject(Message((int)xOri, (int)yOri, (int)x, (int)y, color), MISSILE_LAYER);
	else if (explode)
		scene->renderObject(Message((int)x, (int)y, radius, color), MISSILE_LAYER);
}

float MissilePlayer::getX()
{
	return x;
}

float MissilePlayer::getY()
{
	return y;
}

void MissilePlayer::resetMissilePlayer()
{
	active = false;
	explode = false;
	crecimiento = abs(crecimiento);
	radius = 1;
	x = y = 0;
	finalX = finalY = 0;
	Xdir = Ydir = 0;
	simTime = 0;
	color = 0;
}

bool MissilePlayer::canShoot() {
	return (!active && !explode);
}

uint32_t MissilePlayer::getColor()
{
	return color;
}

void MissilePlayer::shoot(int xDest, int yDest, int cInd)
{
	// Si no está siendo ya disparado, se activa
	if (canShoot()) {
		color = avColors[cInd];

		finalX = xDest;
		finalY = yDest;
		x = xOri;
		y = yOri;
		double vecX = (double)finalX - xOri;
		double vecY = (double)finalY - yOri;

		double vecMod = sqrt((vecX * vecX) + (vecY * vecY));
		Xdir = (float)(vecX / vecMod);
		Ydir = (float)(vecY / vecMod);

		active = true;
	}
}

bool MissilePlayer::getActive()
{
	return active;
}

bool MissilePlayer::isPointInExplosion(int x_od, int y_od)
{
	if (!explode) return false;
	return ((x_od - x) * (x_od - x) + (y_od - y) * (y_od - y)) < radius * radius;
}

void MissilePlayer::move(float fixedDeltaTime)
{
	x += (Xdir * speed * fixedDeltaTime);
	y += (Ydir * speed * fixedDeltaTime);
}

void MissilePlayer::circleExplosion(float fixedDeltaTime)
{
	radius += (int)(crecimiento * fixedDeltaTime);
	if (radius >= maxRadius) crecimiento = -crecimiento;
}
