#include "ContentBrowser.h"
#include "Engine/Graphics/Graphics.h"
#include "Engine/Resources/Actor.h"
#include <Editor/Dependencies/imgui-master/imgui.h>
#include "Engine/Utility/Random.h"


#include <filesystem>

namespace Sapphire
{
	ContentBrowser contentBrowser_;

	ContentBrowser::ContentBrowser()
	{
		name_ = "Content";
		ScanDirectory();
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
		//ImGui::Text("Content");
		//Spawn Nico on Press and Release
		//if (ImGui::ImageButton((void*)(intptr_t)textureManager_.GetTexture("../Assets/Player/Nico.png")->textureId_, ImVec2(64, 64)))
		//{
		//	glm::vec3 newPos = glm::vec3(0);
		//	newPos.x = random_.GenerateFloat(-10.f, 10.f);
		//	newPos.y = random_.GenerateFloat(5.5f, 9.5f);

		//	actorManager_.CreateActor("../Assets/Player/Nico.png", newPos, nullptr);
		//	//cursorInMenu = true;
		//}

		//list all Assets in folders
		for (const auto &asset : contentAssets_) {
			//const auto texture = textureManager_.GetTexture(charPath);
			if (ImGui::ImageButton((void*)(intptr_t)textureManager_.GetTexture(asset.path_)->textureId_, ImVec2(64, 64)))
			{
				glm::vec3 newPos = glm::vec3(0);
				newPos.x = random_.GenerateFloat(-10.f, 10.f);
				newPos.y = random_.GenerateFloat(5.5f, 9.5f);

				objectManager_.CreateObject(asset.path_, "Quad", newPos, nullptr);
				//cursorInMenu = true;
			}
			ImGui::SameLine(0, 20);
		}
		int i = 0;
	}

	void ContentBrowser::End()
	{
		InterfaceElement::End();
	}

	ContentBrowser::ContentAsset::ContentAsset(const std::string &newPath) : path_(newPath)
	{

	}

	void ContentBrowser::ScanDirectory()
	{
		const std::string path = "../Assets";
		for (const auto & entry : std::experimental::filesystem::recursive_directory_iterator(path))
		{
			if (entry.status().type() == std::experimental::filesystem::file_type::directory) continue;
			const ContentAsset newAsset(entry.path().string());
			contentAssets_.push_back(newAsset);
			printf("%s \n", entry.path().string().c_str());
		}
			int i = 0;
	}
}
