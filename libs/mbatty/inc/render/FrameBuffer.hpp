#pragma once

#include "render/GL.hpp"
#include "render/Texture.hpp"
#include "math.hpp"

struct   FrameBuffer
{
    public:
        FrameBuffer() {}
        ~FrameBuffer();

        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer& operator=(const FrameBuffer&) = delete;

        void    create(u32 width, u32 height);
        void    resize(u32 width, u32 height);

        void    bind() const;
        void    unbind() const;
        static void	clear();

        u32     id() const;
        void	bindColor(u32 unit);
		void	bindDepth(u32 unit);
		u32     colorTexture()const;
        u32     depthTexture()const;
        u32     width() const;
        u32     height() const;
    private:
        u32     _FBO = 0;
        u32     _colorTex = 0;
        u32		_depthTex = 0;
        u32     _width = 0;
        u32     _height = 0;
};
