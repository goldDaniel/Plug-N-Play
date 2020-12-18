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
		for (const auto& e0 : entities)
		{
			const auto& trans0 = ECS->GetComponent<Transform>(e0);
			const auto& collider0 = ECS->GetComponent<Collider>(e0);

			for (const auto& e1 : entities)
			{
				const auto& trans1 = ECS->GetComponent<Transform>(e1);
				const auto& collider1 = ECS->GetComponent<Collider>(e1);

				if ( (collider1.category & collider0.collides_with) != Collider::None)
				{
					if (glm::length(trans0.position - trans1.position) < collider0.radius + collider1.radius)
					{
						Collision col = 
						{
							e0, collider0.category, 
							e1, collider1.category 
						};
						Entity collision = ECS->CreateEntity();
						ECS->AddComponent<Collision>(collision, col);
					}	
				}
			}
		}
	}
};

#endif