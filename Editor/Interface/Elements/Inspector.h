#pragma once
#include "InterfaceElement.h"

namespace Sapphire
{
	class Inspector : public InterfaceElement
	{
	public:
		Inspector();
		~Inspector();
		void Start() override;
		void Update() override;
		void End() override;

	private:
		bool tileMode_;
	};

	extern Inspector inspector_;
}

