#pragma once
#include "InterfaceElement.h"
#include <Engine/Dependencies/glm/detail/type_vec2.hpp>

namespace Sapphire
{
	class SceneView : public InterfaceElement
	{
	public:
		SceneView();
		~SceneView();

		void Start() override;
		void Update() override;
		void End() override;
		void ShowWorldOrigin() const;
		void ShowGrid(const int& divisions) const;
		void CameraMovement();
	};

	extern SceneView sceneView_;
}
