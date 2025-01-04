#include "Suelo.h"



#include "misilesscene.h"
#include "messages.h"
#include "render_layers.h"

Suelo::Suelo(int _yPos, uint32_t c, MisilesScene* scene_) : GameObject(scene_), yPos(_yPos), color(c)
{
}

void Suelo::fixedUpdate(float fixedDeltaTime)
{
}

void Suelo::update(float deltaTime)
{
}

void Suelo::render()
{
	scene->renderObject(Message(yPos, color), STATIC_BACKGROUND_LAYER);
}