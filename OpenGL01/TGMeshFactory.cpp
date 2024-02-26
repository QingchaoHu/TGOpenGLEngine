#include "TGMeshFactory.h"
#include "TGMeshGeometry.h"

std::shared_ptr<TGMeshFactory> TGMeshFactory::_Instance = nullptr;
std::mutex TGMeshFactory::mMutex;

TGMeshFactory& TGMeshFactory::Get()
{
	if (_Instance == nullptr)
	{
		std::unique_lock<std::mutex> lock(mMutex);
		if (_Instance == nullptr)
		{
			_Instance = std::make_shared<TGMeshFactory>();
		}
	}

	return *_Instance;
}

void TGMeshFactory::Delete()
{
	std::unique_lock<std::mutex> lock(mMutex);
	if (_Instance)
	{
		_Instance.reset();
		_Instance = nullptr;
	}
}

TGMeshFactory::~TGMeshFactory()
{

}

std::shared_ptr<TGMeshGeometry> TGMeshFactory::CreateCube(float size)
{
	if (size <= 0) size = 1.0f;

	float halfSize = size / 2.0f;

	TGVertex v1(glm::vec3(halfSize, halfSize, halfSize), glm::vec2(1.0, 1.0), glm::vec3(1.0, 0.0, 0.0));
	TGVertex v2(glm::vec3(halfSize, -halfSize, halfSize), glm::vec2(1.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	TGVertex v3(glm::vec3(-halfSize, -halfSize, halfSize), glm::vec2(0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
	TGVertex v4(glm::vec3(-halfSize, halfSize, halfSize), glm::vec2(0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v5(glm::vec3(halfSize, halfSize, -halfSize), glm::vec2(1.0, 1.0), glm::vec3(1.0, 1.0, 0.0));
	TGVertex v6(glm::vec3(halfSize, -halfSize, -halfSize), glm::vec2(1.0, 0.0), glm::vec3(1.0, 0.0, 1.0));
	TGVertex v7(glm::vec3(-halfSize, -halfSize, -halfSize), glm::vec2(0.0, 0.0), glm::vec3(0.0, 1.0, 1.0));
	TGVertex v8(glm::vec3(-halfSize, halfSize, -halfSize), glm::vec2(0.0, 1.0), glm::vec3(0.0, 0.0, 0.0));

	std::vector<TGVertex> vertices;
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	vertices.push_back(v4);
	vertices.push_back(v5);
	vertices.push_back(v6);
	vertices.push_back(v7);
	vertices.push_back(v8);

	for (int i = 0; i < vertices.size(); ++i)
	{
		vertices[i].mNormal = glm::normalize(vertices[i].mPosition);
	}

	std::vector<unsigned int> indices = {
		0, 1, 3,
		1, 2, 3,
		0, 4, 1,
		4, 5, 1,
		2, 1, 5,
		5, 6, 2,
		2, 6, 7,
		2, 7, 3,
		0, 3, 7,
		4, 0, 7,
		4, 7, 5,
		5, 7, 6
	};

	std::shared_ptr<TGMeshGeometry> mesh = std::make_shared<TGMeshGeometry>();
	mesh->AddSubMesh("Cube", vertices, indices);
	mesh->Generate();

	return mesh;
}

std::shared_ptr<TGMeshGeometry> TGMeshFactory::CreateCube2(float size)
{
	if (size <= 0) size = 1.0f;

	float halfSize = size / 2.0f;

	// 013
	TGVertex v1(glm::vec3(halfSize, halfSize, halfSize), glm::vec2(1.0, 1.0), glm::vec3(1.0, 1.0, 1.0)); // +++ +-+ -++
	TGVertex v2(glm::vec3(halfSize, -halfSize, halfSize), glm::vec2(1.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v3(glm::vec3(-halfSize, halfSize, halfSize), glm::vec2(0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));

	// 123
	TGVertex v4(glm::vec3(halfSize, -halfSize, halfSize), glm::vec2(1.0, 0.0), glm::vec3(1.0, 1.0, 1.0)); // +-+ --+ -++
	TGVertex v5(glm::vec3(-halfSize, -halfSize, halfSize), glm::vec2(0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v6(glm::vec3(-halfSize, halfSize, halfSize), glm::vec2(0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));

	// 041
	TGVertex v7(glm::vec3(halfSize, halfSize, halfSize), glm::vec2(1.0, 1.0), glm::vec3(1.0, 1.0, 1.0)); // +++ ++- +-+
	TGVertex v8(glm::vec3(halfSize, halfSize, -halfSize), glm::vec2(1.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v9(glm::vec3(halfSize, -halfSize, halfSize), glm::vec2(0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));

	// 451
	TGVertex v10(glm::vec3(halfSize, halfSize, -halfSize), glm::vec2(1.0, 0.0), glm::vec3(1.0, 1.0, 1.0)); // ++- +-- +-+
	TGVertex v11(glm::vec3(halfSize, -halfSize, -halfSize), glm::vec2(0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v12(glm::vec3(halfSize, -halfSize, halfSize), glm::vec2(0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));

	// 215
	TGVertex v13(glm::vec3(-halfSize, -halfSize, halfSize), glm::vec2(0.0, 1.0), glm::vec3(1.0, 1.0, 1.0)); // --+ +-+ +--
	TGVertex v14(glm::vec3(halfSize, -halfSize, halfSize), glm::vec2(1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v15(glm::vec3(halfSize, -halfSize, -halfSize), glm::vec2(1.0, 0.0), glm::vec3(1.0, 1.0, 1.0));

	// 562
	TGVertex v16(glm::vec3(halfSize, -halfSize, -halfSize), glm::vec2(1.0, 0.0), glm::vec3(1.0, 1.0, 1.0)); // +-- --- --+
	TGVertex v17(glm::vec3(-halfSize, -halfSize, -halfSize), glm::vec2(0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v18(glm::vec3(-halfSize, -halfSize, halfSize), glm::vec2(0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));

	// 267
	TGVertex v19(glm::vec3(-halfSize, -halfSize, halfSize), glm::vec2(1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v20(glm::vec3(-halfSize, -halfSize, -halfSize), glm::vec2(1.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v21(glm::vec3(-halfSize, halfSize, -halfSize), glm::vec2(0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));

	// 273
	TGVertex v22(glm::vec3(-halfSize, -halfSize, halfSize), glm::vec2(1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v23(glm::vec3(-halfSize, halfSize, -halfSize), glm::vec2(0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v24(glm::vec3(-halfSize, halfSize, halfSize), glm::vec2(0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));

	// 037
	TGVertex v25(glm::vec3(halfSize, halfSize, halfSize), glm::vec2(0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v26(glm::vec3(-halfSize, halfSize, halfSize), glm::vec2(1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v27(glm::vec3(-halfSize, halfSize, -halfSize), glm::vec2(1.0, 0.0), glm::vec3(1.0, 1.0, 1.0));

	// 407
	TGVertex v28(glm::vec3(halfSize, halfSize, -halfSize), glm::vec2(0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v29(glm::vec3(halfSize, halfSize, halfSize), glm::vec2(0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v30(glm::vec3(-halfSize, halfSize, -halfSize), glm::vec2(1.0, 0.0), glm::vec3(1.0, 1.0, 1.0));

	// 475
	TGVertex v31(glm::vec3(halfSize, halfSize, -halfSize), glm::vec2(0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v32(glm::vec3(-halfSize, halfSize, -halfSize), glm::vec2(1.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v33(glm::vec3(halfSize, -halfSize, -halfSize), glm::vec2(0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));

	// 576
	TGVertex v34(glm::vec3(halfSize, -halfSize, -halfSize), glm::vec2(0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v35(glm::vec3(-halfSize, halfSize, -halfSize), glm::vec2(1.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v36(glm::vec3(-halfSize, -halfSize, -halfSize), glm::vec2(1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));

	std::vector<TGVertex> vertices;
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	vertices.push_back(v4);
	vertices.push_back(v5);
	vertices.push_back(v6);
	vertices.push_back(v7);
	vertices.push_back(v8);
	vertices.push_back(v9);
	vertices.push_back(v10);
	vertices.push_back(v11);
	vertices.push_back(v12);
	vertices.push_back(v13);
	vertices.push_back(v14);
	vertices.push_back(v15);
	vertices.push_back(v16);
	vertices.push_back(v17);
	vertices.push_back(v18);
	vertices.push_back(v19);
	vertices.push_back(v20);
	vertices.push_back(v21);
	vertices.push_back(v22);
	vertices.push_back(v23);
	vertices.push_back(v24);
	vertices.push_back(v25);
	vertices.push_back(v26);
	vertices.push_back(v27);
	vertices.push_back(v28);
	vertices.push_back(v29);
	vertices.push_back(v30);
	vertices.push_back(v31);
	vertices.push_back(v32);
	vertices.push_back(v33);
	vertices.push_back(v34);
	vertices.push_back(v35);
	vertices.push_back(v36);

	for (int i = 0; i < vertices.size(); ++i)
	{
		vertices[i].mNormal = glm::normalize(vertices[i].mPosition);
	}

	std::vector<unsigned int> indices = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35
	};

	std::shared_ptr<TGMeshGeometry> mesh = std::make_shared<TGMeshGeometry>();
	mesh->AddSubMesh("Cube", vertices, indices);
	mesh->Generate();

	return mesh;
}

std::shared_ptr<TGMeshGeometry> TGMeshFactory::CreateGrid(float size)
{
	if (size <= 0) size = 1.0f;

	float halfSize = size / 2.0f;

	// 013
	TGVertex v1(glm::vec3(halfSize, halfSize, 0), glm::vec2(1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v2(glm::vec3(halfSize, -halfSize, 0), glm::vec2(1.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v3(glm::vec3(-halfSize, -halfSize, 0), glm::vec2(0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
	TGVertex v4(glm::vec3(-halfSize, halfSize, 0), glm::vec2(0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));

	std::vector<TGVertex> vertices;
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	vertices.push_back(v4);

	for (int i = 0; i < vertices.size(); ++i)
	{
		vertices[i].mNormal = glm::vec3(0, 0, 1);
	}

	std::vector<unsigned int> indices = {
		0, 1, 3,
		1, 2, 3,
	};

	std::shared_ptr<TGMeshGeometry> mesh = std::make_shared<TGMeshGeometry>();
	mesh->AddSubMesh("Grid", vertices, indices);
	mesh->Generate();

	return mesh;
}
