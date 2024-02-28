#pragma once
#include "GL/glew.h"

class TGFrameBuffer
{
public:
	TGFrameBuffer(int scrWidth, int scrHeight);

private:
	unsigned int gPositionTexture;

	unsigned int gNormalTexture;

	unsigned int gAlbedoSpecTexture;

	unsigned int gColorTexture;

	unsigned int gRenderBuffer;

	GLuint gBuffer;

	int mBufferWidth;
	int mBufferHeight;
};