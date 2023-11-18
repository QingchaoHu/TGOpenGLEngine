#pragma once
#include <memory>
#include <mutex>
#include "TGMeshGeometry.h"

class TGMeshFactory
{
public:
	static TGMeshFactory& Get();

	static void Delete();

public:
	TGMeshFactory() {}

	~TGMeshFactory();

	// TGMeshFactory(const TGMeshFactory& factory);

	// const TGMeshFactory& operator=(const TGMeshFactory& factory);

	std::shared_ptr<TGMeshGeometry> CreateCube(float size);

private:
	static std::shared_ptr<TGMeshFactory> _Instance;

	static std::mutex mMutex;
};