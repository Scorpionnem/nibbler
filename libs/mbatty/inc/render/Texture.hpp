#pragma once

#include "math.hpp"
#include "render/GL.hpp"

#include <vector>

struct	Texture
{
	public:
		Texture() {}
		~Texture();

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		void	add_pixel_data(u8* bytes, u64 size);
		void	clear_pixel_data();
		void	set_format(u32 width, u32 height, GLenum internal_format = GL_RGBA8, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);

		void	upload();
		void	bind(u32 unit) const;
		static void	bind(GLuint id, u32 unit);

		u32		id() const {return (_id);}
		u32		width() const {return (_width);}
		u32		height() const {return (_height);}
	private:
	    void    _generateUnknownTexture();
		u32				_id = 0;

		std::vector<u8>	_pixels;
		u32				_width = 0;
		u32				_height = 0;

		GLenum			_internal_format = GL_RGBA8;
		GLenum			_format = GL_RGBA;
		GLenum			_type = GL_UNSIGNED_BYTE;
};
