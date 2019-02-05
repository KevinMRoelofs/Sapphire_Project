#pragma once
#include "Elements/InterfaceElement.h"
#include <vector>
#include <memory>

namespace Sapphire
{
	class Interface
	{
	public:
		Interface();
		~Interface();

		void Run();

		std::vector<InterfaceElement*> interfaceElements_;
	};

	extern  Interface interface_;
}

