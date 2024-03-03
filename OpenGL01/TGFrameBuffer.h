#pragma once
#include "GL/glew.h"

class TGFrameBuffer
{
public:
	TGFrameBuffer(int scrWidth, int scrHeight);

	void Use();

	void EndUse();

	unsigned int GetGPositionTexture();

	unsigned int GetGNormalTexture();

	unsigned int GetGAlbedoSpecTexture();

	unsigned int GetGRenderBuffer();

private:
	unsigned int gPositionTexture;

	unsigned int gNormalTexture;

	unsigned int gAlbedoSpecTexture;

	unsigned int gMaterialParamTexture;

	unsigned int gDepthStencilTexture;

	unsigned int gStencilView;

	unsigned int gRenderBuffer;

	unsigned int gDepthStencilRenderBuffer;

	GLuint gBuffer;

	int mBufferWidth;

	int mBufferHeight;

	bool bIsFrameBufferComplete = false;
};