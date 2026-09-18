#pragma once

#include "math.hpp"
#include "render/GL.hpp"

struct   Shader
{
    public:
        Shader() {}
        ~Shader() {}

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void    load(const char *vert_path, const char *frag_path);
        void    unload();
        void    reload();
        void    bind() const;

        void    setInt(const char *name, i32 value) const;
        void    setFloat(const char *name, float value) const;
        void    setVec3(const char *name, const vec3f& v) const;
        void    setVec2(const char *name, const vec2f& v) const;
        void    setVec4(const char *name, const vec4f& v) const;
        void    setMat4(const char *name, const mat4f& m) const;
    private:
        u32 _program = 0;
        std::string _frag_path;
        std::string _vert_path;
};
