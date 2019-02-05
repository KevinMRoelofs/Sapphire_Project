#include "ContentBrowser.h"
#include "Engine/Graphics/Graphics.h"
#include "Engine/Resources/Actor.h"
#include <Editor/Dependencies/imgui-master/imgui.h>
#include "Engine/Utility/Random.h"

namespace Sapphire
{
	ContentBrowser contentBrowser_;

	ContentBrowser::ContentBrowser()
	{
		name_ = "Content";
	}

	ContentBrowser::~ContentBrowser()
	{
	}

	void ContentBrowser::Start()
	{
		width_ = graphics_.windowWidth_ * 3 / 5;
		height_ = graphics_.windowHeight_ * 1 / 4;
		positionX_ = graphics_.windowWidth_ * 1 / 5;
		positionY_ = graphics_.windowHeight_ * 3 / 4;
		InterfaceElement::Start();
	}

	void ContentBrowser::Update()
	{
		ImGui::Text("Content");
		//Spawn Nico on Press and Release
		if (ImGui::ImageButton((void*)(intptr_t)textureManager_.GetTexture("../Assets/Player/Nico.png").lock()->textureId_, ImVec2(64, 64)))
		{
			glm::vec3 newPos = glm::vec3(0);
			newPos.x = random_.GenerateFloat(-10.f, 10.f);
			newPos.y = random_.GenerateFloat(5.5f, 9.5f);

			actorManager_.CreateActor("../Assets/Player/Nico.png", newPos, nullptr);
			//cursorInMenu = true;
		}
		ImGui::SameLine(0, 20);
		if (ImGui::ImageButton((void*)(intptr_t)textureManager_.GetTexture("../Assets/Tiles/Ground_Tile.png").lock()->textureId_, ImVec2(64, 64)))
		{
			glm::vec3 newPos = glm::vec3(0);
			newPos.x = random_.GenerateFloat(-10.f, 10.f);
			newPos.y = random_.GenerateFloat(5.5f, 9.5f);

			objectManager_.CreateObject("../Assets/Tiles/Ground_Tile.png","Quad", newPos, nullptr);
			//cursorInMenu = true;
		}
	}

	void ContentBrowser::End()
	{
		InterfaceElement::End();
	}
}
