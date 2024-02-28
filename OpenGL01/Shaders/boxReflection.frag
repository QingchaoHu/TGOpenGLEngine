#version 330 core

#define NR_POINT_LIGHTS 4
#define NR_DIR_LIGHTS 1
#define NR_SPOT_LIGHTS 2

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;

	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_specular1;
	sampler2D texture_specular2;

	samplerCube round_diffuse;
};

uniform Material material;

out vec4 FragColor;

in vec2 TexCoord;
in vec3 OutColor;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

void main()
{
    float ratio = 1.00 / 1.52;

	vec3 norm = normalize(Normal);
	vec3 viewDir = -normalize(viewPos - FragPos);

    vec3 reflectDir = reflect(viewDir, norm);

    vec3 refractDir = refract(viewDir, norm, ratio);

    vec4 skyColor = vec4(texture(material.round_diffuse, reflectDir).rgb, 1.0);

	vec4 mixColor = texture(material.texture_diffuse1, TexCoord);

	if (mixColor.a < 0.1)
	 	discard;

    FragColor = skyColor * 0.9 + mixColor * 0.1;
}