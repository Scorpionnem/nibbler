#include "render/Mesh.hpp"

Mesh::~Mesh()
{
    if (_VAO != 0)
		glDeleteVertexArrays(1, &_VAO);
	if (_VBO != 0)
		glDeleteBuffers(1, &_VBO);
}

void	Mesh::add_vertex_data(u8 *bytes, u64 size)
{
    _vertices += size / _sizeof_layout;
	_mesh_bytes.insert(_mesh_bytes.end(), bytes, bytes + size);
}

void	Mesh::add_vertex_layout(u32 location, u32 count, GLenum type, u32 offset)
{
	_vertex_layouts.push_back(VertexLayout{.location = location, .count = count, .type = type, .offset = offset});
}

void	Mesh::set_sizeof_layout(u64 size)
{
	_sizeof_layout = size;
}

void	Mesh::upload()
{
	if (_VAO != 0)
		glDeleteVertexArrays(1, &_VAO);
	if (_VBO != 0)
		glDeleteBuffers(1, &_VBO);

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);

	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _mesh_bytes.size(), _mesh_bytes.data(), GL_STATIC_DRAW);

	for (auto &v : _vertex_layouts)
	{
		glVertexAttribPointer(v.location, v.count, v.type, GL_FALSE, _sizeof_layout, (void*)v.offset);
		glEnableVertexAttribArray(v.location);
	}

	glBindVertexArray(0);
}

u64	Mesh::draw(GLenum mode)
{
	glBindVertexArray(_VAO);
	glDrawArrays(mode, 0, _vertices);
	glBindVertexArray(0);
	return (_vertices);
}
