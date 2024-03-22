#pragma once
#include <memory>
#include "TGShaderProgram.h"
#include "TGInstancedMesh.h"

enum ETGMeshType
{
	ETGMeshType_StaticMesh,
	ETGMeshType_InstanceStaticMesh
};

class ITGMeshProxy
{
public:
	virtual void GenerateMeshProxy() = 0;

	virtual ETGMeshType GetMeshType() = 0;

	virtual void Draw(std::shared_ptr<TGShaderProgram> shader) = 0;

	virtual void DrawInstance(std::shared_ptr<TGShaderProgram> shader, TGDrawInstanceProxy instanceProxy) = 0;
};