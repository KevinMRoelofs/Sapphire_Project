#pragma once
#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/Mesh.h"

#include "Engine/Dependencies/glm/glm.hpp"
#include "Engine/Dependencies/cereal-master/include/cereal/types/vector.hpp"
#include "Engine/Dependencies/cereal-master/include/cereal/types/memory.hpp"
#include <vector>
#include <memory>
#include <array>

namespace glm
{
	template<class Archive>
	void serialize(Archive& archive, glm::vec2& vec2)
	{
		archive(cereal::make_nvp("x", vec2.x), cereal::make_nvp("y", vec2.y));
	}

	template<class Archive>
	void serialize(Archive& archive, glm::vec3& vec3)
	{
		archive(cereal::make_nvp("x", vec3.x), cereal::make_nvp("y", vec3.y), cereal::make_nvp("z", vec3.z));
	}
}

namespace Sapphire
{
	class Object
	{
	public:
		Object();
		virtual ~Object();

		const int &GetId() const;

		const std::shared_ptr<Mesh> &GetMesh() const;
		void SetMesh(const std::shared_ptr<Mesh> &newMesh);

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

		template<class Archive>
		void serialize(Archive & archive)
		{
			float posX = GetPositionWorld().x, posY = GetPositionWorld().y;
			archive(texture_, positionWorld_, objectId_);
		}

	private:
		std::shared_ptr<Texture> texture_ = nullptr;
		std::shared_ptr<Mesh> mesh_ = nullptr;
		glm::vec2 positionWorld_;
		glm::vec2 positionScreen_;
		glm::vec2 scale_;
		int objectId_ = -1;
	};

	class ObjectManager
	{
	public:
		ObjectManager();
		~ObjectManager();

		std::shared_ptr<Object> CreateObject(const std::string texturePath, const char* meshName, glm::vec2 newPos, std::shared_ptr<Object> ObjectPtr);

		bool DeleteObject(std::weak_ptr<Object> objectToDelete);
		void DeleteAllObjects();

		std::vector<std::weak_ptr<Object>> GetAllObjects();
		int lastObjectId_;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(allObjects_);
		}
	private:
		std::vector<std::shared_ptr<Object>> allObjects_;

	};

	extern std::shared_ptr<ObjectManager> objectManager_;
}

CEREAL_REGISTER_TYPE(Sapphire::Object)