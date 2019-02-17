#pragma once

#include "Engine/Resources/Object.h"
#include <memory>

namespace Sapphire
{
	class Actor : public Object
	{
	public:
		Actor();
		~Actor();

		void UpdateActor();

		float health_;
		float maxHealth_;

		glm::vec2 movementDirection_;
		glm::vec2 velocity_;

		float movementSpeed_;
		float jumpStrength_;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(cereal::base_class<Object>(this), health_);
		}
	};

	class ActorManager
	{
	public:
		~ActorManager();
		std::vector<std::shared_ptr<Actor>> allActors_;
		std::weak_ptr<Actor> CreateActor(const std::string &texturePath, glm::vec2 newPos, std::shared_ptr<Actor> actorPtr = nullptr);
		std::weak_ptr<Actor> GetActor(const int &elementIndex);
		bool DeleteActor(std::weak_ptr<Actor> actorToDelete);
		void DeleteAllActors();
		int actorCount = 0;

		template<class Archive>
		void serialize(Archive & archive) 
		{
			archive(allActors_);
		}
	};

	extern std::shared_ptr<ActorManager> actorManager_;
}

CEREAL_REGISTER_TYPE(Sapphire::Actor)


class Base
{
public:

	Base(){};
	virtual ~Base(){};
	int y;
	template <class Archive>
	void serialize(Archive & ar)
	{
		ar(y);
	}
};

class Derived : public Base
{
public:
	Derived() {};
	~Derived() {};
	template <class Archive>
	void serialize(Archive & ar)
	{
		ar(cereal::base_class<Base>(this), y);
	}
};

CEREAL_REGISTER_TYPE(Derived);

namespace MyNamespace
{
	class Base
	{
	public:

		Base() {};
		virtual ~Base() {};
		int y;
		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(y);
		}
	};

	class Derived : public Base
	{
	public:
		Derived() {};
		~Derived() {};
		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(cereal::base_class<Base>(this), y);
		}
	};
}

CEREAL_REGISTER_TYPE(MyNamespace::Derived);