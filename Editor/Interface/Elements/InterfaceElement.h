#pragma once

struct ImDrawList;

namespace Sapphire
{
	class InterfaceElement
	{
	public:
		InterfaceElement();
		~InterfaceElement();

		virtual void Start();
		virtual void Update() {};
		virtual void End();

		char* name_;
		int positionX_, positionY_;
		int width_, height_;
		int flags_;
		ImDrawList* drawlist_{};

		bool cursorHover{};
	private:
	};
}

