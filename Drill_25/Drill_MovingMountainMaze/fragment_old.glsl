#version 330

in vec3 out_Color;
out vec4 Fragcolor;

void main() {
	Fragcolor = vec4 (out_Color, 1.0);
}