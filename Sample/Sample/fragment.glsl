#version 330 core

in vec3 outColor;
out vec4 color;

void main()
{
    color = vec4(outColor, 1.0);		//--- 빨강색으로 색상 고정
}