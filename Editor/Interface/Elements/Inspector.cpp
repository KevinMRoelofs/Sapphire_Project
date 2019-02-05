#include "Inspector.h"
#include "Engine/Graphics/Graphics.h"
#include <Editor/Dependencies/imgui-master/imgui.h>
#include "Hierarchy.h"
#include "Engine/Resources/Player.h"

namespace Sapphire
{
	Inspector inspector_;

	Inspector::Inspector()
	{
		name_ = "Inspector";
		positionY_ = 0;
		tileMode_ = true;
	}

	Inspector::~Inspector()
	{
	}

	void Inspector::Start()
	{
		width_ = graphics_.windowWidth_ * 1 / 5;
		height_ = graphics_.windowHeight_;
		positionX_ = graphics_.windowWidth_ * 4 / 5;
		InterfaceElement::Start();
	}

	void Inspector::Update()
	{
		for (auto object : hierarchy_.selectedObjects_)
		{
			std::weak_ptr<Actor> actorPtr = std::dynamic_pointer_cast<Actor>(object.lock());
			std::weak_ptr<Player> playerPtr = std::dynamic_pointer_cast<Player>(actorPtr.lock());
			ImGui::PushID(object.lock()->GetId());
			ImGui::Image((void*)(intptr_t)object.lock()->GetTexture()->textureId_, ImVec2(32, 32));
			ImGui::Text("%i", object.lock()->GetId());
			if (!playerPtr.expired())
			{
				ImGui::SameLine();
				ImGui::Text("[Player %i]", playerPtr.lock()->playerId_);
			}
			else if (!actorPtr.expired())
			{
				ImGui::SameLine();
				ImGui::Text("[Actor]");
			}
			glm::vec2 objectPos = object.lock()->GetPositionWorld();
			ImGui::InputFloat("X", reinterpret_cast<float*>(&objectPos.x), 0.125f, 0.25f, "%.3f");
			ImGui::InputFloat("Y", reinterpret_cast<float*>(&objectPos.y), 0.125f, 0.25f, "%.3f");

			if (ImGui::Button("Center"))
			{
				objectPos = ceil(objectPos);
				objectPos -= 0.5f;
			}

			object.lock()->SetPositionWorld(objectPos);
			ImGui::PopID();
			ImGui::Separator();
		}
	}

	void Inspector::End()
	{

		InterfaceElement::End();
	}
}
