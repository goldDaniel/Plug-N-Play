#ifndef COLLISION_RESOLUTION_SYSTEM_H
#define COLLISION_RESOLUTION_SYSTEM_H

#include <Core.h>

class CollisionResolutionSystem : public System
{
private:


public:

	CollisionResolutionSystem(ECSController* ECS) : System::System(ECS) {}


	void Update(float dt)
	{

		std::set<Entity> to_remove;

		for (const auto& entity : entities)
		{
			const auto& collision = ECS->GetComponent<Collision>(entity);

			if (collision.type0 == Collider::Bullet &&
				collision.type1 == Collider::Enemy)
			{
				std::cout << "collision" << std::endl;
				to_remove.insert(collision.e0);
				to_remove.insert(collision.e1);
				
				//collision entities only hold collision data, so we must delete 
				to_remove.insert(entity);
			}
		}

		for (const auto& entity : to_remove)
		{
			ECS->DestroyEntity(entity);
		}
	}
};

#endif