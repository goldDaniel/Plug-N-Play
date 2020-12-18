#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include <Core.h>

class CollisionSystem : public System
{
private:
	

public:

	CollisionSystem(ECSController* ECS) : System::System(ECS)
	{
	}


	void Update(float dt)
	{
		for (const auto& entity : entities)
		{

		}
	}
};

#endif