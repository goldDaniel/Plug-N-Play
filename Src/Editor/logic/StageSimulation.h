#ifndef STAGE_SIMULATION_H
#define STAGE_SIMULATION_H

#include <Core.h>
#include <Game/Systems/PathFollowingSystem.h>
#include <Game/Systems/RenderSystem.h>

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
			cam_comp.view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			ECS->AddComponent(camera, cam_comp);
			render_system->SetCamera(camera);
		}

		//test stage
		StageData stage = LoadStageFromFile("Assets/Stages/Test Stage.stage");

		for (std::size_t i = 0; i < stage.enemy_start_times.size(); i++)
		{
			Entity enemy = ECS->CreateEntity();


			BezierPath path;
			path.time_start = stage.enemy_start_times[i];
			path.speed = 1.f / 6.f;
			path.curve = LoadPathFromFile(stage.enemy_paths[i]);

			glm::vec2 pos({ path.curve.valueAt(0).x, path.curve.valueAt(0).y });
			ECS->AddComponent(enemy, Transform{ pos, glm::vec2(1.f, 1.f), 0.f });

			ECS->AddComponent(enemy, path);
			ECS->AddComponent(enemy, Renderable({ glm::vec4(1,1,1,1), Texture::CreateTexture(stage.enemy_textures[i]) }));
		}
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