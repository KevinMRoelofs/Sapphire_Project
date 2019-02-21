
#include "Engine/Graphics/Graphics.h"
#include "Engine/Engine.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "Engine/Dependencies/stb_image/stb_image.h"
#include <Engine/Dependencies/glm/gtc/type_ptr.hpp>
#include <Engine/Dependencies/glm/gtc/matrix_transform.inl>
#include "Engine/Utility/Input.h"

//#define VISUALGREEN

namespace Sapphire {
	Graphics graphics_;

	Graphics::Graphics() : window_(nullptr), windowWidth_(0), windowHeight_(0), shaderProgram_(0)
	{
		view_ = glm::mat4x4(1);
		projection_ = glm::mat4x4(1);
		//clearColor_ = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
	}

	Graphics::~Graphics()
	{


		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwDestroyWindow(window_);
		glfwTerminate();

	}

	void Graphics::SetUpWindow(int const& width, int const& height, const std::string& name, const bool& fullScreen)
	{

		// glfw: initialize and configure
		// ------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

		// glfw window creation
		// --------------------
		GLFWmonitor* monitor = fullScreen ? glfwGetPrimaryMonitor() : NULL;
		window_ = glfwCreateWindow(width, height, name.c_str(), monitor, NULL);
		if (window_ == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			//return -1;
		}
		glfwMakeContextCurrent(window_);
		glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			//return -1;
		}

		glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		SetUpShaders();
	}

	void Graphics::SetUpShaders()
	{

		// build and compile our shader program
		// ------------------------------------
		// vertex shader
		const int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource_, nullptr);
		glCompileShader(vertexShader);
		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// fragment shader


		const int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource_, nullptr);
		glCompileShader(fragmentShader);
		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// link shaders
		shaderProgram_ = glCreateProgram();
		glAttachShader(shaderProgram_, vertexShader);
		glAttachShader(shaderProgram_, fragmentShader);
		glLinkProgram(shaderProgram_);
		// check for linking errors
		glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram_, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	bool Graphics::PrepareRender()
	{
		// render
		// ------

		glfwGetWindowSize(window_, &windowWidth_, &windowHeight_);

		//view_ = glm::translate(view_, glm::vec3(camera_.GetPositionWorld().x, camera_.GetPositionWorld().y, -3.0f));

		float aspect = float(windowWidth_) / float(windowHeight_);
#ifdef PERS
		projection_ = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
#endif
#ifdef ORTH
		//projection_ = glm::ortho(-500.f, 500.f, -350.f, 350.f, 0.1f, 100.0f);
		projection_ = glm::ortho(-float(windowWidth_) / 2, float(windowWidth_) / 2, -float(windowHeight_) / 2, float(windowHeight_) / 2, 0.1f, 100.0f);
#endif

		view_ = glm::mat4x4(1);
		view_ = glm::translate(view_, glm::vec3(-camera_.GetPositionWorld().x, -camera_.GetPositionWorld().y, -3.0f));

		glClearColor(clearColor_.x, clearColor_.y, clearColor_.z, clearColor_.w);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram_);

		const int vertexColorLocation = glGetUniformLocation(shaderProgram_, "uniCustomColor");
		glUniform3f(vertexColorLocation, 1.0f, 1.0f, 1.0f);

#ifdef VISUALGREEN
		greenValue = (sin(float(glfwGetTime())) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "uniCustomColor");
		glUniform3f(vertexColorLocation, 0.0f, greenValue, 0.0f);
#endif

		const unsigned int viewLoc = glGetUniformLocation(shaderProgram_, "uniView");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_));

		const unsigned int projLoc = glGetUniformLocation(shaderProgram_, "uniProj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection_));

		for (const auto& object : objectManager_->GetAllObjects())
		{
			object.lock()->UpdatePositionScreen();
			RenderObject(object.lock());
		}
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwPollEvents();

		// Check if the ESC key was pressed or the window was closed
		return glfwGetKey(window_, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window_) == 0;
	}

	void Graphics::Render() const
	{
		glfwSwapBuffers(window_);
	}


	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}

	void Graphics::ProcessTexture(const unsigned char* stbiData, const std::shared_ptr<Texture>& passedTexture)
	{
		assert(passedTexture);
		glGenTextures(1, &passedTexture->textureId_);
		glBindTexture(GL_TEXTURE_2D, passedTexture->textureId_);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, passedTexture->width_, passedTexture->height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, stbiData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	const glm::vec4& Graphics::GetClearColor() const
	{
		return clearColor_;
	}

	void Graphics::SetClearColor(const glm::vec4& newClearColor)
	{
		clearColor_ = newClearColor;
	}

	void Graphics::SetClearColor(const float& newX, const float& newY, const float newZ, const float newW)
	{
		clearColor_ = glm::vec4(newX, newY, newZ, newW);
	}

	void Graphics::ProcessMesh(std::shared_ptr<Mesh>& newMesh)
	{
		glGenVertexArrays(1, &newMesh->vao_);
		glGenBuffers(1, &newMesh->vbo_);
		glGenBuffers(1, &newMesh->ebo_);

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(newMesh->vao_);

		glBindBuffer(GL_ARRAY_BUFFER, newMesh->vbo_);
		glGetError();
		glBufferData(GL_ARRAY_BUFFER, sizeof newMesh->vertices_[0] * newMesh->vertices_.size(), &newMesh->vertices_[0], GL_STATIC_DRAW);
		glGetError();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh->ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof newMesh->indices_[0] * newMesh->indices_.size(), &newMesh->indices_[0], GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(nullptr));
		glEnableVertexAttribArray(0);

		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Texture attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	void Graphics::RenderObject(const std::shared_ptr<Object>& object) const
	{
		const unsigned int transformLoc = glGetUniformLocation(shaderProgram_, "uniModel");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(object->model_));

		glBindVertexArray(object->GetMesh()->vao_);
		glBindBuffer(GL_ARRAY_BUFFER, object->GetMesh()->vbo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->GetMesh()->ebo_);

		glBindTexture(GL_TEXTURE_2D, object->GetTexture()->textureId_);
		const GLsizei& indicesAmount = GLsizei(object->GetMesh()->indices_.size());
		glDrawElements(GL_TRIANGLES, indicesAmount, GL_UNSIGNED_SHORT, nullptr);
	}

	void Graphics::UnBufferMesh(std::shared_ptr<Mesh>& newMesh)
	{
		glDeleteVertexArrays(1, &newMesh->vao_);
		glDeleteBuffers(1, &newMesh->vbo_);
		glDeleteBuffers(1, &newMesh->ebo_);
		glBindVertexArray(0);
	}

	Graphics::Camera::Camera() : position_(glm::vec2(0, 0)), rotation_(0.f)
	{
	}

	void Graphics::Camera::SetPositition(const glm::vec2 &newPos)
	{
		position_ = newPos;
	}

	void Graphics::Camera::SetPositition(const float& newX, const float& newY)
	{
		position_ = glm::vec2(newX, newY);
	}

	const glm::vec2& Graphics::Camera::GetPositionWorld() const
	{
		return position_;
	}

	void Graphics::Camera::OffsetPosition(const glm::vec2& addedPos)
	{
		position_ += addedPos;
	}

	void Graphics::Camera::OffsetPosition(const float& addedX, const float& addedY)
	{
		position_ += glm::vec2(addedX, addedY);
	}
}