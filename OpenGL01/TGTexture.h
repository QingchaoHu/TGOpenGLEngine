#pragma once
#include <string>
#include <format>
#include <vector>

enum class ETGDataFormat
{
	ETGDataFormat_R = 0,
	ETGDataFormat_R16F = 1,
	ETGDataFormat_R32F = 2,
	ETGDataFormat_RG = 3,
	ETGDataFormat_RG16F = 4,
	ETGDataFormat_RG32F = 5,
	ETGDataFormat_RGB = 6,
	ETGDataFormat_RGB16F = 7,
	ETGDataFormat_RGB32F = 8,
	ETGDataFormat_RGBA = 9
};

enum ETGTextureWrapType
{
	ETGTextureAddressType_Repeat,
	ETGTextureAddressType_ClampEdge,
	ETGTextureAddressType_Mirrow,
};

enum ETGTextureFilterType
{
	ETGTextureFilterType_Linear,
	ETGTextureFilterType_Nearest,
};

enum ETGTextureType
{
	ETGTextureType_Texture2D,
	ETGTextureType_CubeMap
};

enum ETGTextureUseType
{
	// 混合
	ETGTextureUseType_Diffuse,
	// 反射
	ETGTextureUseType_Specular,
	// 环境光遮蔽
	ETGTextureUseType_Ambient,
	// 高光贴图
	ETGTextureUseType_Shininess,
	// 金属度贴图
	ETGTextureUseType_Metalness,
	// 粗糙度贴图
	ETGTextureUseType_Roughness,
	// 法线贴图
	ETGTextureUseType_Normal,
	// 凹凸贴图
	ETGTextureUseType_Bump
};

class ITGTexture
{
public:
	virtual ETGTextureUseType GetUseType() = 0;

	virtual unsigned int GetID() = 0;

	virtual ETGTextureType GetTextureType();

	virtual void SetTextureAddressType(ETGTextureWrapType sType, ETGTextureWrapType tType);

	virtual void SetTexureFilterType(ETGTextureFilterType magType, ETGTextureFilterType minType);

	virtual void SetTextureUseType(ETGTextureUseType useType);

protected:
	unsigned int mId;

	bool bIsValid;

	std::string mPath;

	ETGTextureType mTextureType;

	ETGTextureUseType mTextureUseType;
};

class TGTexture2D : public ITGTexture
{
public:
	TGTexture2D(std::string filePath, ETGTextureUseType type);

	TGTexture2D(int textureWidth, int textureHeight, ETGDataFormat format);

	virtual ETGTextureUseType GetUseType() override;

	virtual unsigned int GetID() override;

	virtual void SetTextureDataI(unsigned char* data);
	
	virtual void SetTextureDataF(float* data);

	virtual unsigned char* GetPixelValueC();

	virtual float* GetPixelValueF();

protected:
	unsigned int mTexWidth;

	unsigned int mTexHeight;

	unsigned int mTexChannels;

	ETGDataFormat mDataFormat;
};

class TGCubeTexture : public ITGTexture
{
public:
	TGCubeTexture(std::vector<std::string> cubeImagePath, ETGTextureUseType type);

	virtual ETGTextureUseType GetUseType() override;

	virtual unsigned int GetID() override;
};