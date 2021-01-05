#ifndef STAGE_SIMULATION_H
#define STAGE_SIMULATION_H

#include <Core.h>
#include <Game/Systems/PathFollowingSystem.h>
#include <Game/Systems/RenderSystem.h>
#include <bezier.h>

#include <Editor/data/StagePersistence.h>

class StageSimulation
{
private:

	bool running = false;


	float stage_timer = 0.f;
	float stage_length = 30.f;
	
	float previous_time;

	std::unique_ptr<ECSController> ECS;

	std::unique_ptr<StagePersistence> persistence;

	std::shared_ptr<PathFollowingSystem> path_system;
	std::shared_ptr<RenderSystem> render_system;

	std::set<Entity> active_entities;
	std::set<Entity> to_remove;

public:

	StageSimulation();

	[[nodiscard]] const std::unordered_map<std::string, Texture*>& GetTextureCache();

	[[nodiscard]] const std::unordered_map<std::string, Bezier::Bezier<3>>& GetPathCache();

	[[nodiscard]] const std::set<Entity>& GetActiveEntities();

	[[nodiscard]] bool IsRunning() const;

	[[nodiscard]] float GetStageLength() const;

	[[nodiscard]] float GetStageTime() const;

	[[nodiscard]] Entity CloneEntity(Entity entity);
	
	void DestroyEntity(Entity entity);

	void LoadStage(const std::string& filepath);

	void SaveStage(const std::string& filepath);

	void SelectEntity(Entity& result, glm::vec2 mouse_world_pos);

	void AddDefaultEnemy();

	void UpdateEnemyPathing();

	void SetStageLength(float l);

	void Start();

	void Pause();

	void Stop();

	void Update();

	void SetStageTime(const float time);

	/// <summary>
	/// Checks if an entity has a component, and returns it if it does.
	/// We return a pointer rather than a reference as the pointer will be null
	/// if an entity does not have a component
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="entity"></param>
	/// <returns></returns>
	template<typename T>
	T* GetComponent(Entity entity) const
	{
		if (ECS->HasComponent<T>(entity))
		{
			return &ECS->GetComponent<T>(entity);
		}

		return nullptr;
	}
};

#endif