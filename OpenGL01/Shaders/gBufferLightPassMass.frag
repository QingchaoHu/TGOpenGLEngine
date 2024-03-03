#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D lightBuf;

uniform int lightBufSize;
uniform vec3 viewPos;

vec3 getTextureData(int pos, float uvStep)
{
    int rowIndex = pos / lightBufSize;
    int colIndex = pos - rowIndex * lightBufSize;
    float v = (rowIndex) * uvStep;
    float u = (colIndex) * uvStep;
    return texture(lightBuf, vec2(u, v)).rgb;
}

void main()
{
    vec3 FragPos = texture(gPosition, TexCoord).rgb;
    vec3 Normal = texture(gNormal, TexCoord).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoord).rgb;
    float Specular = texture(gAlbedoSpec, TexCoord).a;

    Normal = normalize(Normal);
    vec3 lighting = Albedo * 0.1;
    vec3 viewDir = normalize(viewPos - FragPos);

    // get light params from lightBuf;
    float uvStep = 1.0 / lightBufSize;
    vec3 lightNumberInfo = texture(lightBuf, vec2(0.0, 0.0)).rgb;
    int actPointLightNumber = int(round(lightNumberInfo.r));
    int actDirLightNumber = int(round(lightNumberInfo.g));
    int actSpotLightNumber = int(round(lightNumberInfo.b));

    // 依次处理各个点光源
    int currPos = 1;
    for (int i = 0; i < actPointLightNumber; i++)
    {
        vec3 lightPos = getTextureData(currPos, uvStep);
        vec3 lightColor = getTextureData(currPos + 1, uvStep);
        vec3 lightStrength = getTextureData(currPos + 2, uvStep);

        float constant = lightStrength.r;
        float linear = lightStrength.g;
        float quadratic = lightStrength.b;

        vec3 lightDir = normalize(lightPos - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * lightColor;

        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(viewDir, halfwayDir), 0), 4.0);
        vec3 specular = lightColor * spec * Specular;
        
        float distance = length(lightPos - FragPos);
        float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);

        diffuse *= attenuation;
        specular *= attenuation;

        lighting += diffuse;
        lighting += specular;

        currPos += 3;
    }

    for (int i = 0; i < actDirLightNumber; i++)
    {
        vec3 lightColor = getTextureData(currPos, uvStep);

        vec3 lightDir = getTextureData(currPos + 1, uvStep);

        lightDir = normalize(-lightDir);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * lightColor;

        vec3 reflectDir = reflect(-lightDir, Normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0), 2.0);
        vec3 specular = lightColor * spec * Specular;

        lighting += diffuse;
        lighting + specular;

        currPos += 2;
    }

    for (int i = 0; i < actSpotLightNumber; i++)
    {
        vec3 lightPos = getTextureData(currPos, uvStep);

        vec3 lightColor = getTextureData(currPos + 1, uvStep);

        vec3 lightDir = getTextureData(currPos + 2, uvStep);

        vec3 lightStrength = getTextureData(currPos + 3, uvStep);

        vec3 lightInfo2 = getTextureData(currPos + 4, uvStep);

        float constant = lightStrength.r;
        float linear = lightStrength.g;
        float quadratic = lightStrength.b;
        float cutoff = lightInfo2.r;

        vec3 toLightDir = normalize(lightPos - FragPos);
        float theta = dot(lightDir, toLightDir);

        if (theta > cutoff)
        {
            vec3 diffuse = max(dot(Normal, toLightDir), 0.0) * Albedo * lightColor;
            vec3 halfwayDir = normalize(toLightDir + viewDir);
            float spec = pow(max(dot(viewDir, halfwayDir), 0), 2.0);
            vec3 specular = lightColor * spec * Specular;

            float distance = length(lightPos - FragPos);
            float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);

            diffuse *= attenuation;
            specular *= attenuation;

            lighting += diffuse;
            lighting += specular;
        }

        currPos += 5;
    }

    FragColor = vec4(lighting, 1.0);
}