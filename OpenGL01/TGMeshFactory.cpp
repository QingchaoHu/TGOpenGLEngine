#include "TGMeshFactory.h"

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
	v1.mNormal = glm::normalize(v1.mPosition);
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
	mesh->GenerateBuffer();

	return mesh;
}
