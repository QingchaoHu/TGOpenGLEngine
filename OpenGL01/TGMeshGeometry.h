#pragma once

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "TGTexture.h"
#include <GL/glew.h>
#include "TGMaterial.h"

class TGShaderProgram;

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

	virtual std::vector<float> GetVertexData();

	virtual std::vector<unsigned int> GetIndexData();

	unsigned int GetVertexBufferDesc();

	unsigned int GetEdgeBufferDesc();

	virtual bool Generate();

	virtual bool DrawMesh(std::shared_ptr<TGShaderProgram> shader);

	//int AddTexture(std::string texturePath, std::string type);

	//int AddTexture(std::shared_ptr<ITGTexture> texture);

	void SetMaterial(std::shared_ptr<TGMaterial> material);

	std::shared_ptr<TGMaterial> GetMaterial();

	void SetEnableDepthTest(bool useDepthTest);

	// 这个暂时不好用，其实可以做成 Lambda 输入
	void SetEnableStencilTest(bool useStencilTest, int stencilValue = 0);

	//std::shared_ptr<ITGTexture> GetTexture(int index);

protected:
	std::vector<TGVertex> mVertices;

	std::vector<unsigned int> mIndices;

	// std::vector<std::shared_ptr<ITGTexture>> mTextures;

	std::shared_ptr<TGMaterial> mMaterial;

	std::map<std::string, TGSubMeshGeometry> mSubMeshes;

	ETGMeshDrawType mMeshDrawType;

	bool bDirty = false;

	bool bInit = false;

	unsigned int mVBO;

	unsigned int mEBO;

	unsigned int mVAO;

	bool bOpenDepthTest = true;

	bool bOpenStencilTest = false;

	int mStencilValue = 22; 
};

class TGSkyBoxGeometry : public TGMeshGeometry
{
public:
	TGSkyBoxGeometry();

	virtual bool DrawMesh(std::shared_ptr<TGShaderProgram> shader) override;

	virtual bool Generate() override;

	virtual std::vector<float> GetVertexData() override;
};