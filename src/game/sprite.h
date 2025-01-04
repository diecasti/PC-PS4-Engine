#pragma once
#ifndef SPRITE_H
#define SPRITE_H

class Image;

class Sprite {
public:
	Sprite(const Image* img, int x, int y, int w, int h);
	virtual ~Sprite() = default;

	void draw(int dst_x, int dst_y, int dst_w, int dst_h) const;

protected:
	int tx_x, tx_y;
	int tx_w, tx_h;

	const Image* image;
};
#endif // SPRITE_H