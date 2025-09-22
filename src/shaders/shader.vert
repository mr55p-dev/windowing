#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
uniform float cosTheta;
uniform float sinTheta;
out vec3 VertexColor;

void main()
{
	gl_Position = vec4(
		pos.x * cosTheta - pos.y * sinTheta,
		pos.x * sinTheta + pos.y * cosTheta,
		pos.z,
		1.0
	);
	VertexColor = color;
}
