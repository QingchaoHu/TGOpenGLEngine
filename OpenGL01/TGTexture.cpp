#include "TGTexture.h"
#include "GL/glew.h"
#include "stb_image.h"
#include <iostream>

TGTexture2D::TGTexture2D(std::string filePath, std::string type)
{
	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_2D, mId);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		mTexWidth = width;
		mTexHeight = height;
		mTexChannels = nrChannels;

		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		if (nrChannels == 3)
		{
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			mDataFormat = ETGDataFormat::ETGDataFormat_RGB;
		}
		else if (nrChannels == 4)
		{
			// alpha 纹理需要设置为 钳制到边缘 以避免和下一轮重复值进行插值
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			mDataFormat = ETGDataFormat::ETGDataFormat_RGBA;
		}
		else if (nrChannels == 1)
		{
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R, width, height, 0, GL_R, GL_UNSIGNED_BYTE, data);
			mDataFormat = ETGDataFormat::ETGDataFormat_R;
		}
		else if (nrChannels == 2)
		{
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
			mDataFormat = ETGDataFormat::ETGDataFormat_RG;
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		bIsValid = true;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	mType = type;
	mPath = filePath;
	mTextureType = ETGTextureType_Texture2D;
}

TGTexture2D::TGTexture2D(int textureWidth, int textureHeight, ETGDataFormat format)
{
	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_2D, mId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (format == ETGDataFormat::ETGDataFormat_RGB)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_UNSIGNED_BYTE, GL_RGB, NULL);
	}
	else if (format == ETGDataFormat::ETGDataFormat_RGBA)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_UNSIGNED_BYTE, GL_RGBA, NULL);
	}
	else if (format == ETGDataFormat::ETGDataFormat_R16F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, textureWidth, textureWidth, 0, GL_FLOAT, GL_R, NULL);
	}
	else if (format == ETGDataFormat::ETGDataFormat_R32F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, textureWidth, textureWidth, 0, GL_FLOAT, GL_R, NULL);
	}
	else if (format == ETGDataFormat::ETGDataFormat_RG16F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, textureWidth, textureWidth, 0, GL_FLOAT, GL_RG, NULL);
	}
	else if (format == ETGDataFormat::ETGDataFormat_RG32F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, textureWidth, textureWidth, 0, GL_FLOAT, GL_RG, NULL);
	}
	else if (format == ETGDataFormat::ETGDataFormat_RGB16F)
	{	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, textureWidth, textureHeight, 0, GL_FLOAT, GL_RGB, NULL);
	}
	else if (format == ETGDataFormat::ETGDataFormat_RGB32F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, textureWidth, textureHeight, 0, GL_FLOAT, GL_RGB, NULL);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

std::string TGTexture2D::GetType()
{
	return mType;
}

unsigned int TGTexture2D::GetID()
{
	return mId;
}

void TGTexture2D::SetTextureDataI(unsigned char* data)
{
	glBindTexture(GL_TEXTURE_2D, mId);

	if (mDataFormat == ETGDataFormat::ETGDataFormat_R)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R, mTexWidth, mTexHeight, 0, GL_UNSIGNED_BYTE, GL_R, data);
	}
	else if (mDataFormat == ETGDataFormat::ETGDataFormat_RG)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, mTexWidth, mTexHeight, 0, GL_UNSIGNED_BYTE, GL_RG, data);
	}
	else if (mDataFormat == ETGDataFormat::ETGDataFormat_RGB)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mTexWidth, mTexHeight, 0, GL_UNSIGNED_BYTE, GL_RGB, data);
	}
	else if (mDataFormat == ETGDataFormat::ETGDataFormat_RGBA)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTexWidth, mTexHeight, 0, GL_UNSIGNED_BYTE, GL_RGBA, data);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TGTexture2D::SetTextureDataF(float* data)
{
	glBindTexture(GL_TEXTURE_2D, mId);

	if (mDataFormat == ETGDataFormat::ETGDataFormat_R16F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, mTexWidth, mTexHeight, 0, GL_FLOAT, GL_R, data);
	}
	else if (mDataFormat == ETGDataFormat::ETGDataFormat_R32F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, mTexWidth, mTexHeight, 0, GL_FLOAT, GL_RG, data);
	}
	else if (mDataFormat == ETGDataFormat::ETGDataFormat_RG16F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, mTexWidth, mTexHeight, 0, GL_FLOAT, GL_RG, data);
	}
	else if (mDataFormat == ETGDataFormat::ETGDataFormat_RG32F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, mTexWidth, mTexHeight, 0, GL_FLOAT, GL_RG, data);
	}
	else if (mDataFormat == ETGDataFormat::ETGDataFormat_RGB16F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, mTexWidth, mTexHeight, 0, GL_FLOAT, GL_RGB, data);
	}
	else if (mDataFormat == ETGDataFormat::ETGDataFormat_RGB32F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, mTexWidth, mTexHeight, 0, GL_FLOAT, GL_RGB, data);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

int TGTexture2D::GetPixelValueAtI()
{
	glBindTexture()
}

float TGTexture2D::GetPixelValueAtF()
{

}

TGCubeTexture::TGCubeTexture(std::vector<std::string> cubeImagePath, std::string type)
{
	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mId);

	int width, height, nChannels;

	for (int i = 0; i < cubeImagePath.size(); i++)
	{
		unsigned char* imageData = stbi_load(cubeImagePath[i].c_str(), &width, &height, &nChannels, 0);

		if (imageData)
		{
			// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
			if (nChannels == 3)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			}
			else if (nChannels == 4)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
			}
			else if (nChannels == 1)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R, width, height, 0, GL_R, GL_UNSIGNED_BYTE, imageData);
			}
			else if (nChannels == 2)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, imageData);
			}

			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			bIsValid = true;
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(imageData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	mType = type;
	mTextureType = ETGTextureType_CubeMap;
}

std::string TGCubeTexture::GetType()
{
	return mType;
}

unsigned int TGCubeTexture::GetID()
{
	return mId;
}

ETGTextureType ITGTexture::GetTextureType()
{
	return mTextureType;
}

void ITGTexture::SetTextureAddressType(ETGTextureAddressType sType, ETGTextureAddressType tType)
{
	glBindTexture(GL_TEXTURE_2D, mId);

	if (sType == ETGTextureAddressType_ClampEdge)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	}
	else if (sType == ETGTextureAddressType_Repeat)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	}

	if (tType == ETGTextureAddressType_ClampEdge)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else if (sType == ETGTextureAddressType_Repeat)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}
}
