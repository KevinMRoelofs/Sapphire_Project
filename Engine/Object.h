#pragma once
#include "Graphics/Texture.h"
#include "Graphics/Mesh.h"

#include <list>

#include "Engine/Dependencies/glm/glm.hpp"
#include <vector>
#include <memory>

namespace Sapphire
{
	class Object : public std::enable_shared_from_this<Object>
	{
	public:
		Object();
		~Object();

		virtual void Virt(){};
		//void Create(const char* texturePath, glm::vec2 newPos);

		const int &GetId() const;

		std::shared_ptr<Mesh> mesh_ = nullptr;
		const std::shared_ptr<Mesh> &GetMesh() const;
		void SetMesh(const std::shared_ptr<Mesh> &newMesh);

		std::shared_ptr<Texture> texture_ = nullptr;
		const std::shared_ptr<Texture> &GetTexture() const;
		void SetTexture(const std::shared_ptr<Texture> &newTexture);

		const glm::vec2 & GetPositionWorld() const;
		void SetPositionWorld(const glm::vec2& newPosition);
		void SetPositionWorld(const float& newX, const float& newY);
		
		const glm::vec2 & GetPositionScreen();
		void SetPositionScreen(const glm::vec2& newPosition);
		void SetPositionScreen(const float& newX, const float& newY);
		void UpdatePositionScreen();


		const glm::vec2 & GetScale() const;
		void SetScale(const glm::vec2& newScale);
		void SetScale(const float& newX, const float& newY);

		float rotation_;
		glm::vec2 pivot_;

		glm::mat4x4 model_;

		void UpdateTransform();

		bool operator==(Object ob) const;

	private:
		glm::vec2 positionWorld_;
		glm::vec2 positionScreen_;
		glm::vec2 scale_;
		const int objectId_;
	};

	class ObjectManager
	{
	public:
		ObjectManager();
		~ObjectManager();

		std::shared_ptr<Object> CreateObject(const char* texturePath, const char* meshName, glm::vec3 newPos, std::shared_ptr<Object> ObjectPtr);
		bool DestroyObject(std::weak_ptr<Object> objectToDelete);
		void Update();

		std::vector<std::shared_ptr<Object>> allObjects_;
		int lastObjectId_;
	};

	extern ObjectManager objectManager_;
}

