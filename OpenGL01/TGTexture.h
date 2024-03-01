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

enum ETGTextureAddressType
{
	ETGTextureAddressType_Repeat,
	ETGTextureAddressType_ClampEdge,
	ETGTextureAddressType_Mirrow,
};

enum ETGTextureType
{
	ETGTextureType_Texture2D,
	ETGTextureType_CubeMap
};

class ITGTexture
{
public:
	virtual std::string GetType() = 0;

	virtual unsigned int GetID() = 0;

	virtual ETGTextureType GetTextureType();

	virtual void SetTextureAddressType(ETGTextureAddressType sType, ETGTextureAddressType tType);

protected:
	unsigned int mId;

	std::string mType;

	bool bIsValid;

	std::string mPath;

	ETGTextureType mTextureType;
};

class TGTexture2D : public ITGTexture
{
public:
	TGTexture2D(std::string filePath, std::string type = "");

	TGTexture2D(int textureWidth, int textureHeight, ETGDataFormat format);

	virtual std::string GetType() override;

	virtual unsigned int GetID() override;

	virtual void SetTextureDataI(unsigned char* data);
	
	virtual void SetTextureDataF(float* data);

	virtual int GetPixelValueAtI();

	virtual float GetPixelValueAtF();

protected:
	unsigned int mTexWidth;

	unsigned int mTexHeight;

	unsigned int mTexChannels;

	ETGDataFormat mDataFormat;
};

class TGCubeTexture : public ITGTexture
{
public:
	TGCubeTexture(std::vector<std::string> cubeImagePath, std::string type);

	virtual std::string GetType() override;

	virtual unsigned int GetID() override;
};