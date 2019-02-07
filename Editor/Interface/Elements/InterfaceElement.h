#pragma once
#include <string>

struct ImDrawList;

namespace Sapphire
{
	class InterfaceElement
	{
	public:
		InterfaceElement();
		virtual ~InterfaceElement();

		virtual void Start();
		virtual void Update() {};
		virtual void End();

		std::string name_;
		int positionX_, positionY_;
		int width_, height_;
		int flags_;
		ImDrawList* drawlist_{};

		bool cursorHover{};
	private:
	};
}

