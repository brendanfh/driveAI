#include "./track.h"

Track::Track()
{

}

Track::~Track()
{

}

auto Track::GenerateMesh() -> void
{
	int num_lines = lines.size();
	mesh = std::make_unique<GLMesh>(8 * num_lines, 6 * num_lines);

	std::vector<float> verticies;	
	std::vector<int> indicies;

	Vector2D c1, c2, c3, c4;

	// Compute the mesh
	for (Line& l : lines) {
		Vector2D p1(l.x1, l.y1), p2(l.x2, l.y2);

		Vector2D v = (p1 - p2).Normalized();
		v = v.GetOrtho() * 0.5;

		c1 = p1 - v;
		c2 = p1 + v;
		c3 = p2 - v;
		c4 = p2 + v;

		verticies.push_back(c1.x);
		verticies.push_back(c1.y);
		verticies.push_back(c2.x);
		verticies.push_back(c2.y);
		verticies.push_back(c3.x);
		verticies.push_back(c3.y);
		verticies.push_back(c4.x);
		verticies.push_back(c4.y);
	}

	for (int i = 0, l = lines.size(); i < l; i++) {
		indicies.push_back(0 + i * 4);
		indicies.push_back(1 + i * 4);
		indicies.push_back(2 + i * 4);
		indicies.push_back(1 + i * 4);
		indicies.push_back(2 + i * 4);
		indicies.push_back(3 + i * 4);
	}

	mesh->SetVerticies(&verticies[0], verticies.size());
	mesh->SetIndicies(&indicies[0], indicies.size());
	mesh->SetScale(1, 1);
	mesh->SetPos(0, 0);
	mesh->SetColor(1, 0, 0, 1);
	mesh->Init();
	mesh->Buffer();
}

auto Track::AddLoop(const float* points, int count) -> void
{
	for (int i = 0; i < count - 1; i++) {
		Line line(
			points[i * 2 + 0], points[i * 2 + 1],
			points[i * 2 + 2], points[i * 2 + 3]
		);

		lines.push_back(line);
	}

	Line line(
		points[count * 2 - 2], points[count * 2 - 1],
		points[0], points[1]
	);
	lines.push_back(line);

	this->GenerateMesh();
}

auto Track::Intersects(const Line& line, float &x, float &y) -> bool
{
	bool intersects = false;
	float minDist = 1000000000;
	for (Line& l : lines) {
		if (l.Intersects(line)) {
			Vector2D in = l.Intersection(line);
			float dist = (in.x - line.x1) * (in.x - line.x1) + (in.y - line.y2) * (in.y - line.y2);
			if (dist < minDist) {
				minDist = dist;
				x = in.x;
				y = in.y;
			}
			intersects = true;
		}
	}
	return intersects;
}

auto Track::Render() -> void
{
	mesh->Render();
}
