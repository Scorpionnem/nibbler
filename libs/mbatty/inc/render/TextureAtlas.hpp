#pragma once

#include "loader/texture/STBLoader.hpp"
#include "render/Texture.hpp"
#include "utils/AABB.hpp"
#include "vec.hpp"

#include <algorithm>
#include <climits>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

struct   TextureAtlas
{
    public:
        TextureAtlas() {}
        ~TextureAtlas() {}

        TextureAtlas(const TextureAtlas&) = delete;
        TextureAtlas& operator=(const TextureAtlas&) = delete;

        void        add_texture(const std::string& path);
        void	add_texture(const std::string& path, const std::vector<u8>& pixels, int width, int height, int channels, GLenum format);

        void        upload();

        void        bind(u32 unit) const;
        bool        has(const std::string& path) const;

        vec4f       uv(const std::string& path) const;

        const Texture&  texture() const;
        u32         size() const;

    private:
        bool    _isSpotFree(const aabb2i& target_box) const;

        bool    _findSpot(const vec2i& size, vec2i& out_pos);

        void    _sizeUp();

        void    _insertTexture(const std::vector<u8>& rgba, int width, int height, const vec2i& pos);

        static std::vector<u8> _toRGBA(const std::vector<u8>& data, int width, int height, int channels);

        Texture     _texture;

        std::vector<u8> _pixels;
        u32             _n = 1; // power of 2 size of the atlas
        u32             _size = 0; // always a square
        u32             _channels = 4;

        int smallest = INT_MAX;

        int last_x = 0;
        int last_y = 0;

        std::map<std::string, aabb2i>   _uvs;
};
