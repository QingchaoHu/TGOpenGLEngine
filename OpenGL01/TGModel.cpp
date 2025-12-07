#include "TGModel.h"
#include "TGShaderProgram.h"
#include "TGMeshGeometry.h"
#include <assimp/scene.h>
#include <set>
#include "TGInstancedMesh.h"

TGModel::TGModel(std::string path)
{
	LoadModel(path);
}

TGModel::TGModel()
{

}

void TGModel::Draw(std::shared_ptr<TGShaderProgram> shader)
{
	for (unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i]->DrawMesh(shader);
	}
}

void TGModel::DrawInstance(std::shared_ptr<TGShaderProgram> shader, TGDrawInstanceProxy instanceProxy)
{
	for (unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i]->DrawInstanceMesh(shader, instanceProxy);
	}
}

void TGModel::AddMeshObject(std::shared_ptr<TGMeshGeometry> meshComponent)
{
	mMeshes.push_back(meshComponent);
}

void TGModel::LoadModel(std::string path)
{
	Assimp::Importer importer;

	// aiProcess_GenNormals | aiProcess_SplitLargeMeshes | aiProcess_OptimizeMeshes
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	mDirectory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void TGModel::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mMeshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

std::shared_ptr<TGMeshGeometry> TGModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<TGVertex> vertices;
	std::vector<unsigned int> indices;
	std::set<std::string> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 position(mesh->mVertices[i].x, mesh->mVertices[i].y, -mesh->mVertices[i].z);
		glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		glm::vec2 textureCoordinate(0, 0);
		if (mesh->mTextureCoords[0])
		{
			textureCoordinate.x = mesh->mTextureCoords[0][i].x;
			textureCoordinate.y = mesh->mTextureCoords[0][i].y;
		}
		TGVertex vertex(position, textureCoordinate, glm::vec3(1.0, 1.0, 1.0));
		vertex.mNormal = normal;
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	std::shared_ptr<TGMeshGeometry> Mesh = std::make_shared<TGMeshGeometry>();
	Mesh->AddSubMesh(mesh->mName.C_Str(), vertices, indices);

	if (mesh->mMaterialIndex >= 0)
	{
		std::shared_ptr<TGMaterial> innerMaterial = std::make_shared<TGMaterial>();

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<std::string> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, ETGTextureUseType_Diffuse);
		for (std::string key : diffuseMaps)
		{
			innerMaterial->AddTexture(mLoadedTexture[key]);
		}

		std::vector<std::string> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, ETGTextureUseType_Specular);
		for (std::string key : specularMaps)
		{
			innerMaterial->AddTexture(mLoadedTexture[key]);
		}

		Mesh->SetMaterial(innerMaterial);
	}

	return Mesh;
}

std::vector<std::string> TGModel::LoadMaterialTextures(aiMaterial* material, aiTextureType type, ETGTextureUseType innerType)
{
	std::vector<std::string> texturePaths;

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);

		std::string typeV = "_" + std::to_string((int)innerType);
		texturePaths.push_back(str.C_Str() + typeV);

		bool skip = false;
		if (mLoadedTexture.count(str.C_Str() + typeV)) skip = true;

		if (!skip)
		{
			
			std::shared_ptr<TGTexture2D> newTexture = std::make_shared<TGTexture2D>(mDirectory + "/" + str.C_Str(), innerType);
			mLoadedTexture.insert({ str.C_Str() + typeV, newTexture });
		}
	}

	return texturePaths;
}