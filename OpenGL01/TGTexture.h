#pragma once
#include <string>

enum ETGTextureAddressType
{
	ETGTextureAddressType_Repeat,
	ETGTextureAddressType_ClampEdge,
	ETGTextureAddressType_Mirrow,
};

class TGTexture
{
public:
	TGTexture(std::string filePath, std::string type = "");

	void SetTextureAddressType(ETGTextureAddressType sType, ETGTextureAddressType tType);

	std::string GetType();

	unsigned int GetID();

private:
	unsigned int mId;
	std::string mType;
	bool bIsValid = false;
	std::string mPath;
};