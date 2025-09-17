#version 330 core

out vec4 FragColor;

uniform vec4 VertexColor;

void main()
{
	FragColor = VertexColor;
}

