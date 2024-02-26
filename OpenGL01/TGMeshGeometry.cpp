#include "TGMeshGeometry.h"
#include "TGTexture.h"
#include "TGShaderProgram.h"

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// texcoor0
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);
	// texcoor1
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);
	// texcoor2
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(13 * sizeof(float)));
	glEnableVertexAttribArray(5);

	// Reset
	glBindVertexArray(0);

	bDirty = false;

	return true;
}

bool TGMeshGeometry::DrawMesh(std::shared_ptr<TGShaderProgram> shader)
{
	if (bDirty) Generate();

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < mTextures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number = "";
		std::string name = mTextures[i]->GetType();

		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader->SetInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, mTextures[i]->GetID());
	}

	shader->SetVector3("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	shader->SetVector3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
	shader->SetVector3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	shader->SetFloat("material.shininess", 32.0f);

	// ReBind VAO
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	return true;
}

bool TGMeshGeometry::AddTexture(std::string texturePath, std::string type)
{
	std::shared_ptr<TGTexture> newTexture = std::make_shared<TGTexture>(texturePath, type);

	mTextures.push_back(newTexture);

	return true;
}

bool TGMeshGeometry::AddTexture(std::shared_ptr<TGTexture> texture)
{
	mTextures.push_back(texture);

	return true;
}
