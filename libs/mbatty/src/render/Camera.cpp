#include "render/Camera.hpp"

mat4f	Camera::getViewMatrix()
{
	return (mat4f::rotateX(radians(-pitch)) * mat4f::rotateY(radians(yaw)) * mat4f::translate(-pos));
}

mat4f	Camera::getProjectionMatrix()
{
    return (mat4f::perspective(fov, aspect, near, far));
}

vec3f    Camera::front() const
{
    float   c = std::cos(radians(pitch));

    return (vec3f(
        std::sin(radians(yaw)) * c,
        std::sin(radians(pitch)),
        -std::cos(radians(yaw)) * c));
}
