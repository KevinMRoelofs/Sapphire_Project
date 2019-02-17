#pragma once
#include <Engine/Dependencies/glm/glm.hpp>

namespace Sapphire
{
	class Editor
	{
	public:
		Editor();
		void Start();

		void EditorLoop();
		void EditorCameraMovement();

		enum ProjectState
		{
			PlayMode,
			EditMode
		}projectState_;

		void CreateTemp();

		void Play();
		void Edit();

		glm::vec4 clearColor_;
	};
	extern Editor editor_;
}

