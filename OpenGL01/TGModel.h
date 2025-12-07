#pragma once
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <memory>
#include <unordered_map>
#include "TGTexture.h"

class TGShaderProgram;
class TGMeshGeometry;
class TGTexture2D;
class TGDrawInstanceProxy;

class TGModel
{
public:
	TGModel(std::string path);

	void Draw(std::shared_ptr<TGShaderProgram> shader);

	void DrawInstance(std::shared_ptr<TGShaderProgram> shader, TGDrawInstanceProxy instanceProxy);

	void AddMeshObject(std::shared_ptr<TGMeshGeometry> meshComponent);

	TGModel();

private:
	std::vector<std::shared_ptr<TGMeshGeometry>> mMeshes;

	std::string mDirectory;

	void LoadModel(std::string path);

	void ProcessNode(aiNode* node, const aiScene* scene);

	std::shared_ptr<TGMeshGeometry> ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<std::string> LoadMaterialTextures(aiMaterial* material, aiTextureType type, ETGTextureUseType innerType);

	std::unordered_map<std::string, std::shared_ptr<TGTexture2D>> mLoadedTexture;
};