#pragma once
#include "InterfaceElement.h"
#include <string>
#include <vector>
#include "Engine/Graphics/Texture.h"

namespace Sapphire
{
	struct ContentAsset
	{
		ContentAsset(const std::string &newPath);
		const std::string path_;
	};

	class ContentBrowser : public InterfaceElement
	{
	public:
		ContentBrowser();
		~ContentBrowser();
		void Start() override;
		void Update() override;
		void End() override;


		void ScanDirectory(const std::string &path, std::vector<ContentAsset> &list);

		std::vector<ContentAsset> playerAssets_;
		std::vector<ContentAsset> npcAssets_;
		std::vector<ContentAsset> tileAssets_;

		enum ObjectType
		{
			OT_All,
			OT_Actor,
			OT_Player,
			OT_Tile
		} currentListedObjectType;
	};

	extern ContentBrowser contentBrowser_;
}

