#pragma once

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "TGTexture.h"
#include <GL/glew.h>
#include "TGMaterial.h"
#include <iostream>

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

struct Box {
	glm::vec3 min = glm::vec3(0.0f, 0.0f, 0.0f );
	glm::vec3 max = glm::vec3(0.0f, 0.0f, 0.0f );
	Box() {};
	Box(Box &box) {
		max = box.max;
		min = box.min;
	}
	Box set(Box &box) {
		max = box.max;
		min = box.min;

		return *this;
	}
	Box reset(const glm::vec3 &point) {
		min.x = point.x;
		min.y = point.y;
		min.z = point.z;
		max.x = point.x;
		max.y = point.y;
		max.z = point.z;

		return *this;
	}
	Box expand(const glm::vec3 &point) {
		if (point.x < min.x) min.x = point.x;
		if (point.y < min.y) min.y = point.y;
		if (point.z < min.z) min.z = point.z;
		if (point.x > max.x) max.x = point.x;
		if (point.y > max.y) max.y = point.y;
		if (point.z > max.z) max.z = point.z;

		return *this;
	}
	Box expand(const Box &box) {
		expand(box.min);
		expand(box.max);

		return *this;
	}
	 glm::vec3 getCenter(glm::vec3 &result) {
		result.x = (min.x + max.x) / 2;
		result.y = (min.y + max.y) / 2;
		result.z = (min.z + max.z) / 2;

		return result;
	 }
	 glm::vec3 getSize(glm::vec3 &result) {
		result.x = max.x - min.x;
		result.y = max.y - min.y;
		result.z = max.z - min.z;

		return result;
	 }
	 const glm::vec3 getSize() {
		glm::vec3 result;
		getSize(result);

		return result;
	 }
	friend std::ostream &operator<<(std::ostream &os, Box &box);
};

struct Sphere {
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f );
    float radius = 0;
	Sphere(Box &box) {
		box.getCenter(center);
		const glm::vec3 size = box.getSize();
		radius = fmax(fmaxf(size.x, size.y), size.z) / 2;
	}
	void fromBox(Box &box) {
		box.getCenter(center);
		const glm::vec3 size = box.getSize();
		radius = fmax(fmaxf(size.x, size.y), size.z) / 2;
	}
	friend std::ostream &operator<<(std::ostream &os, Sphere &s);
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

	std::shared_ptr<Box> mBoundingBox;

	std::shared_ptr<Sphere> mbBoundingSphere;

	Box computeBoundingBox(bool force = false);

	Sphere computeBoundingSphere(bool force = false);

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