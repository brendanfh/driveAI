/* Made by Brendan Hansen */

#ifndef __GL_WRAPPER_H__
#define __GL_WRAPPER_H__

#include <iostream>
#include <GLES3/gl3.h>

class GLWrapper {
private:
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	float view_mat[16];

public:
	GLWrapper();
	~GLWrapper();

	auto SetViewport(float left, float top, float right, float bottom) -> void;
};


class GLMesh {
	friend GLWrapper;

private:
	float tx, ty;
	float sx, sy;

	float rot;

	GLuint vao;
	GLuint v_buffer;
	GLuint i_buffer;

	int vertex_count;
	float *verticies;

	int index_count;
	int *indicies;

	float color[4];

public:
	GLMesh(int max_vertex_count, int max_index_count);
	~GLMesh();

	auto Init() -> void;
	auto SetVerticies(const float *verticies, int num_verts) -> void;
	auto SetIndicies(const int *indicies, int num_inds) -> void;
	auto SetColor(float r, float g, float b, float a) -> void;
	auto Buffer() const -> void;
	auto Render() const -> void;

	auto SetPos(float x, float y) -> void;
	auto SetScale(float x, float y) -> void;
	auto SetRotation(float rot) -> void;	

	auto Translate(float dx, float dy) -> void;
	auto Scale(float sx, float sy) -> void;
	auto Rotate(float rot) -> void;
};

#endif
