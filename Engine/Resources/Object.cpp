#include "Object.h"

#include "Engine/Graphics/Graphics.h"
#include "Engine/Engine.h"

#include <Engine/Dependencies/glm/gtc/matrix_transform.hpp>


namespace Sapphire
{
	std::shared_ptr<ObjectManager> objectManager_(new ObjectManager);


	Object::Object()
	{
		pivot_ = glm::vec2(0);
		positionWorld_ = glm::vec2(0);
		rotation_ = 0.0f;
		scale_ = glm::vec2(1);
		model_ = glm::mat4x4(1);

		if (objectId_ == -1) objectId_ = objectManager_->lastObjectId_++;

	}

	Object::~Object()
	{

	}

	const int& Object::GetId() const
	{
		return objectId_;
	}

	const std::shared_ptr<Mesh>& Object::GetMesh() const
	{
		return mesh_;
	}

	void Object::SetMesh(const std::shared_ptr<Mesh>& newMesh)
	{
		mesh_ = newMesh;
	}

	const std::shared_ptr<Texture> &Object::GetTexture() const
	{
		return texture_;
	}

	void Object::SetTexture(const std::shared_ptr<Texture> &newTexture)
	{
		texture_ = newTexture;
	}

	const glm::vec2& Object::GetPositionWorld() const
	{
		return positionWorld_;
	}

	void Object::SetPositionWorld(const float& newX, const float& newY)
	{
		positionWorld_ = glm::vec2(newX, newY);
		UpdateTransform();
	}

	void Object::SetPositionWorld(const glm::vec2& newPosition)
	{
		positionWorld_ = newPosition;
		UpdateTransform();
	}

	const glm::vec2& Object::GetPositionScreen()
	{
		return positionScreen_;
	}

	void Object::SetPositionScreen(const glm::vec2& newPosition)
	{
		positionScreen_ = newPosition;
		positionWorld_ = glm::vec2(positionScreen_.x / engine_.GetTileSize() - -graphics_.camera_.GetPositionWorld().x / engine_.GetTileSize(), -positionScreen_.y / engine_.GetTileSize() + graphics_.camera_.GetPositionWorld().y / engine_.GetTileSize());
		UpdateTransform();
	}

	void Object::SetPositionScreen(const float& newX, const float& newY)
	{
		positionScreen_ = glm::vec2(newX, newY);
		positionWorld_ = glm::vec2(positionScreen_.x / engine_.GetTileSize() - -graphics_.camera_.GetPositionWorld().x / engine_.GetTileSize(), -positionScreen_.y / engine_.GetTileSize() + graphics_.camera_.GetPositionWorld().y / engine_.GetTileSize());
		UpdateTransform();
	}

	void Object::UpdatePositionScreen()
	{
		positionScreen_ = glm::vec2(positionWorld_.x * engine_.GetTileSize() + -graphics_.camera_.GetPositionWorld().x + float(graphics_.windowWidth_) / 2, -positionWorld_.y * engine_.GetTileSize() + graphics_.camera_.GetPositionWorld().y + float(graphics_.windowHeight_) / 2);
	}

	const glm::vec2& Object::GetScale() const
	{
		return scale_;
	}

	void Object::SetScale(const glm::vec2& newScale)
	{
		scale_ = newScale;
		UpdateTransform();
	}

	void Object::SetScale(const float& newX, const float& newY)
	{
		scale_ = glm::vec2(newX, newY);
		UpdateTransform();
	}

	void Object::UpdateTransform()
	{
		model_ = glm::mat4x4(1);
		glm::vec3 const tileSizePos(positionWorld_.x * engine_.GetTileSize(), positionWorld_.y * engine_.GetTileSize(), 0);
		glm::vec3 const tileSizePivot(pivot_.x * engine_.GetTileSize(), pivot_.y * engine_.GetTileSize(), 0);
		model_ = glm::translate(model_, tileSizePos - tileSizePivot);
		model_ = glm::rotate(model_, glm::radians(rotation_), glm::vec3(0.0, 0.0, 1.0));
		glm::vec3 const tileSizeScale(scale_.x* texture_->width_, scale_.y*texture_->height_, 0);
		model_ = glm::scale(model_, tileSizeScale);
		int i = 0;
	}

	bool Object::operator==(Object ob) const
	{
		return objectId_ == ob.objectId_;
	}


	ObjectManager::ObjectManager()
	{
		lastObjectId_ = 0;
		//allObjects_.reserve(100);
	}

	ObjectManager::~ObjectManager()
	{
		allObjects_.clear();
		int i = 0;
	}

	std::shared_ptr<Object> ObjectManager::CreateObject(const std::string texturePath, const char* meshName, glm::vec2 newPos, std::shared_ptr<Object> objectPtr = nullptr)
	{
		if (!objectPtr)
		{
			objectPtr = std::make_shared<Object>();
		}

		objectPtr->SetMesh(meshManager_.FindMesh(meshName));
		objectPtr->SetTexture(textureManager_.GetTexture(texturePath));
		objectPtr->SetPositionWorld(newPos);

		for (auto &object : allObjects_)
		{
			if (objectPtr.get() == object.get())
			{
				return objectPtr;
			}
		}

		allObjects_.push_back(objectPtr);
		return objectPtr;
	}

	bool ObjectManager::DeleteObject(std::weak_ptr<Object> objectToDelete)
	{
		if (objectToDelete.expired()) return false;
		int currentElementIndex = 0;
		for (auto &object : allObjects_)
		{
			if (object == objectToDelete.lock())
			{
				std::rotate(allObjects_.begin(), allObjects_.begin() + currentElementIndex + 1, allObjects_.end());
				allObjects_.pop_back();
				std::rotate(allObjects_.begin(), allObjects_.end() - currentElementIndex, allObjects_.end());
				return true;
			}
			currentElementIndex++;
		}
		return false;
	}

	void ObjectManager::DeleteAllObjects()
	{
		lastObjectId_ = 0;
		allObjects_.clear();
	}
	std::vector<std::weak_ptr<Object>> ObjectManager::GetAllObjects()
	{
		std::vector<std::weak_ptr<Object>> weakObjects;
		for(auto &object : allObjects_)
		{
			weakObjects.push_back(object);
		}
		return weakObjects;
	}
}
