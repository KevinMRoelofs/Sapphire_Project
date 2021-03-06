#include "Actor.h"

#include "Engine/Engine.h"
#include "Engine/Utility/Input.h"
#include "Engine/Utility/Random.h"

namespace Sapphire
{
	std::shared_ptr<ActorManager> actorManager_(new ActorManager);

	Actor::Actor()
	{

		health_ = 100;
		maxHealth_ = health_;

		movementDirection_ = glm::vec3(0);
		velocity_ = glm::vec3(0);

		movementSpeed_ = 10;
		jumpStrength_ = 10;

		int i = 0;
	}

	Actor::~Actor()
	{

	}

	void Actor::UpdateActor()
	{
		velocity_ += engine_.gravity_ * engine_.deltaTime_ * engine_.timeScale_;
		SetPositionWorld(GetPositionWorld() + velocity_ * engine_.deltaTime_ * engine_.timeScale_ + movementDirection_ * movementSpeed_ * engine_.deltaTime_ * engine_.timeScale_);
		const glm::vec2& objectPos = GetPositionWorld();
		if (objectPos.y < 0)
		{
			SetPositionWorld(objectPos.x, 0.f);
			velocity_ = glm::vec2(0);
		}
		if (inputManager_.AllKeyStates_[Key_L].isPressed)
		{
			velocity_.y += random_.GenerateFloat(jumpStrength_ / 2, jumpStrength_ * 2);
			int i = 0;
		}

		if (inputManager_.AllKeyStates_[Key_Z].isHeld) rotation_ += 5;
	}

	ActorManager::~ActorManager()
	{
		allActors_.clear();
	}

	std::weak_ptr<Actor> ActorManager::CreateActor(const std::string &texturePath, glm::vec2 newPos, std::shared_ptr<Actor> actorPtr)
	{
		if (!actorPtr)
		{
			actorPtr = std::make_shared<Actor>();
		}


		for (auto &actor : allActors_)
		{
			if (actorPtr.get() == actor.get())
			{
				return allActors_.back();
			}
		}
		objectManager_->CreateObject(texturePath, "Quad", newPos, actorPtr);

		allActors_.push_back(actorPtr);
		return allActors_.back();
	}

	std::weak_ptr<Actor> ActorManager::GetActor(const int& elementIndex)
	{
		if (allActors_.size() - 1 > elementIndex && elementIndex >= 0)
		{
			return allActors_[elementIndex];
		}
		return std::weak_ptr<Actor>();
	}
	bool ActorManager::DeleteActor(std::weak_ptr<Actor> actorToDelete)
	{
		if (actorToDelete.expired()) return false;
		int currentElementIndex = 0;
		for (auto &actor : allActors_)
		{
			if (actor.get() == actorToDelete.lock().get())
			{
				//printf("Destroyed Actor %i ", deleteActorObjectId);
				std::rotate(allActors_.begin(), allActors_.begin() + currentElementIndex + 1, allActors_.end()); //rotate ActorToDelete to end of vector
				allActors_.pop_back();
				std::rotate(allActors_.begin(), allActors_.end() - currentElementIndex, allActors_.end()); //rotate vector back to it was
				objectManager_->DeleteObject(actorToDelete);

				return true;
			}
			currentElementIndex++;
		}
		return false;
	}

	void ActorManager::DeleteAllActors()
	{
		allActors_.clear();
	}
}