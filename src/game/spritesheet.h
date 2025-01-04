#pragma once
#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <vector>

class Image;
class Sprite;

class SpriteSheet {
public:
	SpriteSheet(const Image* img, int rows, int cols);
	~SpriteSheet();

	const Sprite* getSprite(const int& ind) const;
private:
	std::vector<const Sprite*> sprites;
};
#endif // SPRITESHEET_H