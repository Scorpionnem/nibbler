#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUv;

out vec4    fragColor;

uniform sampler2D uTex;
uniform vec3 uColor;

void main()
{
	fragColor = vec4(uColor, 1);
}
