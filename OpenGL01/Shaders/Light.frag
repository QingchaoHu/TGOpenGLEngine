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

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform Material material;

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int actPointLightNumber;

uniform DirLight dirLights[NR_DIR_LIGHTS];
uniform int actDirLightNumber;

uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform int actSpotLightNumber;

uniform vec3 viewPos;

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
	vec3 mixColor = vec3(mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.6) * 0.7 + vec4(OutColor * 0.3, 1.0));

	mixColor = vec3(texture(texture1, TexCoord));

	FragColor = vec4(lightResult * mixColor, 1.0);
}