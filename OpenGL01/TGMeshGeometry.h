#pragma once

#include "ThirdParty/glm/glm.hpp"
#include <map>
#include <vector>
#include <string>

enum class ETGMeshDrawType : unsigned char
{
	TGMeshDrawType_Static = 0,
	TGMeshDrawType_Dynamic = 1,
	TGMeshDrawType_Stream = 2,
};

class TGVertex
{
public:
	TGVertex() 
	{
		mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		mNormal = glm::vec3(0.0f, 0.0f, 1.0f);
		mColor = glm::vec3(1.0f, 1.0f, 1.0f);
		mTexCoord0 = glm::vec2(0.0f, 0.0f);
		mTexCoord1 = glm::vec2(0.0f, 0.0f);
		mTexCoord2 = glm::vec2(0.0f, 0.0f);
	}

	TGVertex(glm::vec3 position)
	{
		mPosition = position;
		mNormal = glm::vec3(0.0f, 0.0f, 1.0f);
		mColor = glm::vec3(1.0f, 1.0f, 1.0f);
		mTexCoord0 = glm::vec2(0.0f, 0.0f);
		mTexCoord1 = glm::vec2(0.0f, 0.0f);
		mTexCoord2 = glm::vec2(0.0f, 0.0f);
	}

	TGVertex(glm::vec3 position, glm::vec2 texCoor, glm::vec3 color)
	{
		mPosition = position;
		mTexCoord0 = texCoor;
		mColor = color;
		mNormal = glm::vec3(0.0f, 0.0f, 1.0f);
		mTexCoord1 = glm::vec2(0.0f, 0.0f);
		mTexCoord2 = glm::vec2(0.0f, 0.0f);
	}

	glm::vec3 mPosition;
	glm::vec3 mNormal;
	glm::vec3 mColor;
	glm::vec2 mTexCoord0;
	glm::vec2 mTexCoord1;
	glm::vec2 mTexCoord2;
};

struct TGSubMeshGeometry
{
	int mIndexStartIndex;
	int mIndexNumber;
	int mVertexStartIndex;
};

class TGMeshGeometry
{
public:
	TGMeshGeometry();

	bool AddSubMesh(std::string SubmeshName, std::vector<TGVertex> VertexArray, std::vector<unsigned int> IndexArray);

	std::vector<float> GetVertexData();

	std::vector<unsigned int> GetIndexData();

	unsigned int GetVertexBufferDesc();

	unsigned int GetEdgeBufferDesc();

	bool GenerateBuffer();

	bool UseBuffer();

	bool DrawMesh();

private:
	std::vector<TGVertex> mVertices;

	std::vector<unsigned int> mIndices;

	std::map<std::string, TGSubMeshGeometry> mSubMeshes;

	ETGMeshDrawType mMeshDrawType;

	bool bDirty = false;

	bool bInit = false;

	unsigned int mVBO;

	unsigned int mEBO;
};