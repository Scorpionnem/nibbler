#pragma once

#include "math.hpp"

struct   Camera
{
    mat4f	getViewMatrix();
    mat4f	getProjectionMatrix();

    vec3f    front() const;

    vec3f   pos;
    float  yaw = 0;
    float  pitch = 0;
    float  fov = 0;
    float  aspect = 0;
    float  near = 0;
    float  far = 0;
};
