#include "Mesh.h"
#include "Graphics.h"
//#include "Editor/Dependencies/glew-2.1.0/include/GL/gl3w.h"

namespace Sapphire
{
	MeshManager meshManager_;

	Mesh::Mesh(const char* newName, std::vector<Vertex>& newVertices, std::vector<short>& newIndices) : name(newName),vertices_(newVertices), indices_(newIndices)
	{
		CalculateBounds();

		vbo_ = 0;
		vao_ = 0;
		ebo_ = 0;
	}

	Mesh::~Mesh()
	{
	}

	const glm::vec2& Mesh::GetBoundsMin()
	{
		return boundsMin;
	}

	const glm::vec2& Mesh::GetBoundsMax()
	{
		return boundsMax;
	}

	void Mesh::CalculateBounds()
	{
		boundsMin.x = vertices_[0].position_[0];
		boundsMax.x = vertices_[0].position_[0];
		boundsMin.y = vertices_[0].position_[1];
		boundsMax.y = vertices_[0].position_[1];
		for(auto &vert : vertices_)
		{
			if (vert.position_[0] < boundsMin.x) boundsMin.x = vert.position_[0];
			if (vert.position_[0] > boundsMax.x) boundsMax.x = vert.position_[0];
			if (vert.position_[1] < boundsMin.y) boundsMin.y = vert.position_[1];
			if (vert.position_[1] > boundsMax.y) boundsMax.y = vert.position_[1];
		}
	}

	MeshManager::MeshManager()
	{

	}

	MeshManager::~MeshManager()
	{
		for(auto it = allMeshes.begin(); it != allMeshes.end(); ++it)
		{
			graphics_.UnBufferMesh(it->second);
		}
	}

