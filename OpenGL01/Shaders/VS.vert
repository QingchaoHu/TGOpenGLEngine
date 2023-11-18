#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord0;
layout (location = 4) in vec2 aTexCoord1;
layout (location = 5) in vec2 aTexCoord2;

out vec2 TexCoord;
out vec3 OutColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoord0.x, aTexCoord0.y);
    OutColor = aColor;
}