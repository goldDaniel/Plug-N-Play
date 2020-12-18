#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include <Core.h>

class CollisionSystem : public System
{
private:
	

public:

	CollisionSystem(ECSController* ECS) : System::System(ECS) {}


	void Update(float dt)
	{

		std::set<Entity> to_remove;

		for (const auto& e0 : entities)
		{
			const auto& trans0 = ECS->GetComponent<Transform>(e0);
			const auto& collider0 = ECS->GetComponent<Collider>(e0);

			for (const auto& e1 : entities)
			{
				const auto& trans1 = ECS->GetComponent<Transform>(e1);
				const auto& collider1 = ECS->GetComponent<Collider>(e1);

				if ( (collider0.category & collider1.collides_with) != Collider::None)
				{
					if (glm::length(trans0.position - trans1.position) < collider0.radius + collider1.radius)
					{
						to_remove.insert(e0);
						to_remove.insert(e1);
					}
				}
			}
		}
		for (const auto& entity : to_remove)
		{
			ECS->DestroyEntity(entity);
		}
	}
};

#endif