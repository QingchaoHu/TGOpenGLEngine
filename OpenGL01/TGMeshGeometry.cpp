#include <math.h>
#include "TGMeshGeometry.h"
#include "TGTexture.h"
#include "TGShaderProgram.h"


std::ostream &operator<<(std::ostream &os, Box &box) {
	const auto max = box.max;
	const auto min = box.min;
	os << "Box { max: [" << max.x << ", " << max.y << ", " << max.z << "], min: [" << min.x << ", " << min.y << ", " << min.z << "] }";

	return os;
}

std::ostream &operator<<(std::ostream &os, Sphere &s) {
	const auto center = s.center;
	const auto radius = s.radius;
	os << "Sphere { center: [" << center.x << ", " << center.y << ", " << center.z << "], radius: [" << radius << "}";

	return os;
}

TGMeshGeometry::TGMeshGeometry()
{
	mMeshDrawType = ETGMeshDrawType::TGMeshDrawType_Static;

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);
}

bool TGMeshGeometry::AddSubMesh(std::string SubmeshName, std::vector<TGVertex> VertexArray, std::vector<unsigned int> IndexArray)
{
	TGSubMeshGeometry subMeshInfo;
	subMeshInfo.mVertexStartIndex = mVertices.size();
	subMeshInfo.mIndexNumber = IndexArray.size();
	subMeshInfo.mIndexStartIndex = mIndices.size();

	mVertices.insert(mVertices.end(), VertexArray.begin(), VertexArray.end());
	mIndices.insert(mIndices.end(), IndexArray.begin(), IndexArray.end());

	mSubMeshes.insert({ SubmeshName, subMeshInfo });

	bDirty = true;

	return true;
}

std::vector<float> TGMeshGeometry::GetVertexData()
{
	std::vector<float> vertexData;

	for (int i = 0; i < mVertices.size(); ++i)
	{
		TGVertex vertex = mVertices[i];
		vertexData.push_back(vertex.mPosition.x);
		vertexData.push_back(vertex.mPosition.y);
		vertexData.push_back(vertex.mPosition.z);
		vertexData.push_back(vertex.mColor.x);
		vertexData.push_back(vertex.mColor.y);
		vertexData.push_back(vertex.mColor.z);
		vertexData.push_back(vertex.mNormal.x);
		vertexData.push_back(vertex.mNormal.y);
		vertexData.push_back(vertex.mNormal.z);
		vertexData.push_back(vertex.mTexCoord0.x);
		vertexData.push_back(vertex.mTexCoord0.y);
		vertexData.push_back(vertex.mTexCoord1.x);
		vertexData.push_back(vertex.mTexCoord1.y);
		vertexData.push_back(vertex.mTexCoord2.x);
		vertexData.push_back(vertex.mTexCoord2.y);
		vertexData.push_back(mStencilValue);
	}

	return vertexData;
}

Box TGMeshGeometry::computeBoundingBox(bool force) {
	if (mBoundingBox && !force) return *mBoundingBox;

	if (!mBoundingBox) mBoundingBox = std::make_shared<Box>();

	for (std::vector<TGVertex>::iterator it = mVertices.begin(); it != mVertices.end(); it++) {
		if (it == mVertices.begin()) {
			mBoundingBox->reset(it->mPosition);
			continue;
		}
		mBoundingBox->expand(it->mPosition);
	}

	return *mBoundingBox;
}

Sphere TGMeshGeometry::computeBoundingSphere(bool force) {
	if (mbBoundingSphere && !force) return *mbBoundingSphere;

	computeBoundingBox();

	if (!mbBoundingSphere) mbBoundingSphere = std::make_shared<Sphere>(Sphere(*mBoundingBox));

	return *mbBoundingSphere;
}

std::vector<unsigned int> TGMeshGeometry::GetIndexData()
{
	return mIndices;
}

unsigned int TGMeshGeometry::GetVertexBufferDesc()
{
	return mVBO;
}

unsigned int TGMeshGeometry::GetEdgeBufferDesc()
{
	return mEBO;
}

bool TGMeshGeometry::Generate()
{
	std::vector<float> vertices = GetVertexData();
	std::vector<unsigned int> indices = GetIndexData();

	GLenum GLDrawType;
	if (mMeshDrawType == ETGMeshDrawType::TGMeshDrawType_Static)
	{
		GLDrawType = GL_STATIC_DRAW;
	}
	else if (mMeshDrawType == ETGMeshDrawType::TGMeshDrawType_Dynamic)
	{
		GLDrawType = GL_DYNAMIC_DRAW;
	}
	else
	{
		GLDrawType = GL_STREAM_DRAW;
	}
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GLDrawType);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GLDrawType);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// texcoor0
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);
	// texcoor1
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);
	// texcoor2
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(13 * sizeof(float)));
	glEnableVertexAttribArray(5);

	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(15 * sizeof(float)));
	glEnableVertexAttribArray(6);

	// Reset
	glBindVertexArray(0);

	bDirty = false;

	return true;
}

