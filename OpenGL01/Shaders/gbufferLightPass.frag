#version 330 core

#define NR_POINT_LIGHTS 200
#define NR_DIR_LIGHTS 1
#define NR_SPOT_LIGHTS 10

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D texture0;

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

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int actPointLightNumber;

uniform DirLight dirLights[NR_DIR_LIGHTS];
uniform int actDirLightNumber;

uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform int actSpotLightNumber;

uniform vec3 viewPos;

void main()
{
    vec3 FragPos = texture(gPosition, TexCoord).rgb;
    vec3 Normal = texture(gNormal, TexCoord).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoord).rgb;
    float Specular = texture(gAlbedoSpec, TexCoord).a;

    Normal = normalize(Normal);

    vec3 lighting = Albedo * 0.0;
    vec3 viewDir = normalize(viewPos - FragPos);

    for (int i = 0; i < actPointLightNumber; i++)
    {
        PointLight light = pointLights[i];

        vec3 lightDir = normalize(light.position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * light.color;

        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(viewDir, halfwayDir), 0), 1.0);
        vec3 specular = light.color * spec * Specular;
        
        float distance = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

        diffuse *= attenuation;
        specular *= attenuation;

        lighting += diffuse;
        lighting += specular;
    }

    for (int i = 0; i < actDirLightNumber; i++)
    {
        DirLight light = dirLights[i];

        vec3 lightDir = normalize(-light.direction);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * light.color;

        vec3 reflectDir = reflect(-lightDir, Normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0), 16);
        vec3 specular = light.color * spec * Specular;

        lighting += diffuse + specular;
    }

    for (int i = 0; i < actSpotLightNumber; i++)
    {
        SpotLight light = spotLights[i];

        vec3 lightDir = normalize(light.position - FragPos);
        float theta = dot(light.direction, lightDir);

        if (theta > light.cutoff)
        {
            vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * light.color;
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(viewDir, halfwayDir), 0), 16);
            vec3 specular = light.color * spec * Specular;

            float distance = length(light.position - FragPos);
            float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

            diffuse *= attenuation;
            specular *= attenuation;

            lighting += diffuse + specular;
        }
    }

    float gamma = 2.2;
    lighting = pow(lighting, 2);

    FragColor = vec4(lighting, 1.0);
}