#include "Cursor.h"
#include "Input.h"
#include "messages.h"
#include "misilesscene.h"
#include "spritesheet.h"
#include "render_layers.h"
#include "renderer.h"
#include "Torreta.h"
#include "userService.h"
#include "player_color.h"

#include <iostream>

Cursor::Cursor(int xPos, int yPos, int w, int h, int t, Torreta* torret, const SpriteSheet* _s, MisilesScene* scene_) : GameObject(scene_), x(xPos), y(yPos), width(w), height(h), type(t), active(false), s(_s), torreta(torret)
{
	xOri = xPos; yOri = yPos;
	lastRTrigger = 0;
	lastLTrigger = 0;
	relativeSpeed = (float)Renderer::Instance()->getWidth() / (float)1920;
	id = -1;
}

void Cursor::fixedUpdate(float fixedDeltaTime)
{
	if (!active)
		return;

	move(fixedDeltaTime);
}

void Cursor::update(float deltaTime)
{

	if (!active)
		return;


	Input* inPut = Input::Instance();
	if (inPut->getStatus(id).action_1 || inPut->getStatus(id).action_2 ||
		(inPut->getStatus(id).rightTrigger > 0.8 && lastRTrigger) || (inPut->getStatus(id).leftTrigger > 0.8 && lastLTrigger)) {
		if (!shooted)
			shooted = torreta->shoot(x, y, type);
	}
	else
		shooted = false;

	// Se calcula que el gatillo debe levantarse casi del todo antes de poder volver a disparar
	if (lastRTrigger && inPut->getStatus(id).rightTrigger > 0.8) lastRTrigger = false;
	else if (inPut->getStatus(id).rightTrigger < 0.2) lastRTrigger = true;

	if (lastLTrigger && inPut->getStatus(id).leftTrigger > 0.8) lastLTrigger = false;
	else if (inPut->getStatus(id).leftTrigger < 0.2) lastLTrigger = true;
}

void Cursor::render()
{
	if (!active)
		return;

	scene->renderObject(Message(s->getSprite(type),
		x - width / 2, y - height / 2, width, height), CURSOR_LAYER);
}

void Cursor::setDestroyed(bool b)
{

}

void Cursor::setActive(bool a)
{
	active = a;
	if(!a)
		id = -1;
	else {
		x = xOri; y = yOri;
	}
}

void Cursor::setPosition(int x_, int y_)
{
	x = x_;
	y = y_;
}

bool Cursor::getActive()
{
	return active;
}

void Cursor::setUserId(UserId id_)
{
	id = id_;
	switch (UserService::Instance()->getUIdColor(id_))
	{
	case PlayerColor::BLUE:
		type = 0;
		break;
	case PlayerColor::RED:
		type = 1;
		break;
	case PlayerColor::GREEN:
		type = 2;
		break;
	case PlayerColor::PINK:
		type = 3;
		break;
	case PlayerColor::NONE:
		break;
	}
	//Static cast de enum a int, (color --> type int) para elegir el sprite del cursor, con color.
	//type = static_cast<int>(UserService::Instance()->getUIdColor(id_));
}

UserId Cursor::getUserId()
{
	return id;
}

void Cursor::move(float fixedDeltaTime)
{

	
	InputStatus ipSts = Input::Instance()->getStatus(id);

	x += (-ipSts.angularVelY + ipSts.horizontalAxis) * CURSOR_SPEED * relativeSpeed;
	y += (-ipSts.angularVelX + ipSts.verticalAxis) * CURSOR_SPEED * relativeSpeed;


	//limits
	x = x < 0 ? 0 : x;
	x = x > Renderer::Instance()->getWidth() ? Renderer::Instance()->getWidth() : x;

	y = y < 0 ? 0 : y;
	y = y > Renderer::Instance()->getHeight() ? Renderer::Instance()->getHeight() : y;
}
