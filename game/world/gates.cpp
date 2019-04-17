#include "./gates.h"

Gates::Gates()
{
    target = 0;
    gates = std::vector<Line>();
	mesh = std::make_unique<GLMesh>(8, 6);
	mesh->SetScale(1, 1);
	mesh->SetPos(0, 0);
	mesh->SetColor(0, 1, 0, 1);
	mesh->Init();
}

Gates::~Gates()
{

}

auto Gates::AddGate(float x1, float y1, float x2, float y2) -> void
{
    gates.push_back(Line(x1, y1, x2, y2));
}

auto Gates::HitTarget(const Line& l) -> bool
{
    Line& tar = gates[target];
    bool hit = tar.Intersects(l);
    if (hit) {
        target += 1;
        if (target >= gates.size()) target = 0;
    }

    return hit;
}

auto Gates::Reset() -> void
{
    target = 0;
}

auto Gates::Render() -> void
{
    GenerateMesh();
    mesh->Render();
}

auto Gates::GenerateMesh() -> void
{
	std::vector<float> verticies;	

	Vector2D c1, c2, c3, c4;

    Line& l = gates[target];
    Vector2D p1(l.x1, l.y1), p2(l.x2, l.y2);

    Vector2D v = (p1 - p2).Normalized();
    v = v.GetOrtho() * 0.2;

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

    int indicies[] = { 0, 1, 2, 1, 2, 3 };

	mesh->SetVerticies(&verticies[0], verticies.size());
	mesh->SetIndicies(indicies, 6);
	mesh->Buffer();
}