#version 330 core

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

in vec2 TexCoord;
in vec4 OutColor;
in vec3 FragPos;
in vec3 Normal;
in float CustomStencil;

// GBuffer: 位置
layout (location = 0) out vec4 gPosition;
// GBuffer: 法线
layout (location = 1) out vec3 gNormal;
// GBuffer: 基础颜色 + 各向异性
layout (location = 2) out vec4 gAlbedoSpec;
// GBuffer: 金属度 + 高光度 + 粗糙度
layout (location = 3) out vec3 gMaterialParam;
// GBuffer: 场景深度 
layout (location = 4) out vec3 gDepth;

uniform Material material;
uniform vec3 viewPos;

void main()
{
    gPosition.rgb = FragPos;
	gPosition.a = CustomStencil;
    gNormal = normalize(Normal);

    vec3 baseTexColor = texture(material.texture_diffuse1, TexCoord).rgb;
    vec3 diffuseColor = baseTexColor * (1.0 - OutColor.a) + OutColor.rgb * OutColor.a;
    gAlbedoSpec.rgb = diffuseColor;

    gAlbedoSpec.a = texture(material.texture_specular, TexCoord).r;

	float sceneDepth = length(FragPos - viewPos);
	gDepth.r = sceneDepth;

	gMaterialParam.r = material.metalness;
	gMaterialParam.g = material.shininess;
	gMaterialParam.b = material.roughness;
}