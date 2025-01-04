#include "Torreta.h"
#include "messages.h"
#include "misilesscene.h"
#include "spritesheet.h"
#include "MissilePlayer.h"
#include "Cursor.h"
#include "Input.h"
#include "renderer.h"

#include "render_layers.h"
#include "player_color.h"
#include "userService.h"

Torreta::Torreta(int xPos, int yPos, int w, int h, int t, const SpriteSheet* _s, MissilePlayer* m, MisilesScene* scene_) : GameObject(scene_), width(w), height(h), type(t), s(_s), missile(m)
{
	x = xPos - (w / 2);
	y = yPos - (h / 2);

	maxRadius = (width * 3)/4;
	// La explosion durará medio segundo de crecimiento y medio para cerrarse, total 1 segundo
	crecimiento = maxRadius * 2;
	radius = 1;

	destroyed = false;
}


void Torreta::setNextTurret(Torreta* t) {
	nextTurret = t;
}

void Torreta::setType(UserId id)
{
	switch (UserService::Instance()->getUIdColor(id))
	{
	case PlayerColor::RED:
		type = 1;
		break;
	case PlayerColor::GREEN:
		type = 2;
		break;
	case PlayerColor::PINK:
		type = 3;
		break;
	default:
		type = 0;
		break;
	}
}

void Torreta::setToExplode()
{
	toExplode = true;
}

void Torreta::fixedUpdate(float fixedDeltaTime)
{
	if (toExplode) {
		radius += (int)(crecimiento * fixedDeltaTime);
		if (radius >= maxRadius) crecimiento = -crecimiento;
	}
	if (radius < 1) destroyed = true;
}

void Torreta::update(float deltaTime)
{
}

void Torreta::render()
{
	if (!destroyed) {
		if(crecimiento > 0)
			scene->renderObject(Message(s->getSprite(type),
				x, y, width, height), CITY_LAYER);
	}
	if (toExplode) {
		scene->renderObject(Message((int)(x + width/2), (int)(y+ height/2), radius, COLOR_EXPLOSION), MISSILE_LAYER);
	}
}

bool Torreta::checkCollision(float xOther, float yOther)
{
	if (!toExplode && xOther > x && xOther < (x + width) && yOther > y && yOther < (y + height)) return true;
	return false;
}

void Torreta::setDestroyed(bool b)
{
	destroyed = b;
}

void Torreta::turretReset()
{
	destroyed = false;
	toExplode = false;
	crecimiento = abs(crecimiento);
	radius = 1;
}

bool Torreta::shoot(int xOther, int yOther, int t)
{
	if(!destroyed && type == t && y >= yOther)
		if (missile->canShoot()) {
			missile->shoot(xOther, yOther, type);
			return true;
		}
	//si no se pudo disparar o si el tipo no era el correcto se intentara disparar desde el siguiente
	if (nextTurret != nullptr) {
		return nextTurret->shoot(xOther, yOther, t);
	}
	else {
		return false;
	}

}
