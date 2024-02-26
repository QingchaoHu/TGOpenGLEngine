#include "TGModel.h"
#include "TGShaderProgram.h"
#include "TGMeshGeometry.h"
#include "assimp/scene.h"
#include <set>

TGModel::TGModel(std::string path)
{
	LoadModel(path);
}

void TGModel::Draw(std::shared_ptr<TGShaderProgram> shader)
{
	for (unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i].DrawMesh(shader);
	}
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

TGMeshGeometry TGModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
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

	TGMeshGeometry Mesh;
	Mesh.AddSubMesh(mesh->mName.C_Str(), vertices, indices);

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<std::string> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		for (std::string key : diffuseMaps)
		{
			Mesh.AddTexture(mLoadedTexture[key]);
		}

		std::vector<std::string> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		for (std::string key : specularMaps)
		{
			Mesh.AddTexture(mLoadedTexture[key]);
		}
	}

	return Mesh;
}

std::vector<std::string> TGModel::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
{
	std::vector<std::string> texturePaths;

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);

		texturePaths.push_back(str.C_Str() + typeName);

		bool skip = false;
		if (mLoadedTexture.count(str.C_Str() + typeName)) skip = true;

		if (!skip)
		{
			std::shared_ptr<TGTexture> newTexture = std::make_shared<TGTexture>(mDirectory + "/" + str.C_Str(), typeName);
			mLoadedTexture.insert({ str.C_Str() + typeName, newTexture });
		}
	}

	return texturePaths;
}