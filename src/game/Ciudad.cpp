#include "Ciudad.h"

#include "messages.h"
#include "misilesscene.h"
#include "spritesheet.h"


#include "render_layers.h"

Ciudad::Ciudad(int _xPos, int _yPos, int _w, int _h, const SpriteSheet* _s, MisilesScene* scene_) : GameObject(scene_), xPos(_xPos), yPos(_yPos), wPos(_w), hPos(_h), s(_s)
{
	ciudadReset();


	xPosBound = xPos + (int)(wPos * 0.367);
	yPosBound = yPos + (int)(hPos * 0.355);
	wPosBound = xPosBound + (int)(wPos * 0.332);
	hPosBound = yPosBound + (int)(hPos * 0.378);
}

void Ciudad::fixedUpdate(float fixedDeltaTime)
{

}

void Ciudad::update(float deltaTime)
{
	if (!animationEnded && destroyed)
	{
		animationElapsedTime += deltaTime;

		while (animationElapsedTime > CITY_ANIM_VEL)
		{
			if (frameActual < CITY_ANIM_FRAMES) {
				frameActual++;
			}
			else {
				animationEnded = true;
				animationElapsedTime = 0;
			}
			animationElapsedTime -= CITY_ANIM_VEL;
			
		}
	}
}

void Ciudad::render()
{
	scene->renderObject(Message(s->getSprite(frameActual),
		xPos, yPos, wPos, hPos), CITY_LAYER);

	// PARA DEBUG
	//scene->renderObject(Message(xPosBound, yPosBound, wPosBound, yPosBound, 0xFFFF0000), MISSILE_LAYER);
	//scene->renderObject(Message(xPosBound, yPosBound, xPosBound, hPosBound, 0xFFFF0000), MISSILE_LAYER);
	//scene->renderObject(Message(wPosBound, yPosBound, wPosBound, hPosBound, 0xFFFF0000), MISSILE_LAYER);
	//scene->renderObject(Message(xPosBound, hPosBound, wPosBound, hPosBound, 0xFFFF0000), MISSILE_LAYER);
}

bool Ciudad::checkCollision(float x, float y)
{
	if (!destroyed && x > xPosBound && x < wPosBound && y > yPosBound && y < hPosBound) return true;
	return false;
}

void Ciudad::setDestroyed(bool b)
{
	destroyed = b;
}

void Ciudad::ciudadReset()
{
	frameActual = 0;
	animationElapsedTime = 0;

	destroyed = false;
	animationEnded = false;
}

bool Ciudad::chechAnimEnd()
{
	return animationEnded;
}
