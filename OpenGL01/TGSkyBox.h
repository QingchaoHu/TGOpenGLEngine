#pragma once
#include <memory>
#include "TGMeshGeometry.h"
#include "TGShader.h"
#include "TGShaderProgram.h"
#include "TGMeshFactory.h"
#include "TGLocalPlayer.h"

class TGSkyBox
{
public:
	TGSkyBox(int size = 100.0);

	void Draw(TGLocalPlayer* player);

	std::shared_ptr<ITGTexture> GetSkyboxTexture();

private:
	std::shared_ptr<TGSkyBoxGeometry> mSkyBoxGeometry;

	std::shared_ptr<TGShaderProgram> mSkyBoxShaderProgram;

	std::shared_ptr<TGVertexShader> mSkyBoxVS;

	std::shared_ptr<TGFragmentShader> mSkyBoxPS;

	std::shared_ptr<TGCubeTexture> mSkyBoxTexture;

	int mScale = 100.0;
};