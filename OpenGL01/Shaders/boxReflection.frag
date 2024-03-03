#version 330 core

#define NR_POINT_LIGHTS 4
#define NR_DIR_LIGHTS 1
#define NR_SPOT_LIGHTS 2

struct Material
{
	vec3 ambient;
	vec3 specular;
	vec3 diffuse;
	
	// 高光
	float shininess;
	// 金属
	float metalness;
	// 粗糙
	float roughness;

	// 颜色贴图 1 - 3
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;

	// 反射贴图 1
	sampler2D texture_specular;

	sampler2D texture_ambient;

	sampler2D texture_shininess;

	sampler2D texture_metalness;

	sampler2D texture_roughness;

	sampler2D texture_normal;

	sampler2D texture_bump;

	samplerCube texture_round;

	int diffuseTexNumber;

	int specularTexNumber;

	int ambientTexNumber;

	int shininessTexNumber;

	int metalnessTexNumber;

	int roughnessTexNumber;

	int normalTexNumber;

	int bumpTexNumber;
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

    vec4 skyColor = vec4(texture(material.texture_round, reflectDir).rgb, 1.0);

	vec4 mixColor = texture(material.texture_diffuse1, TexCoord);

	if (mixColor.a < 0.1)
	 	discard;

    FragColor = skyColor * 0.9 + mixColor * 0.1;
}