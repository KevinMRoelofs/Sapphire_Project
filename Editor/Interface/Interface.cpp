#include "Interface.h"

#include "Elements/ContentBrowser.h"
#include "Elements/Inspector.h"
#include "Elements/SceneView.h"

namespace Sapphire
{
	Interface interface_;

	Interface::Interface()
	{
	}


	Interface::~Interface()
	{
	}

	void Interface::Run()
	{
		for(auto &interfaceElement : interfaceElements_)
		{
			interfaceElement->Start();
			interfaceElement->Update();
			interfaceElement->End();
		}
		//ImGui::ShowDemoWindow(static_cast<bool*>(nullptr));
	}
}
