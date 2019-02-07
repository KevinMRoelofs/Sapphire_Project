#pragma once
//#define GLFW_INCLUDE_NONE
#include <Engine/Dependencies/glad/include/glad/glad.h>
#include <Engine/Dependencies/glfw-3.2.1/include/GLFW/glfw3.h>

#include <string>
#include "Mesh.h"
#include "Engine/Resources/Object.h"

//#define PERS
#define ORTH

namespace Sapphire
{
	class Graphics
	{
	public:
		Graphics();
		~Graphics();
		void SetUpWindow(int const& width, int const& height, const std::string& name, const bool& fullScreen);

		bool PrepareRender();
		void Render() const;

		void ProcessTexture(const unsigned char* path, const std::shared_ptr<Texture>& passedTexture);

		/// <summary>
		/// The executable Window
		/// </summary>
		GLFWwindow* window_;

		const glm::vec4& GetClearColor() const;
		void SetClearColor(const glm::vec4& newClearColor);
		void SetClearColor(const float& newX, const float& newY, const float newZ, const float newW);

		int windowWidth_, windowHeight_;

		void ProcessMesh(std::shared_ptr<Mesh> &newMesh);
		void RenderObject(const std::shared_ptr<Object> &object) const;

		void UnBufferMesh(std::shared_ptr<Mesh> &newMesh);
		struct Camera
		{
			Camera();
			void SetPositition(const glm::vec2 &newPos);
			void SetPositition(const float &newX, const float &newY);
			const glm::vec2 &GetPositionWorld() const;
			void OffsetPosition(const glm::vec2 &addedPos);
			void OffsetPosition(const float& addedX, const float &addedY);
		private:
			glm::vec2 position_;
			float rotation_;
		}camera_;

	private:
		void SetUpShaders();

		int shaderProgram_;

		//unsigned int VBO, VAO, EBO;

		//float greenValue;

		glm::vec4 clearColor_;

		glm::mat4x4 view_;
		glm::mat4x4 projection_;

		const char *vertexShaderSource_ = "#version 330 core\n"
			"layout (location = 0) in vec3 inVertexPos;\n"
			"layout (location = 1) in vec3 inVertexColor;\n"
			"layout (location = 2) in vec2 inTexCoord;\n"
			"out vec3 outColor;\n"
			"out vec2 outTexCoord;\n"
			"uniform mat4 uniModel;\n"
			"uniform mat4 uniView;\n"
			"uniform mat4 uniProj;\n"
			"void main()\n"
			"{\n"
			"   //gl_Position = vec4(inVertexPos.x, inVertexPos.y, inVertexPos.z, 1.0);\n"
			"   gl_Position = uniProj * uniView * uniModel * vec4(inVertexPos, 1.0f);\n"

			"	outColor = inVertexColor;\n"
			"	outTexCoord = inTexCoord;\n"
			"}\0";

		const char *fragmentShaderSource_ = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"in vec3 outColor;\n"
			"in vec2 outTexCoord;\n"
			"uniform vec3 uniCustomColor;\n"
			"uniform sampler2D uniTexture;\n"
			"void main()\n"
			"{\n"
			"   FragColor = texture(uniTexture, outTexCoord) * vec4(outColor * uniCustomColor, 1.0);\n"
			"}\n\0";
	};

	extern Graphics graphics_;

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
}
