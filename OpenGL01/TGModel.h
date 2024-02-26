#pragma once
#include <string>
#include <vector>
#include "assimp/Importer.hpp"
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include <memory>
#include <unordered_map>

class TGShaderProgram;
class TGMeshGeometry;
class TGTexture;

class TGModel
{
public:
	TGModel(std::string path);

	void Draw(std::shared_ptr<TGShaderProgram> shader);

private:
	std::vector<TGMeshGeometry> mMeshes;

	std::string mDirectory;

	void LoadModel(std::string path);

	void ProcessNode(aiNode* node, const aiScene* scene);
	TGMeshGeometry ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<std::string> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);

	std::unordered_map<std::string, std::shared_ptr<TGTexture>> mLoadedTexture;
};