#version 330 core

#define NR_POINT_LIGHTS 100
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

struct DirLight {
	vec3 direction;
	vec3 color;
};

struct PointLight {
	vec3 position;
	vec3 color;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutoff;
	vec3 color;

	float constant;
	float linear;
	float quadratic;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 OutColor;
in vec3 FragPos;
in vec3 Normal;

uniform Material material;

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int actPointLightNumber;

uniform DirLight dirLights[NR_DIR_LIGHTS];
uniform int actDirLightNumber;

uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform int actSpotLightNumber;

uniform vec3 viewPos;

float random(float x)
{
	return fract(sin(x) * 1.0);
}

vec3 CalcSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightToFragDir = normalize(light.position - fragPos);
	float theta = dot(lightToFragDir, light.direction);

	if (theta > light.cutoff)
	{
		float diff = max(dot(normal, lightToFragDir), 0.0);

		vec3 reflectDir = reflect(lightToFragDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

		float distance = length(light.position - fragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

		vec3 ambient = light.color * material.ambient;
		vec3 diffuse = light.color * (diff * material.diffuse);
		vec3 specular = light.color * (spec * material.specular);

		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		return ambient + diffuse + specular;
	}
	else
	{
		return vec3(0, 0, 0);
	}
}

vec3 CalcPointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.color * material.ambient;
	vec3 diffuse = light.color * (diff * material.diffuse);
	vec3 specular = light.color * (spec * material.specular);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return ambient + diffuse + specular;
}

vec3 CalcDirLight(DirLight light, Material material, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.color * material.ambient;
	vec3 diffuse = light.color * (diff * material.diffuse);
	vec3 specular = light.color * (spec * material.specular);

	return ambient + diffuse + specular;
}

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 lightResult;
	for (int i = 0; i < actDirLightNumber; ++i)
	{
		lightResult += CalcDirLight(dirLights[i], material, norm, viewDir);
	}

	for (int i = 0; i < actPointLightNumber; ++i)
	{
		lightResult += CalcPointLight(pointLights[i], material, norm, FragPos, viewDir);
	}

	for (int i = 0; i < actSpotLightNumber; ++i)
	{
		lightResult += CalcSpotLight(spotLights[i], material, norm, FragPos, viewDir);
	}

	// linearly interpolate between both textures (80% container, 20% awesomeface)
	vec3 mixColor = vec3(mix(texture(material.texture_diffuse1, TexCoord), texture(material.texture_diffuse2, TexCoord), 0.5) * 0.7 + vec4(OutColor * 0.3, 1.0));

	FragColor = vec4(lightResult * mixColor, 1.0);
}