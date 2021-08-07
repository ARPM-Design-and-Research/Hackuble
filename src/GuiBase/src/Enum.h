#pragma once

/* Contains useful data structures used throughout the application
*/

#include "glm.hpp"

enum class Pivot {
	CENTER,
	TOP_LEFT,
	BOTTOM_LEFT,
	TOP_RIGHT,
	BOTTOM_RIGHT
};

struct BoundingBox {
	float x0;
	float y0;
	float x1;
	float y1;
};

struct GlyphInfo {
	glm::vec2 pos;
	glm::vec2 size;
	glm::vec4 uv;
	float advance;
	unsigned short unicode;
};

struct AtlasInfo {
	glm::vec2 size;
	float fontSize;
};

struct IconInfo {
	glm::vec2 pos;
	glm::vec2 size;
};


enum class MouseButton {
	NONE,
	LEFT,
	MIDDLE,
	RIGHT
};

enum class EventType {
	MOUSEMOVE,
	MOUSEDOWN,
	MOUSEUP,
	MOUSEWHEEL
};