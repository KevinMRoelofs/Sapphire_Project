#pragma once
#include <vector>
#include <map>
//#include <Engine/Dependencies/glad/include/glad/glad.h>
//#include <Engine/Dependencies/glfw-3.2.1/include/GLFW/glfw3.h>
#include <future>
#include <Engine/Dependencies/glm/detail/type_vec2.hpp>


namespace Sapphire
{

	struct Mesh
	{
		//new
		const char* name;
		struct Vertex
		{
			float position_[3];
			float color_[3];
			float uv_[2];
		};
		const std::vector<Vertex> vertices_;
		const std::vector<short> indices_;

		Mesh(const char* newName, std::vector<Vertex>& newVertices, std::vector<short>& newIndices);
		Mesh() = delete;
		~Mesh();
		
		const glm::vec2 &GetBoundsMin();
		const glm::vec2 &GetBoundsMax();

		unsigned int vbo_, vao_, ebo_;
	private:
		void CalculateBounds();

		glm::vec2 boundsMin, boundsMax;

	};

	//struct Quad
	//{
	//	float vertices[32] = {
	//		//Position         |Color            |Texcoords
	//		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Top-left
	//		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // Top-right
	//		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Bottom-right
	//		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // Bottom-left
	//	};
	//	unsigned int indices[6] = {
	//		0, 1, 2,
	//		2, 3, 0
	//	};
	//};

	class MeshManager
	{
		std::map<const std::string, std::shared_ptr<Mesh>> allMeshes;

	public:
		MeshManager();
		~MeshManager();

		void Start();
		void AddMesh(std::shared_ptr<Mesh> &newMesh);
		std::shared_ptr<Mesh> &FindMesh(const char* meshName);
	};
	extern MeshManager meshManager_;
}
