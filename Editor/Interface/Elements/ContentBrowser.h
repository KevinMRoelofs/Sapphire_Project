#pragma once
#include "InterfaceElement.h"
#include <string>
#include <vector>
#include "Engine/Graphics/Texture.h"

namespace Sapphire
{

	class ContentBrowser : public InterfaceElement
	{
	public:
		ContentBrowser();
		~ContentBrowser();
		void Start() override;
		void Update() override;
		void End() override;

		struct ContentAsset
		{
			ContentAsset(const std::string &newPath);
			const std::string path_;
		};

		void ScanDirectory(); 
		std::vector<ContentAsset> contentAssets_;
	};

	extern ContentBrowser contentBrowser_;
}

