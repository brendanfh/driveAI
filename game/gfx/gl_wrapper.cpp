/* Made by Brendan Hansen */

#include "gl_wrapper.h"

#include <cstring>
#include <cmath>

//These need to be accessed by both GLMesh and GLWrapper
static GLuint VIEW_MAT_LOC = 0;
static GLuint TRANS_MAT_LOC = 0;
static GLuint COLOR_LOC = 0;

static const char *vertex_shader_src =
	"attribute vec2 a_pos;"
	"uniform mat3 u_trans_mat;"
	"uniform mat4 u_view_mat;"
	"void main() {"
	"	gl_Position = u_view_mat * vec4(u_trans_mat * vec3(a_pos, 1.0), 1.0);"
	"}";

static const char *fragment_shader_src =
	"uniform vec4 u_color;"
	"void main() {"
	"	gl_FragColor = u_color;"
	"}";

static auto compile_shader(const char *src, GLint type) -> GLuint
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);	

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		std::cerr << "Error compiling shader " << type << std::endl;
	}

	return shader;
}

static auto link_program(GLuint v_shader, GLuint f_shader) -> GLuint
{
	GLuint program = glCreateProgram();
	glAttachShader(program, v_shader);
	glAttachShader(program, f_shader);
	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		std::cerr << "Error linking program" << std::endl;
	}

	return program;
}

GLWrapper::GLWrapper()
{
	vertex_shader = compile_shader(vertex_shader_src, GL_VERTEX_SHADER);
	fragment_shader = compile_shader(fragment_shader_src, GL_FRAGMENT_SHADER);
	program = link_program(vertex_shader, fragment_shader);

	VIEW_MAT_LOC = glGetUniformLocation(program, "u_view_mat");
	TRANS_MAT_LOC = glGetUniformLocation(program, "u_trans_mat");
	COLOR_LOC = glGetUniformLocation(program, "u_color");

	glUseProgram(program);
	glDisable(GL_DEPTH_TEST);

	SetViewport(-1, 1, 1, -1);
}

GLWrapper::~GLWrapper()
{
	
}

auto GLWrapper::SetViewport(float left, float top, float right, float bottom) -> void
{
	for (int i = 0; i < 16; i++)
		view_mat[i] = 0.0f;

	float
		far = 1000.0f,
		near = 0.01f;

	view_mat[0 + 0 * 4] = 2.0f / (right - left);
	view_mat[1 + 1 * 4] = 2.0f / (top - bottom);
	view_mat[2 + 2 * 4] = 2.0f / (far - near);
	view_mat[3 + 3 * 4] = 1.0f;

	view_mat[0 + 3 * 4] = -(right + left) / (right - left);
	view_mat[1 + 3 * 4] = -(top + bottom) / (top - bottom);
	view_mat[2 + 3 * 4] = -(far + near) / (far - near);

	glUniformMatrix4fv(VIEW_MAT_LOC, 1, GL_FALSE, view_mat);
}

GLMesh::GLMesh(int max_vertex_count, int max_index_count)
{
	verticies = new float[max_vertex_count];
	vertex_count = max_vertex_count;

	indicies = new int[max_index_count];
	index_count = max_index_count;

	tx = ty = 0;
	sx = sy = 1;
	rot = 0;

	vao = 0;
	v_buffer = i_buffer = 0;

	color[0] = 1;
	color[1] = 0;
	color[2] = 0;
	color[3] = 1;
}

GLMesh::~GLMesh()
{
	// glDeleteBuffers(1, &v_buffer);
	// glDeleteBuffers(1, &i_buffer);
}

auto GLMesh::Init() -> void
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &v_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * 2 * 4, verticies, GL_STATIC_DRAW);

	glGenBuffers(1, &i_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * 4, indicies, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*) 0);

	glBindVertexArray(0);
}

auto GLMesh::SetVerticies(const float *verts, int num_verts) -> void
{
	if (verticies != NULL)
		delete[] verticies;

	vertex_count = num_verts;
	verticies = new float[num_verts];
	for (int i = 0; i < num_verts; i++) {
		verticies[i] = verts[i];
	}
}

auto GLMesh::SetIndicies(const int *inds, int num_inds) -> void
{
	if (indicies != NULL)
		delete[] indicies;

	index_count = num_inds;
	indicies = new int[num_inds];
	for (int i = 0; i < num_inds; i++) {
		indicies[i] = inds[i];
	}
}

auto GLMesh::SetColor(float r, float g, float b, float a) -> void
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}

auto GLMesh::Buffer() const -> void
{
	glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_count * 2 * 4, verticies);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, index_count * 4, indicies);
}

auto GLMesh::Render() const -> void
{
	float c = cos(rot);
	float s = sin(rot);
	const float trans_mat[9] = {
		sx * c, -sx * s, 0,
		sy * s,  sy * c, 0,
		tx, ty, 1
	};

	glUniformMatrix3fv(TRANS_MAT_LOC, 1, GL_FALSE, trans_mat);
	glUniform4fv(COLOR_LOC, 1, color);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, (void*) 0);
	glBindVertexArray(0);
}

auto GLMesh::SetPos(float x, float y) -> void
{
	tx = x;
	ty = y;
}

auto GLMesh::SetScale(float x, float y) -> void
{
	sx = x;
	sy = y;
}

auto GLMesh::SetRotation(float rot) -> void
{
	this->rot = rot;
}

auto GLMesh::Translate(float dx, float dy) -> void
{
	tx += dx;
	ty += dy;
}

auto GLMesh::Scale(float sx, float sy) -> void
{
	this->sx *= sx;
	this->sy *= sy;
}

auto GLMesh::Rotate(float rot) -> void
{
	this->rot += rot;
}
