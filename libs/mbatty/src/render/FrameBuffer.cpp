#include "render/FrameBuffer.hpp"

FrameBuffer::~FrameBuffer()
{
    if (_FBO != 0)
        glDeleteFramebuffers(1, &_FBO);
    if (_colorTex != 0)
        glDeleteTextures(1, &_colorTex);
    if (_depthTex != 0)
        glDeleteTextures(1, &_depthTex);
}

void    FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void    FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
    glViewport(0, 0, _width, _height);
}

void	FrameBuffer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void    FrameBuffer::create(u32 width, u32 height)
{
    _width = width;
    _height = height;

    glGenFramebuffers(1, &_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

    glGenTextures(1, &_colorTex);
    glBindTexture(GL_TEXTURE_2D, _colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorTex, 0);

    glGenTextures(1, &_depthTex);
    glBindTexture(GL_TEXTURE_2D, _depthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTex, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Framebuffer: incomplete");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void    FrameBuffer::resize(u32 width, u32 height)
{
    if (width == _width && height == _height)
        return ;
    if (_FBO != 0)
        glDeleteFramebuffers(1, &_FBO);
    if (_colorTex != 0)
        glDeleteTextures(1, &_colorTex);
    if (_depthTex != 0)
        glDeleteTextures(1, &_depthTex);
    create(width, height);
}

void	FrameBuffer::bindColor(u32 unit) {Texture::bind(_colorTex, unit);}
void	FrameBuffer::bindDepth(u32 unit) {Texture::bind(_depthTex, unit);}

u32     FrameBuffer::colorTexture() const {return (_colorTex);}
u32     FrameBuffer::depthTexture() const {return (_depthTex);}
u32     FrameBuffer::width() const {return (_width);}
u32     FrameBuffer::height() const {return (_height);}

u32     FrameBuffer::id() const {return (_FBO);}
