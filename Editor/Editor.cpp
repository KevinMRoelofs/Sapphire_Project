#include "Editor.h"

#include <Editor/Dependencies/glew-2.1.0/include/GL/gl3w.h>
#include "Editor/Dependencies/imgui-master/imgui.h"
#include "Engine/Graphics/Graphics.h"
#include "Editor/Dependencies/imgui-master/opengl3/imgui_impl_glfw_gl3.h"

#include "Engine/Engine.h"
#include "Engine/Utility/Input.h"
#include "Engine/Utility/Random.h"

#include "Game/Game.h"
#include "Engine/Resources/Player.h"
#include "Engine/Resources/Actor.h"
#include "Interface/Elements/Outliner.h"
#include "Interface/Interface.h"

using namespace Sapphire;

int main()
{
	editor_.projectState_ = Editor::ProjectState::EditMode;
	engine_.Start(1600, 900, "Sapphire Engine", false);
	editor_.Start();
	while (graphics_.PrepareRender())
	{
		editor_.EditorLoop();
		graphics_.Render();
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	return 0;
}

namespace Sapphire {

	Editor editor_;

	Editor::Editor()
	{
		projectState_ = EditMode;
		clearColor_ = glm::vec4(0.5f, 0.5f, 0.5f, 0.0f);
	}

	void Editor::Start()
	{
		Edit();
		for (int i = 0; i < 5; i++)
		{
			glm::vec2 newPos = glm::vec2(i * 2 + 0.5f);
			//newPos.x = random_.GenerateFloat(-10.f, 10.f);
			//newPos.y = random_.GenerateFloat(5.5f, 9.5f);
			//actorManager_.CreateActor("../Assets/Textures/Ground_Tile_32.png", newPos, nullptr);
		}

		for(int i = 0; i < 1; i++)
		{
		}

		//objectManager_.CreateObject("../Assets/Textures/Ground_Tile_32.png", "Curve2x2", glm::vec2(0), nullptr);
		//actorManager_.DeleteActor(actorManager_.allActors_[25]);

		playerManager_.CreatePlayer("../Assets/Player/Nico.png", glm::vec2(0.5f));
		gl3wInit();
		// Setup Dear ImGui binding
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplGlfwGL3_Init(graphics_.window_, true);
		ImGui::StyleColorsDark();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	}

	void Editor::EditorLoop()
	{
		if (inputManager_.AllKeyStates_[Key_Tab].isPressed && editor_.projectState_ != Editor::EditMode)
			Edit();
		else if (inputManager_.AllKeyStates_[Key_Tab].isPressed && editor_.projectState_ != Editor::PlayMode)
			Play();

		if (inputManager_.AllKeyStates_[Key_O].isPressed && editor_.projectState_ == Editor::EditMode) SaveScene();
		if (inputManager_.AllKeyStates_[Key_L].isPressed && editor_.projectState_ == Editor::EditMode) LoadScene();

		if (editor_.projectState_ == PlayMode)
		{
			game_.GameLoop();
		}
		else if (editor_.projectState_ == EditMode)
		{
			engine_.MainLoop();
			//if (inputManager_.AllKeyStates_[Key_R].isPressed) engine_.SetTileSize(engine_.GetTileSize() + 1);
			//if (inputManager_.AllKeyStates_[Key_F].isPressed) engine_.SetTileSize(engine_.GetTileSize() - 1);

			glfwPollEvents();
			ImGui_ImplGlfwGL3_NewFrame();

			//outliner_.Selection();
			interface_.Run();


			int display_w, display_h;
			glfwGetFramebufferSize(graphics_.window_, &display_w, &display_h);
			//glViewport(0, 0, display_w, display_h);
			//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			//glClear(GL_COLOR_BUFFER_BIT);
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
			//glfwSwapBuffers(graphics_.window_);
		}
	}

	void Editor::Play()
	{
		editor_.projectState_ = PlayMode;
		graphics_.SetClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		game_.gameState_ = Game::GameState::Playing;
		engine_.timeScale_ = 1.f;
		//save temp of edited level
	}

	void Editor::Edit()
	{
		editor_.projectState_ = EditMode;
		graphics_.SetClearColor(clearColor_);
		//game_.gameState_ = Game::GameState::Paused;
		engine_.timeScale_ = 0.f;
		//reload temp saved edited level here
	}

	void Editor::SaveScene()
	{

	}

	void Editor::LoadScene()
	{

	}
}
