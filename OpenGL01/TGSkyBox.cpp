#include "TGSkyBox.h"
#include "ThirdParty/glm/glm.hpp"
#include "../../glm/ext/matrix_transform.inl"
#include "../../glm/gtc/type_ptr.inl"

TGSkyBox::TGSkyBox(int size)
{
	mSkyBoxVS = std::make_shared<TGVertexShader>("Shaders/SkyBox.vert", "SkyVS");
	mSkyBoxPS = std::make_shared<TGFragmentShader>("Shaders/SkyBox.frag", "SkyPS");
	mSkyBoxShaderProgram = std::make_shared<TGShaderProgram>();
	mSkyBoxShaderProgram->AddVertexShader(mSkyBoxVS.get());
	mSkyBoxShaderProgram->AddFragmentShader(mSkyBoxPS.get());

	std::vector<std::string> skyboxTexturePath
	{
		"Textures/skybox/right_cc.jpg",
		"Textures/skybox/left_c.jpg",
		"Textures/skybox/back_z.jpg",
		"Textures/skybox/front.jpg",
		"Textures/skybox/top.jpg",
		"Textures/skybox/bottom_r.jpg",
	};

	mSkyBoxTexture = std::make_shared<TGCubeTexture>(skyboxTexturePath, ETGTextureUseType_Diffuse);

	mSkyBoxGeometry = TGMeshFactory::Get().CreateSkybox(2.0);
	std::shared_ptr<TGMaterial> skyboxMaterial = std::make_shared<TGMaterial>();
	skyboxMaterial->AddTexture(mSkyBoxTexture);
	mSkyBoxGeometry->SetMaterial(skyboxMaterial);

	mScale = size;
}

void TGSkyBox::Draw(TGLocalPlayer* player)
{
	mSkyBoxShaderProgram->UseProgram();
	glm::mat4 viewMatrix = player->GetPlayerCamera()->GetCameraViewMatrix();
	glm::mat4 projectionMatrix = player->GetPlayerCamera()->GetCameraProjectionMatrix();
	glm::mat4 modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(mScale, mScale, mScale));
	mSkyBoxShaderProgram->SetMatrix4x4("view", glm::value_ptr(viewMatrix));
	mSkyBoxShaderProgram->SetMatrix4x4("model", glm::value_ptr(modelMatrix));
	mSkyBoxShaderProgram->SetMatrix4x4("projection", glm::value_ptr(projectionMatrix));
	mSkyBoxGeometry->DrawMesh(mSkyBoxShaderProgram);
}

std::shared_ptr<ITGTexture> TGSkyBox::GetSkyboxTexture()
{
	return mSkyBoxTexture;
}
