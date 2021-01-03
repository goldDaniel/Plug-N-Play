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

	StageSimulation()
	{
		previous_time = SDL_GetTicks();

		persistence = std::make_unique<StagePersistence>();


		ECS = std::make_unique<ECSController>();


		Signature pathSig;
		pathSig.set(ECS->GetComponentType<Transform>());
		pathSig.set(ECS->GetComponentType<BezierPath>());
		path_system = ECS->RegisterSystem<PathFollowingSystem>(pathSig);

		Signature renderSig;
		renderSig.set(ECS->GetComponentType<Transform>());
		renderSig.set(ECS->GetComponentType<Renderable>());
		render_system = ECS->RegisterSystem<RenderSystem>(renderSig);

		//TODO: This is a bad assumption, fix this later
		float window_width = 1920;
		float window_height = 1080;
		render_system->SetScreenDimensions(window_width, window_height);


		//creates the camera entity
		{
			Entity camera = ECS->CreateEntity();
			Camera cam_comp;

			cam_comp.proj = glm::perspective(glm::radians(67.f), (float)window_width / (float)window_height, 1.f, 100.f);
			cam_comp.view = glm::lookAt(glm::vec3(0, 0, 11), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			ECS->AddComponent(camera, cam_comp);
			render_system->SetCamera(camera);
		}
	}

	const std::map<Texture*, std::string>& GetTextureCache()
	{
		return persistence->GetTextureCache();
	}

	void DestroyEntity(Entity entity)
	{
		to_remove.insert(entity);
	}

	void LoadStage(const std::string& filepath)
	{
		stage_timer = 0;
		for (const auto& entity : active_entities)
		{
			ECS->DestroyEntity(entity);
		}
		active_entities.clear();
		to_remove.clear();
		
		persistence->LoadStage(ECS.get(), active_entities, filepath);

		path_system->SetElapsed(stage_timer);
	}

	void SaveStage(const std::string& filepath)
	{
		persistence->SaveStage(ECS.get(), active_entities, filepath);
	}

	template<typename T>
	T* GetComponent(Entity entity) const
	{
		if (ECS->HasComponent<T>(entity))
		{
			return &ECS->GetComponent<T>(entity);
		}

		return nullptr;
	}

	void SelectEntity(Entity& result, glm::vec2 mouse_world_pos)
	{
		for (const auto& entity: active_entities)
		{
			if (const auto transform = GetComponent<Transform>(entity))
			{
				float radius = transform->scale.x > transform->scale.y ? transform->scale.x : transform->scale.y;

				if (glm::distance(transform->position, mouse_world_pos) < radius)
				{
					result = entity;
				}
			}
		}
	}

	void AddDefaultEnemy()
	{
		Entity enemy = ECS->CreateEntity();
		active_entities.insert(enemy);

		persistence->CreateDefaultEnemy(enemy, ECS.get());
	}

	void UpdateEnemyPathing()
	{
		path_system->SetElapsed(stage_timer);
	}

	std::vector<std::string> GetCurveFilepaths()
	{
		std::vector<std::string> result;

		/*for (const auto& pair : path_cache)
		{
			result.push_back(pair.first);
		}*/

		return result;
	}

	const std::set<Entity>& GetActiveEntities() 
	{
		return active_entities;
	}

	void SetStageLength(float l)
	{
		assert(l >= 30);

		stage_length = l;
	}

	void Start() 
	{
		running = true;
	}

	void Pause()
	{
		running = false;
	}

	void Stop()
	{
		running = false;
		stage_timer = 0;
	}

	void Update()
	{
		float current_time = SDL_GetTicks();
		float dt = (current_time - previous_time) / 1000.f;
		previous_time = current_time;

		for (const auto& e : to_remove)
		{
			ECS->DestroyEntity(e);
			active_entities.erase(active_entities.find(e));
		}
		to_remove.clear();

		if (running)
		{
			stage_timer += dt;
			path_system->SetElapsed(stage_timer);
		}
		render_system->Update(dt);

		if (stage_timer >= stage_length)
		{
			stage_timer = stage_length;
			running = false;
		}
	}

	bool IsRunning() const
	{
		return running;
	}

	float GetStageLength() const 
	{
		return stage_length;
	}

	float GetStageTime() const
	{
		return stage_timer;
	}

	void SetStageTime(const float time)
	{
		assert(time >= 0);

		this->stage_timer = time;
		path_system->SetElapsed(stage_timer);
	}
};

#endif