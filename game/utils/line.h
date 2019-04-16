/* Made by Brendan Hansen */

#ifndef __LINE_H__
#define __LINE_H__

#include "./vector2D.h"

class Line {
private:
public:
	float x1, y1;
	float x2, y2;
	
	Line();
	Line(float x1, float y1, float x2, float y2);
	~Line();

	auto Intersects(const Line& other) const -> bool;
	auto Intersection(const Line& other) const -> Vector2D;
};

#endif