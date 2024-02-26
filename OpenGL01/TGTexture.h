#pragma once
#include <string>

class TGTexture
{
public:
	TGTexture(std::string filePath, std::string type = "");

	std::string GetType();

	unsigned int GetID();

private:
	unsigned int mId;
	std::string mType;
	bool bIsValid = false;
	std::string mPath;
};