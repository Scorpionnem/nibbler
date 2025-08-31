#version 330 core
out vec4 FragColor;

in vec2 fragPos;

uniform sampler2D tex0;

void main()
{
    FragColor = vec4(texture(tex0, fragPos).rgb, 1.0);
}
