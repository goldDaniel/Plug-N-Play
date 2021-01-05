#include "StageSimulation.h"

StageSimulation::StageSimulation()
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

const std::unordered_map<std::string, Texture*>& StageSimulation::GetTextureCache()
{
	return persistence->GetTextureCache();
}

const std::unordered_map<std::string, Bezier::Bezier<3>>& StageSimulation::GetPathCache()
{
	return persistence->GetPathCache();
}

void StageSimulation::DestroyEntity(Entity entity)
{
	to_remove.insert(entity);
}

void StageSimulation::LoadStage(const std::string& filepath)
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

void StageSimulation::SaveStage(const std::string& filepath)
{
	persistence->SaveStage(ECS.get(), active_entities, filepath);
}

void StageSimulation::CloneEntity(Entity entity)
{
	//TODO:
}

void StageSimulation::SelectEntity(Entity& result, glm::vec2 mouse_world_pos)
{
	for (const auto& entity : active_entities)
	{
		if (const auto transform = GetComponent<Transform>(entity))
		{
			float radius = transform->scale.x > transform->scale.y ? transform->scale.x : transform->scale.y;
			radius *= 0.5f;

			if (glm::distance(transform->position, mouse_world_pos) < radius)
			{
				result = entity;
			}
		}
	}
}

void StageSimulation::AddDefaultEnemy()
{
	Entity enemy = ECS->CreateEntity();
	active_entities.insert(enemy);

	persistence->CreateDefaultEnemy(enemy, ECS.get());
}

void StageSimulation::UpdateEnemyPathing()
{
	path_system->SetElapsed(stage_timer);
}

const std::set<Entity>& StageSimulation::GetActiveEntities()
{
	return active_entities;
}

void StageSimulation::SetStageLength(float l)
{
	assert(l >= 30);

	stage_length = l;
}

void StageSimulation::Start()
{
	running = true;
}

void StageSimulation::Pause()
{
	running = false;
}

void StageSimulation::Stop()
{
	running = false;
	stage_timer = 0;
}

void StageSimulation::Update()
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

bool StageSimulation::IsRunning() const
{
	return running;
}

float StageSimulation::GetStageLength() const
{
	return stage_length;
}

float StageSimulation::GetStageTime() const
{
	return stage_timer;
}

void StageSimulation::SetStageTime(const float time)
{
	assert(time >= 0);

	this->stage_timer = time;
	path_system->SetElapsed(stage_timer);
}