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
#include "Engine/Resources/Scene.h"


#include <fstream>
#include <filesystem>

using namespace Sapphire;

int main()
{
	engine_.Start(1366, 768, "Sapphire Engine", false);
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
		CreateTemp();

		Edit();
		for (int i = 0; i < 5; i++)
		{
			glm::vec2 newPos = glm::vec2(i * 2 + 0.5f);
		}

		for(int i = 0; i < 1; i++)
		{
		}

		scene_.fullPath = "../Data/index.sapphirescene";

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
		if (inputManager_.AllKeyStates_[Key_Tab].isPressed && editor_.projectState_ != Editor::EditMode) Edit();
		else if (inputManager_.AllKeyStates_[Key_Tab].isPressed && editor_.projectState_ != Editor::PlayMode) Play();

		if (inputManager_.AllKeyStates_[Key_Control].isHeld && inputManager_.AllKeyStates_[Key_N].isPressed && editor_.projectState_ == Editor::EditMode) scene_.New();
		if (inputManager_.AllKeyStates_[Key_Control].isHeld && inputManager_.AllKeyStates_[Key_S].isPressed && editor_.projectState_ == Editor::EditMode) scene_.Save(scene_.fullPath);
		if (inputManager_.AllKeyStates_[Key_Control].isHeld && inputManager_.AllKeyStates_[Key_L].isPressed && editor_.projectState_ == Editor::EditMode) scene_.Load(scene_.fullPath);

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

			interface_.Run();

			int display_w, display_h;
			glfwGetFramebufferSize(graphics_.window_, &display_w, &display_h);

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}

	void Editor::CreateTemp()
	{
		//create temp directory
		std::string tempPath = "../temp";
		CreateDirectory(tempPath.c_str(), nullptr);

		//create temp save file
		std::ofstream myfile;
		myfile.open("../temp/temp.sapphirescene");
		//myfile << "";
		myfile.close();

		//create initial temp save
		scene_.Save("../temp/temp.sapphirescene");
	}

	void Editor::Play()
	{
		editor_.projectState_ = PlayMode;
		graphics_.SetClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		game_.gameState_ = Game::GameState::Playing;
		engine_.timeScale_ = 1.f;

		scene_.Save("../temp/temp.sapphirescene");
	}

	void Editor::Edit()
	{
		editor_.projectState_ = EditMode;
		graphics_.SetClearColor(clearColor_);
		engine_.timeScale_ = 0.f;

		scene_.Load("../temp/temp.sapphirescene");
	}
}
