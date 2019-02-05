#pragma once
#include "InterfaceElement.h"

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
	};

	extern ContentBrowser contentBrowser_;
}

