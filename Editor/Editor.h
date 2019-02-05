#pragma once
#include <Engine/Dependencies/glm/glm.hpp>
//#include <Engine/Dependencies/cereal-master/include/cereal/cereal.hpp>

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

		void Play();
		void Edit();

		void SaveScene();
		void LoadScene();

		glm::vec4 clearColor_;
	};
	extern Editor editor_;
}

