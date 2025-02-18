#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 OutColor;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.6) * 0.7 + vec4(OutColor, 1.0) * 0.3;
}