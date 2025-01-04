#pragma once
#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>

enum MessageType { CLEAR, RENDER_SPRITE, PRESENT, RENDER_LINE, RENDER_CIRCLE, RENDER_FLOOR };

class Sprite;

struct RenderData {
	const Sprite* sprite;
	int x, y, w, h;
};

struct RenderLine {
	int x1, y1, x2, y2;
	uint32_t colour;
};

struct RenderCircle {
	int x, y, r;
	uint32_t colour;
};

struct RenderFloor {
	int y;
	uint32_t colour;
};

struct Message {
	MessageType type_;
	
	union {
		uint32_t clearColour;
		RenderData renderData;
		RenderLine renderLine;
		RenderCircle renderCircle;
		RenderFloor renderFloor;
	};
	

	inline Message() = default;
	// Constructora para Present
	inline Message(MessageType type) : type_(type) {}
	// Constructora para Clear
	inline Message(uint32_t colour) : type_(MessageType::CLEAR), clearColour(colour) {}
	// Constructora para Render_Floor
	inline Message(int y, uint32_t colour) : type_(MessageType::RENDER_FLOOR), renderFloor({ y, colour }) {}
	// Constructora para RenderCircle
	inline Message(int x1, int y1, int r, uint32_t colour) : type_(MessageType::RENDER_CIRCLE), renderCircle({ x1,y1,r,colour }) {}
	// Constructora para RenderLine
	inline Message(int x1, int y1, int x2, int y2, uint32_t colour) : type_(MessageType::RENDER_LINE), renderLine({ x1,y1,x2,y2,colour }) {}
	// Constructora para RenderSprite
	inline Message(const Sprite* sprite, int dx, int dy, int w, int h) : 
		type_(MessageType::RENDER_SPRITE), renderData({ sprite, dx, dy, w, h }) {}
};
#endif // MESSAGES_H