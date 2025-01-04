#include "sprite.h"

#include "renderer.h"
#include "image.h"

Sprite::Sprite(const Image* img, int x, int y, int w, int h)
{
	image = img;

	tx_x = x; tx_y = y;
	tx_w = w; tx_h = h;
}

// x, y, width, height
void Sprite::draw(int dst_x, int dst_y, int dst_w, int dst_h) const
{
	Renderer::Instance()->drawImage(image, tx_x, tx_y, tx_w, tx_h, dst_x, dst_y, dst_w, dst_h);
}