bool TGMeshGeometry::DrawMesh(std::shared_ptr<TGShaderProgram> shader)
{
	if (bDirty) Generate();

	if (bOpenDepthTest)
	{
		glDepthFunc(GL_LESS);
	}
	else
	{
		glDepthFunc(GL_ALWAYS);
	}

	//unsigned int diffuseNr = 1;
	//unsigned int specularNr = 1;
	//for (unsigned int i = 0; i < mTextures.size(); ++i)
	//{
	//	glActiveTexture(GL_TEXTURE0 + i);

	//	if (mTextures[i]->GetTextureType() == ETGTextureType_Texture2D)
	//	{
	//		std::string number = "";
	//		std::string name = mTextures[i]->GetType();

	//		if (name == "texture_diffuse")
	//			number = std::to_string(diffuseNr++);
	//		else if (name == "texture_specular")
	//			number = std::to_string(specularNr++);

	//		shader->SetInt(("material." + name + number).c_str(), i);
	//		glBindTexture(GL_TEXTURE_2D, mTextures[i]->GetID());
	//	}
	//	else
	//	{
	//		shader->SetInt("material.round_diffuse", i);
	//		glBindTexture(GL_TEXTURE_CUBE_MAP, mTextures[i]->GetID());
	//	}
	//}

	//shader->SetVector3("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	//shader->SetVector3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
	//shader->SetVector3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	//shader->SetFloat("material.shininess", 32.0f);

	mMaterial->ApplyToShader(shader);

	// ReBind VAO
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	return true;
}

void TGMeshGeometry::SetMaterial(std::shared_ptr<TGMaterial> material)
{
	mMaterial = material;
}

std::shared_ptr<TGMaterial> TGMeshGeometry::GetMaterial()
{
	return mMaterial;
}

//int TGMeshGeometry::AddTexture(std::string texturePath, std::string type)
//{
//	std::shared_ptr<TGTexture2D> newTexture = std::make_shared<TGTexture2D>(texturePath, type);
//
//	mTextures.push_back(newTexture);
//
//	return mTextures.size() - 1;
//}
//
//int TGMeshGeometry::AddTexture(std::shared_ptr<ITGTexture> texture)
//{
//	mTextures.push_back(texture);
//
//	return mTextures.size() - 1;
//}

void TGMeshGeometry::SetEnableDepthTest(bool useDepthTest)
{
	bOpenDepthTest = useDepthTest;
}

void TGMeshGeometry::SetEnableStencilTest(bool useStencilTest, int stencilValue /*= 0*/)
{
	bOpenStencilTest = useStencilTest;

	if (bOpenStencilTest)
	{
		mStencilValue = stencilValue;
	}
	else
	{
		mStencilValue = false;
	}
}

//std::shared_ptr<ITGTexture> TGMeshGeometry::GetTexture(int index)
//{
//	if (index >= mTextures.size() || index < 0) return nullptr;
//
//	return mTextures[index];
//}

TGSkyBoxGeometry::TGSkyBoxGeometry() : TGMeshGeometry()
{
	
}

bool TGSkyBoxGeometry::DrawMesh(std::shared_ptr<TGShaderProgram> shader)
{
	if (bDirty) Generate();

	glDepthFunc(GL_LEQUAL);

	for (unsigned int i = 0; i < mMaterial->mTextures.size(); ++i)
	{
		if (mMaterial->mTextures[i]->GetTextureType() == ETGTextureType_CubeMap)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, mMaterial->mTextures[i]->GetID());
		}
	}

	// ReBind VAO
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);

	return true;
}

bool TGSkyBoxGeometry::Generate()
{
	std::vector<float> vertices = GetVertexData();

	GLenum GLDrawType;
	if (mMeshDrawType == ETGMeshDrawType::TGMeshDrawType_Static)
	{
		GLDrawType = GL_STATIC_DRAW;
	}
	else if (mMeshDrawType == ETGMeshDrawType::TGMeshDrawType_Dynamic)
	{
		GLDrawType = GL_DYNAMIC_DRAW;
	}
	else
	{
		GLDrawType = GL_STREAM_DRAW;
	}
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GLDrawType);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Reset
	glBindVertexArray(0);

	bDirty = false;

	return true;
}

std::vector<float> TGSkyBoxGeometry::GetVertexData()
{
	std::vector<float> vertexData;

	for (int i = 0; i < mVertices.size(); ++i)
	{
		TGVertex vertex = mVertices[i];
		vertexData.push_back(vertex.mPosition.x);
		vertexData.push_back(vertex.mPosition.y);
		vertexData.push_back(vertex.mPosition.z);
	}

	return vertexData;
}
