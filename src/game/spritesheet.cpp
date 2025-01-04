#include "spritesheet.h"

#include "image.h"
#include "sprite.h"


SpriteSheet::SpriteSheet(const Image* img, int rows, int cols)
{
	sprites = std::vector<const Sprite*>();
	uint32_t spriteHeight = img->getHeight() / rows, spriteWidth = img->getWidth() / cols;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			sprites.push_back(new Sprite(img, spriteWidth * j, spriteHeight * i, spriteWidth, spriteHeight));
}

SpriteSheet::~SpriteSheet()
{
	for (auto s : sprites)
		delete s;
}


const Sprite* SpriteSheet::getSprite(const int& ind) const
{
	return sprites[ind];
}
