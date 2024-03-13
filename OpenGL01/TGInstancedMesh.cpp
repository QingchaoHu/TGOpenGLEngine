#include "TGInstancedMesh.h"

TGInstancedModel::TGInstancedModel(std::shared_ptr<TGModel> model)
{
	mModel = model;
}

void TGInstancedModel::Draw(std::shared_ptr<TGShaderProgram> shader)
{
	// 重新生成用于传递每个实例信息的 TransformTexture 和 CustomDataTexture 
	GenerateInstanceRenderTexture();

	TGDrawInstanceProxy instanceProxy;
	instanceProxy.mCustomBufferTex = mCustomBufferTex;
	instanceProxy.mPerInstanceCustomDataNumber = mPerInstancedCustomDataNumber;
	instanceProxy.mTransformBufferTex = mTransformBufferTex;
	instanceProxy.mInstanceNumber = mTransforms.size();

	mModel->DrawInstance(shader, instanceProxy);
}

void TGInstancedModel::AddInstance(glm::mat4 transform)
{
	mTransforms.push_back(transform);

	bIsDirty = true;
}

void TGInstancedModel::RemoveInstanceAt(int index)
{
	if (index < 0 || index >= mTransforms.size()) return;

	mTransforms.erase(mTransforms.begin() + index);

	bIsDirty = true;
}

void TGInstancedModel::AddInstanceArray(std::vector<glm::mat4> transformArray)
{
	for (int i = 0; i < transformArray.size(); i++)
	{
		mTransforms.push_back(transformArray[i]);
	}

	bIsDirty = true;
}

void TGInstancedModel::GenerateInstanceRenderTexture()
{
	if (bIsDirty)
	{
		int transformDataSize = mTransforms.size() * 4;

		int customDataSize = mTransforms.size() * mPerInstancedCustomDataNumber;

		int transformBufSize = 1;
		while (transformBufSize * transformBufSize < transformDataSize)
		{
			transformBufSize *= 2;
		}

		// 四通道RGBA32F，刚好长宽/2放得下
		int customBufSize = 1;
		while (customBufSize * customBufSize < customDataSize)
		{
			customBufSize *= 2;
		}
		customBufSize /= 2;

		float* transformBuf = new float[transformBufSize * transformBufSize * 4];
		float* transformBufPtr = transformBuf;
		for (int i = 0; i < mTransforms.size(); i++)
		{
			for (int m = 0; m < 4; m++)
			{
				for (int n = 0; n < 4; n++)
				{
					*(transformBufPtr++) = mTransforms[i][m][n];
				}
			}
		}

		while (transformBufPtr != (transformBuf + transformBufSize * transformBufSize * 4))
		{
			*(transformBufPtr++) = 0.0f;
		}

		float* customBuf = new float[customBufSize * customBufSize * 4];
		std::memcpy(customBuf, mCustomDataBuffer, sizeof(float) * mTransforms.size() * mPerInstancedCustomDataNumber);

		mTransformBufferTex = std::make_shared<TGTexture2D>(transformBufSize, transformBufSize, ETGDataFormat::ETGDataFormat_RGBA32F);
		if (mTransformBufferTex)
		{
			mTransformBufferTex->SetTextureDataF(transformBuf);
			delete[] transformBuf;
		}

		mCustomBufferTex = std::make_shared<TGTexture2D>(customBufSize, customBufSize, ETGDataFormat::ETGDataFormat_RGBA32F);
		if (mCustomBufferTex)
		{
			mCustomBufferTex->SetTextureDataF(customBuf);
			delete[] customBuf;
		}
	}

	bIsDirty = false;
}


