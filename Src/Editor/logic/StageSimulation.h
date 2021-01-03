#ifndef STAGE_SIMULATION_H
#define STAGE_SIMULATION_H

#include <Core.h>
#include <Game/Systems/PathFollowingSystem.h>
#include <Game/Systems/RenderSystem.h>
#include <bezier.h>

#include <filesystem>

class StageSimulation
{
private:

	bool running = false;


	float stage_timer = 0.f;
	float stage_length = 30.f;
	
	float previous_time;

	std::unique_ptr<ECSController> ECS;

	std::shared_ptr<PathFollowingSystem> path_system;
	std::shared_ptr<RenderSystem> render_system;

	std::map<std::string, Bezier::Bezier<3>> path_cache;

	std::vector<Entity> active_entities;
	std::vector<Entity> to_remove;
	

public:

	StageSimulation()
	{
		previous_time = SDL_GetTicks();

		//caching paths 
		auto path = std::filesystem::path("Assets/Paths");
		path.make_preferred();
		for (const auto& file : std::filesystem::directory_iterator(path))
		{
			const auto& path_str = file.path().string();
			Bezier::Bezier<3> curve = LoadPathFromFile(path_str);

			path_cache.insert({ path_str, curve });
		}


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

	void DestroyEntity(Entity entity)
	{
		to_remove.push_back(entity);
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
		
		nlohmann::json data = LoadStageFromFile(filepath);


		for (std::size_t i = 0; i < data["entities"].size(); i++)
		{
			Entity enemy = ECS->CreateEntity();
			active_entities.push_back(enemy);

			if (!data["entities"][i]["transform"].is_null())
			{
				Transform trans;
				trans.position.x = data["entities"][i]["transform"]["position"][0];
				trans.position.y = data["entities"][i]["transform"]["position"][1];
				trans.scale.x = data["entities"][i]["transform"]["scale"][0];
				trans.scale.y = data["entities"][i]["transform"]["scale"][1];
				trans.rotation = data["entities"][i]["transform"]["rotation"];

				ECS->AddComponent(enemy, trans);
			}
			
			if (!data["entities"][i]["path"].is_null())
			{
				BezierPath path;
				path.time_start = data["entities"][i]["path"]["time_start"];
				path.speed = data["entities"][i]["path"]["speed"];
				path.time = 0;

				std::vector<Bezier::Point> control_points;
				for (std::size_t j = 0; j < 4; j++)
				{
					control_points.push_back({ data["entities"][i]["path"]["bezier"][j][0],
											   data["entities"][i]["path"]["bezier"][j][1] });
				}
				path.curve = Bezier::Bezier<3>(control_points);

				ECS->AddComponent(enemy, path);
			}

			
			if (!data["entities"][i]["renderable"].is_null())
			{
				//TODO: do we even use the alpha channel? Might not need to serialize it
				Renderable renderable;
				renderable.color = { data["entities"][i]["renderable"]["color"][0],
									 data["entities"][i]["renderable"]["color"][1],
									 data["entities"][i]["renderable"]["color"][2],
									 data["entities"][i]["renderable"]["color"][3],
				};
				renderable.texture = Texture::CreateTexture(data["entities"][i]["renderable"]["texture"]);

				ECS->AddComponent(enemy, renderable);
			}
			
		}

		path_system->SetElapsed(stage_timer);
	}

	void SaveStage(const std::string& filepath)
	{
		nlohmann::json output;

		

		for (std::size_t i = 0; i < active_entities.size(); i++)
		{
			Entity entity = active_entities[i];

			if (const auto trans = GetComponent<Transform>(entity))
			{
				output["entities"][i]["transform"]["position"] = { trans->position.x, trans->position.y };
				output["entities"][i]["transform"]["scale"] = { trans->scale.x, trans->scale.y };
				output["entities"][i]["transform"]["rotation"] = trans->rotation;
			}

			if (const auto path = GetComponent<BezierPath>(entity))
			{
				output["entities"][i]["path"]["time_start"] = path->time_start;
				output["entities"][i]["path"]["speed"] = path->speed;

				const auto& control_points = path->curve.getControlPoints();
				output["entities"][i]["path"]["bezier"] =
				{
					{control_points[0].x, control_points[0].y},
					{control_points[1].x, control_points[1].y},
					{control_points[2].x, control_points[2].y},
					{control_points[3].x, control_points[3].y},
				};
			}

			if (const auto renderable = GetComponent<Renderable>(entity))
			{
				output["entities"][i]["renderable"]["color"] = { 
																 renderable->color.r, 
																 renderable->color.g, 
																 renderable->color.b, 
																 renderable->color.a, 
															   };

				output["entities"][i]["renderable"]["texture"] = "Assets/Textures/Enemy.png";
			}


			SaveStringToFile(output.dump(), "Assets/Stages/NewFormat.stage");
		}
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
	
	}

	void UpdateEnemyPathing()
	{
		path_system->SetElapsed(stage_timer);
	}

	std::vector<std::string> GetCurveFilepaths()
	{
		std::vector<std::string> result;

		for (const auto& pair : path_cache)
		{
			result.push_back(pair.first);
		}

		return result;
	}

	const std::vector<Entity>& GetActiveEntities() 
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