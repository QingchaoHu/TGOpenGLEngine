#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord0;
layout (location = 4) in vec2 aTexCoord1;
layout (location = 5) in vec2 aTexCoord2;
layout (location = 6) in float aCustomStencil;

struct InstanceProxy
{
    sampler2D texture_transform;
    sampler2D texture_custom;
    int per_custom_number;
    int transform_texture_size;
    int custom_texture_size;
};

out vec2 TexCoord;
out vec4 OutColor;
out vec3 FragPos;
out vec3 Normal;
out float CustomStencil;

uniform mat4 view;
uniform mat4 projection;

uniform InstanceProxy instance;

float GetValueFromInstanceCustomTexture(int locationId)
{
    if (instance.per_custom_number == 0 || instance.custom_texture_size == 0) return 0.0;

    int gridLoc = locationId / 4;
    int dataIndex = locationId - gridLoc * 4;

    int row = gridLoc / instance.custom_texture_size;
    int col = gridLoc - instance.custom_texture_size * row;
    float uvScale = 1.0 / instance.custom_texture_size;

    float u = col * uvScale;
    float v = row * uvScale;

    vec4 gridValue = texture(instance.texture_custom, vec2(u, v)).rgba;

    if (dataIndex == 0) return gridValue.r;

    if (dataIndex == 1) return gridValue.g;

    if (dataIndex == 2) return gridValue.b;

    if (dataIndex == 3) return gridValue.a;

    return 0.0;
}

vec4 GetVec4FromInstanceTexture(int locationId)
{
    int row = locationId / instance.transform_texture_size;
    int col = locationId - instance.transform_texture_size * row;
    float uvScale = 1.0 / instance.transform_texture_size;

    float u = col * uvScale;
    float v = row * uvScale;

    return texture(instance.texture_transform, vec2(u, v)).rgba;
}

mat4 GetMat4FromInstanceTexture(int instanceId)
{
    int startLoc = instanceId * 4;

    vec4 row1 = GetVec4FromInstanceTexture(startLoc);
    vec4 row2 = GetVec4FromInstanceTexture(startLoc + 1);
    vec4 row3 = GetVec4FromInstanceTexture(startLoc + 2);
    vec4 row4 = GetVec4FromInstanceTexture(startLoc + 3);

    return mat4(row1, row2, row3, row4);
}

void main()
{
    mat4 model = GetMat4FromInstanceTexture(gl_InstanceID);

    int customDataStartIndex = gl_InstanceID * instance.per_custom_number;
    OutColor.r = GetValueFromInstanceCustomTexture(customDataStartIndex);
    OutColor.g = GetValueFromInstanceCustomTexture(customDataStartIndex + 1);
    OutColor.b = GetValueFromInstanceCustomTexture(customDataStartIndex + 2);
    OutColor.a = GetValueFromInstanceCustomTexture(customDataStartIndex + 3);

	vec4 worldPos = projection * view * model * vec4(aPos, 1.0f);
    FragPos = (model * vec4(aPos, 1.0f)).xyz; 
    gl_Position = worldPos;
    TexCoord = aTexCoord0;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalMatrix * aNormal;

    CustomStencil = aCustomStencil;
}