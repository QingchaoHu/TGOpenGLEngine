#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord0;
layout (location = 4) in vec2 aTexCoord1;
layout (location = 5) in vec2 aTexCoord2;
layout (location = 6) in float aCustomStencil;

out vec2 TexCoord;
out vec3 OutColor;
out vec3 FragPos;
out vec3 Normal;
out float CustomStencil;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 worldPos = projection * view * model * vec4(aPos, 1.0f);
    FragPos = (model * vec4(aPos, 1.0f)).xyz; 
    gl_Position = worldPos;
    TexCoord = aTexCoord0;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalMatrix * aNormal;

	OutColor = aColor;

    CustomStencil = aCustomStencil;
}