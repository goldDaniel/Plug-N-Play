#ifndef STAGE_PERSISTENCE_H
#define STAGE_PERSISTENCE_H

#include <Core.h>
#include <Application.h>
#include <filesystem>

class StagePersistence
{
private:

	std::unordered_map<std::string, Texture*> texture_cache;
	std::unordered_map<std::string, Bezier::Bezier<3>> path_cache;

public:

	StagePersistence();

	const std::unordered_map<std::string, Texture*>& GetTextureCache();

	const std::unordered_map<std::string, Bezier::Bezier<3>>& GetPathCache();

	void CreateDefaultEnemy(Entity entity, ECSController* const ECS);

	void SaveStage(ECSController* const ECS, const std::set<Entity>& active_entities, const std::string& filepath);;

	void LoadStage(ECSController* const ECS, std::set<Entity>& active_entities, const std::string& filepath);
};

#endif