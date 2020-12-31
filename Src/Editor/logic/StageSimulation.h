#ifndef STAGE_SIMULATION_H
#define STAGE_SIMULATION_H

#include <Core.h>
#include <Game/Systems/PathFollowingSystem.h>
#include <Game/Systems/RenderSystem.h>

#include <bezier.h>

class StageSimulation
{
private:

	struct SimulationData
	{
		std::map<std::size_t, Entity> entity_map;

		StageData stage_data;
	};

	bool running = false;


	float stage_timer = 0.f;
	float stage_length = 30.f;
	
	float previous_time;

	std::unique_ptr<ECSController> ECS;

	std::shared_ptr<PathFollowingSystem> path_system;
	std::shared_ptr<RenderSystem> render_system;


	
	std::map<std::string, Bezier::Bezier<3>> path_cache;

	SimulationData sim_data;

public:

	StageSimulation()
	{
		previous_time = SDL_GetTicks();

		ECS = std::make_unique<ECSController>();

		ECS->RegisterComponent<Transform>();
		ECS->RegisterComponent<Camera>();
		ECS->RegisterComponent<BezierPath>();
		ECS->RegisterComponent<Renderable>();

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

	void LoadStage(const std::string& filepath)
	{
		stage_timer = 0;
		for (const auto& pair : sim_data.entity_map)
		{
			ECS->DestroyEntity(pair.second);
		}
		

		sim_data.entity_map.clear();

		sim_data.stage_data = LoadStageFromFile(filepath);

		std::map<std::string, Bezier::Bezier<3>> path_cache;

		for (std::size_t i = 0; i < sim_data.stage_data.enemy_start_times.size(); i++)
		{
			Entity enemy = ECS->CreateEntity();
			sim_data.entity_map.insert({ i, enemy });

			BezierPath path;
			path.time_start = sim_data.stage_data.enemy_start_times[i];
			path.speed = sim_data.stage_data.enemy_speeds[i];
			
			Bezier::Bezier<3> curve;
			if (path_cache.find(sim_data.stage_data.enemy_paths[i]) != path_cache.end())
			{
				curve = path_cache[sim_data.stage_data.enemy_paths[i]];
			}
			else
			{
				curve = LoadPathFromFile(sim_data.stage_data.enemy_paths[i]);
				path_cache[sim_data.stage_data.enemy_paths[i]] = curve;
			}
			path.curve = curve;
			ECS->AddComponent(enemy, path);
			
			glm::vec2 pos({ path.curve.valueAt(0).x, path.curve.valueAt(0).y });
			ECS->AddComponent(enemy, Transform{ pos, glm::vec2(1.f, 1.f), 0.f });

			
			Texture* tex = Texture::CreateTexture(sim_data.stage_data.enemy_textures[i]);
			ECS->AddComponent(enemy, Renderable({ glm::vec4(1,1,1,1), tex }));
		}
	}

	template<typename T>
	const T& GetComponent(Entity entity) const
	{
		return ECS->GetComponent<T>(entity);
	}

	void SelectEntity(Entity& result, glm::vec2 mouse_world_pos)
	{
		for (const auto& pair : sim_data.entity_map)
		{
			Entity e = pair.second;

			const auto& transform = ECS->GetComponent<Transform>(e);

			float radius = transform.scale.x > transform.scale.y ? transform.scale.x : transform.scale.y;

			if (glm::distance(transform.position, mouse_world_pos) < radius)
			{
				result = e;
			}
		}
	}

	SimulationData& GetStageData() 
	{
		return sim_data;
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

		for (const auto& pair : sim_data.entity_map)
		{
			Entity e = pair.second;

			auto& path = ECS->GetComponent<BezierPath>(e);
			path.time_start = sim_data.stage_data.enemy_start_times[pair.first];
			path.speed = sim_data.stage_data.enemy_speeds[pair.first];

			path_system->SetElapsed(stage_timer);
		}

		if (running)
		{
			stage_timer += dt;
			path_system->Update(dt);
		}
		render_system->Update(dt);
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
		path_system->SetElapsed(time);
	}
};

#endif