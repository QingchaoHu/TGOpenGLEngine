#version core 330

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