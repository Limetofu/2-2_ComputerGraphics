#version 330 core

in vec3 FragPos;

out vec4 Fragcolor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
//uniform vec3 viewPos;

void main() {
	float ambientLight = 0.5;
	vec3 ambient = ambientLight * lightColor;
	
	vec3 normalVector = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diffuseLight = max(dot(normalVector, lightDir), 0.0);
	vec3 diffuse = diffuseLight * lightColor;

	vec3 result = (ambient + diffuse) * objectColor;

	Fragcolor = vec4(result, 1.0);
}