#pragma once
#include "InterfaceElement.h"
#include <Engine/Dependencies/glm/detail/type_vec2.hpp>

namespace Sapphire
{
	class SceneOverlay : public InterfaceElement
	{
	public:
		SceneOverlay();
		~SceneOverlay();

		void Start() override;
		void Update() override;
		void End() override;
		void ShowWorldOrigin() const;
		void ShowGrid(const int& divisions) const;
		void CameraMovement();
	};

	extern SceneOverlay sceneOverlay_;
}
