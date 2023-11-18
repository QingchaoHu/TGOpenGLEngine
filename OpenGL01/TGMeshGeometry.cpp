#include "TGMeshGeometry.h"
#include "GL/glew.h"

TGMeshGeometry::TGMeshGeometry()
{
	mMeshDrawType = ETGMeshDrawType::TGMeshDrawType_Static;

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
	}

	return vertexData;
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

bool TGMeshGeometry::GenerateBuffer()
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

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GLDrawType);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GLDrawType);

	bDirty = false;

	return true;
}

bool TGMeshGeometry::UseBuffer()
{
	if (bDirty) GenerateBuffer();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

	return true;
}

bool TGMeshGeometry::DrawMesh()
{
	UseBuffer();
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, (void*)0);

	return true;
}
