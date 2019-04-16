#include "./line.h"

#define max(a, b) ((a) < (b) ? (b) : (a))
#define min(a, b) ((a) < (b) ? (a) : (b))

Line::Line()
	: x1(0), y1(0), x2(0), y2(0)
{
}

Line::Line(float x1, float y1, float x2, float y2)
	: x1(x1), y1(y1), x2(x2), y2(y2)
{
}

Line::~Line()
{
}

static auto isOnSegment(const Vector2D& p, const Vector2D& q, const Vector2D& r) -> bool {
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && 
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y)) 
       return true; 
  
    return false; 
}

static auto orientation(const Vector2D& p, const Vector2D& q, const Vector2D& r) -> int {
	int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
	if (val == 0) return 0;

	return (val > 0) ? 1 : 2;
}

auto Line::Intersects(const Line& other) const -> bool
{
	Vector2D p1(x1, y1);
	Vector2D q1(x2, y2);
	Vector2D p2(other.x1, other.y1);
	Vector2D q2(other.x2, other.y2);

	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	if (o1 != o2 && o3 != o4) return true;

	if (o1 == 0 && isOnSegment(p1, p2, q1)) return true;
	if (o2 == 0 && isOnSegment(p1, q2, q1)) return true;
	if (o3 == 0 && isOnSegment(p2, p1, q2)) return true;
	if (o4 == 0 && isOnSegment(p2, q1, q2)) return true;

	return false;
}

auto Line::Intersection(const Line& other) const -> Vector2D
{
	float s = (x1 - x2) * (other.y1 - other.y2) - (y1 - y2) * (other.x1 - other.x2);
	if (s == 0.0f) {
		return Vector2D(0, 0);
	}

	float x = (x1 * y2 - y1 * x2) * (other.x1 - other.x2) - (x1 - x2) * (other.x1 * other.y2 - other.y1 * other.x2);
	float y = (x1 * y2 - y1 * x2) * (other.y1 - other.y2) - (y1 - y2) * (other.x1 * other.y2 - other.y1 * other.x2);

	float px = x / s;
	float py = y / s;

	return Vector2D(px, py);
}