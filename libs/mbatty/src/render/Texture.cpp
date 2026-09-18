#include "render/Texture.hpp"

Texture::~Texture()
{
	if (_id != 0)
		glDeleteTextures(1, &_id);
}

void	Texture::add_pixel_data(u8* bytes, u64 size)
{
	_pixels.insert(_pixels.end(), bytes, bytes + size);
}

void	Texture::clear_pixel_data()
{
	_pixels.clear();
}

void	Texture::set_format(u32 width, u32 height, GLenum internal_format, GLenum format, GLenum type)
{
	_width = width;
	_height = height;
	_internal_format = internal_format;
	_format = format;
	_type = type;
}

void	Texture::upload()
{
    if (_pixels.empty())
		_generateUnknownTexture();

	if (_id != 0)
		glDeleteTextures(1, &_id);

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, _width, _height, 0, _format, _type, _pixels.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (_format == GL_RED)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void	Texture::bind(u32 unit) const
{
	Texture::bind(_id, unit);
}

void	Texture::bind(GLuint id, u32 unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, id);
}

void    Texture::_generateUnknownTexture()
{
	_pixels = {255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 255};
	_width = 2;
	_height = 2;
	_format = GL_RGBA;
}