	void MeshManager::Start()
	{
		//curve 2x2
		const std::vector<float> &curveVertices = {
			-1.f,		-1.f,		0.000000f,	1.0f, 1.0f, 1.0f,	0.00f, 1.00f,
			-1.f,		0.f,		0.000000f,	1.0f, 1.0f, 1.0f,	0.00f, 0.00f,
			-0.61f,		-0.962f,	0.000000f,	1.0f, 1.0f, 1.0f,	0.25f, 1.00f,
			-0.805f,	0.019f,		0.000000f,	1.0f, 1.0f, 1.0f,	0.25f, 0.00f,
			-0.235f,	-0.848f,	0.000000f,	1.0f, 1.0f, 1.0f,	0.50f, 1.00f,
			-0.617f,	0.076f,		0.000000f,	1.0f, 1.0f, 1.0f,	0.50f, 0.00f,
			0.111f,		-0.663f,	0.000000f,	1.0f, 1.0f, 1.0f,	0.75f, 1.00f,
			-0.444f,	0.169f,		0.000000f,	1.0f, 1.0f, 1.0f,	0.75f, 0.00f,
			0.414f,		-0.414f,	0.000000f,	1.0f, 1.0f, 1.0f,	1.00f, 1.00f,
			-0.293f,	0.293f,		0.000000f,	1.0f, 1.0f, 1.0f,	1.00f, 0.00f,
			0.663f,		-0.111f,	0.000000f,	1.0f, 1.0f, 1.0f,	1.25f, 1.00f,
			-0.169f,	0.444f,		0.000000f,	1.0f, 1.0f, 1.0f,	1.25f, 0.00f,
			0.848f,		0.235f,		0.000000f,	1.0f, 1.0f, 1.0f,	1.50f, 1.00f,
			-0.076f,	0.617f,		0.000000f,	1.0f, 1.0f, 1.0f,	1.50f, 0.00f,
			0.962f,		0.61f,		0.000000f,	1.0f, 1.0f, 1.0f,	1.75f, 1.00f,
			-0.019f,	0.805f,		0.000000f,	1.0f, 1.0f, 1.0f,	1.75f, 0.00f,
			1.f,		1.f,		0.000000f,	1.0f, 1.0f, 1.0f,	2.00f, 1.00f,
			0.f,		1.f,		0.000000f,	1.0f, 1.0f, 1.0f,	2.00f, 0.00f
		};


		std::vector<short> curveIndices;
		for (int i = 0; i < 8 * 6; i++)
		{
			curveIndices.push_back(0);
			if (i % 6 == 0) curveIndices[i] = short(floor(i / 3));
			if (i % 6 == 1) curveIndices[i] = curveIndices[i - 1] + 1;
			if (i % 6 == 2) curveIndices[i] = curveIndices[i - 1] + 1;
			if (i % 6 == 3) curveIndices[i] = curveIndices[i - 1];
			if (i % 6 == 4) curveIndices[i] = curveIndices[i - 1] - 1;
			if (i % 6 == 5)	curveIndices[i] = curveIndices[i - 1] + 2;
		}
		
		std::vector<Mesh::Vertex> CurveVertices;
		for (int i = 0; i < curveVertices.size(); i++)
		{
			if (i % 8 == 0) CurveVertices.emplace_back();

			if (i % 8 == 0) CurveVertices[int(floor(i / 8))].position_[0] = curveVertices[i];
			if (i % 8 == 1) CurveVertices[int(floor(i / 8))].position_[1] = curveVertices[i];
			if (i % 8 == 2) CurveVertices[int(floor(i / 8))].position_[2] = curveVertices[i];
			if (i % 8 == 3) CurveVertices[int(floor(i / 8))].color_[0] = curveVertices[i];
			if (i % 8 == 4) CurveVertices[int(floor(i / 8))].color_[1] = curveVertices[i];
			if (i % 8 == 5) CurveVertices[int(floor(i / 8))].color_[2] = curveVertices[i];
			if (i % 8 == 6) CurveVertices[int(floor(i / 8))].uv_[0] = curveVertices[i];
			if (i % 8 == 7) CurveVertices[int(floor(i / 8))].uv_[1] = curveVertices[i];
		}

		auto newCurve = std::make_shared<Mesh>(Mesh("Curve2x2", CurveVertices, curveIndices));
		meshManager_.AddMesh(newCurve);

		//Quad
		std::vector<Mesh::Vertex> quadVertices;
		const std::vector<float> &newQuadVertices = {
			//Position         |Color            |Texcoords
			-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Top-left
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // Top-right
			0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
		};
		for (int i = 0; i < newQuadVertices.size(); i++)
		{
			if (i % 8 == 0)	quadVertices.emplace_back();

			if (i % 8 == 0) quadVertices[int(floor(i / 8))].position_[0] = newQuadVertices[i];
			if (i % 8 == 1) quadVertices[int(floor(i / 8))].position_[1] = newQuadVertices[i];
			if (i % 8 == 2)	quadVertices[int(floor(i / 8))].position_[2] = newQuadVertices[i];
			if (i % 8 == 3) quadVertices[int(floor(i / 8))].color_[0] = newQuadVertices[i];
			if (i % 8 == 4) quadVertices[int(floor(i / 8))].color_[1] = newQuadVertices[i];
			if (i % 8 == 5) quadVertices[int(floor(i / 8))].color_[2] = newQuadVertices[i];
			if (i % 8 == 6) quadVertices[int(floor(i / 8))].uv_[0] = newQuadVertices[i];
			if (i % 8 == 7) quadVertices[int(floor(i / 8))].uv_[1] = newQuadVertices[i];
		}
		std::vector<short> quadIndices = {
			0, 1, 2,
			2, 3, 0
		};

		auto newQuad = std::make_shared<Mesh>(Mesh("Quad", quadVertices, quadIndices));
		meshManager_.AddMesh(newQuad);
	}

	void MeshManager::AddMesh(std::shared_ptr<Mesh> &newMesh)
	{
		graphics_.ProcessMesh(newMesh);
		allMeshes.insert(std::pair<const char*, std::shared_ptr<Mesh>>(newMesh->name, newMesh));
	}

	std::shared_ptr<Mesh> &MeshManager::FindMesh(const char* meshName)
	{
		const std::weak_ptr<Mesh> &result = allMeshes.find(meshName)->second;
		if (!result.expired()) return allMeshes.find(meshName)->second;
		return allMeshes[nullptr];
	}
}
