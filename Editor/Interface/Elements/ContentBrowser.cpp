#include "ContentBrowser.h"
#include "Engine/Graphics/Graphics.h"
#include "Engine/Resources/Actor.h"
#include <Editor/Dependencies/imgui-master/imgui.h>
#include "Engine/Utility/Random.h"


#include <filesystem>
#include "Engine/Resources/Player.h"

namespace Sapphire
{
	ContentBrowser contentBrowser_;

	ContentBrowser::ContentBrowser()
	{
		name_ = "Content";
		ScanDirectory("../Assets/Npc", npcAssets_);
		ScanDirectory("../Assets/Player", playerAssets_);
		ScanDirectory("../Assets/Tiles", tileAssets_);
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
		if (ImGui::Button("All")) currentListedObjectType = OT_All;
		ImGui::SameLine(0, 10);
		if (ImGui::Button("NPC")) currentListedObjectType = OT_Actor;
		ImGui::SameLine(0, 10);
		if (ImGui::Button("Player")) currentListedObjectType = OT_Player;
		ImGui::SameLine(0, 10);
		if (ImGui::Button("Tile")) currentListedObjectType = OT_Tile;

		ImGui::Separator();
		switch (currentListedObjectType)
		{
		case OT_All:
			//no break; here to enable all cases to hit for this case
		case OT_Actor:
			for (const auto &asset : npcAssets_)
			{
				if (ImGui::ImageButton((void*)(intptr_t)textureManager_.GetTexture(asset.path_)->textureId_, ImVec2(64, 64)))
				{
					glm::vec3 newPos = glm::vec3(0);
					newPos.x = random_.GenerateFloat(-10.f, 10.f);
					newPos.y = random_.GenerateFloat(5.5f, 9.5f);

					actorManager_.CreateActor(asset.path_, newPos, nullptr);
				}
				ImGui::SameLine(0, 20);
			}
			if (currentListedObjectType != OT_All) break;
		case OT_Player:
			for (const auto &asset : playerAssets_)
			{
				if (ImGui::ImageButton((void*)(intptr_t)textureManager_.GetTexture(asset.path_)->textureId_, ImVec2(64, 64)))
				{
					glm::vec3 newPos = glm::vec3(0);
					newPos.x = random_.GenerateFloat(-10.f, 10.f);
					newPos.y = random_.GenerateFloat(5.5f, 9.5f);

					playerManager_.CreatePlayer(asset.path_, newPos, 0);
				}
				ImGui::SameLine(0, 20);
			}
			if (currentListedObjectType != OT_All) break;
		case OT_Tile:
			for (const auto &asset : tileAssets_)
			{
				if (ImGui::ImageButton((void*)(intptr_t)textureManager_.GetTexture(asset.path_)->textureId_, ImVec2(64, 64)))
				{
					glm::vec3 newPos = glm::vec3(0);
					newPos.x = random_.GenerateFloat(-10.f, 10.f);
					newPos.y = random_.GenerateFloat(5.5f, 9.5f);

					objectManager_.CreateObject(asset.path_, "Quad", newPos, nullptr);
				}
				ImGui::SameLine(0, 20);
			}
			if (currentListedObjectType != OT_All) break;
		default:
			break;
		}
	}

	void ContentBrowser::End()
	{
		InterfaceElement::End();
	}

	ContentAsset::ContentAsset(const std::string &newPath) : path_(newPath)
	{

	}

	void ContentBrowser::ScanDirectory(const std::string &path, std::vector<ContentAsset> &list)
	{
		list.clear();
		for (const auto & entry : std::experimental::filesystem::recursive_directory_iterator(path))
		{
			if (entry.status().type() == std::experimental::filesystem::file_type::directory) continue;
			const ContentAsset newAsset(entry.path().string());
			list.push_back(newAsset);
		}
	}
}
