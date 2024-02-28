#pragma once
#include <string>
#include <format>
#include <vector>

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

class TGTexture : public ITGTexture
{
public:
	TGTexture(std::string filePath, std::string type = "");

	virtual std::string GetType() override;

	virtual unsigned int GetID() override;
};

class TGCubeTexture : public ITGTexture
{
public:
	TGCubeTexture(std::vector<std::string> cubeImagePath, std::string type);

	virtual std::string GetType() override;

	virtual unsigned int GetID() override;
};