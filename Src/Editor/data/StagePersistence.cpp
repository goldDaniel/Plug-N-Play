#include "StagePersistence.h"

StagePersistence::StagePersistence()
{
	//caching paths 
	auto path = std::filesystem::path("Assets/Paths");
	path.make_preferred();
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		const auto& path_str = file.path().string();
		Bezier::Bezier<3> curve = LoadPathFromFile(path_str);

		path_cache.insert({ path_str, curve });
	}

	//caching paths 
	path = std::filesystem::path("Assets/Textures");
	path.make_preferred();
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		const auto& path_str = file.path().string();
		Texture* t = Texture::CreateTexture(path_str);

		texture_cache.insert({ path_str, t });
	}
}

const std::unordered_map<std::string, Texture*>& StagePersistence::GetTextureCache()
{
	return texture_cache;
}

const std::unordered_map<std::string, Bezier::Bezier<3>>& StagePersistence::GetPathCache()
{
	return path_cache;
}

void StagePersistence::CreateDefaultEnemy(Entity entity, ECSController* const ECS)
{
	ECS->AddComponent(entity, Transform());


	BezierPath path;
	path.time_start = 0;
	path.speed = 1.f / 4.f;
	path.curve = path_cache.begin()->second;
	ECS->AddComponent(entity, path);

	Renderable renderable;

	auto file_path = std::filesystem::path("Assets/Textures/Enemy.png");
	file_path.make_preferred();
	if (texture_cache.find(file_path.string()) != texture_cache.end())
	{
		renderable.texture = texture_cache[file_path.string()];
	}
	else
	{
		renderable.texture = texture_cache.begin()->second;
	}
	ECS->AddComponent(entity, renderable);

	Collider collider;
	collider.category = Collider::Enemy;
	collider.collides_with = Collider::Player;
	collider.radius = 0.5f;
	ECS->AddComponent(entity, collider);
}

void StagePersistence::SaveStage(ECSController* const ECS, const std::set<Entity>& active_entities, const std::string& filepath)
{
	nlohmann::json output;

	std::size_t i = 0;
	for (const auto& entity : active_entities)
	{
		if (ECS->HasComponent<Transform>(entity))
		{
			const auto& trans = ECS->GetComponent<Transform>(entity);
			output["entities"][i]["transform"]["position"] = { trans.position.x, trans.position.y };
			output["entities"][i]["transform"]["scale"] = { trans.scale.x, trans.scale.y };
			output["entities"][i]["transform"]["rotation"] = trans.rotation;
		}

		if (ECS->HasComponent<BezierPath>(entity))
		{
			auto& path = ECS->GetComponent<BezierPath>(entity);
			output["entities"][i]["path"]["time_start"] = path.time_start;
			output["entities"][i]["path"]["speed"] = path.speed;

			for (auto& pair : path_cache)
			{
				bool same_curve = true;
				//couldnt get operator overloading working so we verify the data here
				const auto& points_0 = path.curve.getControlPoints();
				const auto& points_1 = pair.second.getControlPoints();

				if (points_0.size() == points_1.size())
				{
					for (std::size_t idx = 0; idx < points_0.size() && same_curve; idx++)
					{
						const auto& p0 = points_0[idx];
						const auto& p1 = points_1[idx];

						if (p0.x != p1.x || p0.y != p1.y)
						{
							same_curve = false;
						}
					}
				}
				if (same_curve)
				{
					output["entities"][i]["path"]["bezier"] = pair.first;
				}
			}
		}

		if (ECS->HasComponent<Collider>(entity))
		{
			const auto& collider = ECS->GetComponent<Collider>(entity);

			output["entities"][i]["collider"]["category"] = collider.category;
			output["entities"][i]["collider"]["collides"] = collider.collides_with;
			output["entities"][i]["collider"]["radius"] = collider.radius;
		}

		if (ECS->HasComponent<Renderable>(entity))
		{
			const auto& renderable = ECS->GetComponent<Renderable>(entity);
			output["entities"][i]["renderable"]["color"] = {
															 renderable.color.r,
															 renderable.color.g,
															 renderable.color.b,
															 renderable.color.a,
			};

			for (const auto& pair : texture_cache)
			{
				if (pair.second == renderable.texture)
				{
					output["entities"][i]["renderable"]["texture"] = pair.first;
				}
			}

		}


		SaveStringToFile(output.dump(), filepath);
		i++;
	}
}

void StagePersistence::LoadStage(ECSController* const ECS, const std::string& filepath)
{
	std::set<Entity> dummy;
	LoadStage(ECS, dummy, filepath);
}

void StagePersistence::LoadStage(ECSController* const ECS, std::set<Entity>& active_entities, const std::string& filepath)
{
	nlohmann::json data = LoadStageFromFile(filepath);


	for (std::size_t i = 0; i < data["entities"].size(); i++)
	{
		Entity entity = ECS->CreateEntity();
		active_entities.insert(entity);

		if (!data["entities"][i]["transform"].is_null())
		{
			Transform trans;
			trans.position.x = data["entities"][i]["transform"]["position"][0];
			trans.position.y = data["entities"][i]["transform"]["position"][1];
			trans.scale.x = data["entities"][i]["transform"]["scale"][0];
			trans.scale.y = data["entities"][i]["transform"]["scale"][1];
			trans.rotation = data["entities"][i]["transform"]["rotation"];

			ECS->AddComponent(entity, trans);
		}

		if (!data["entities"][i]["path"].is_null())
		{
			BezierPath path;
			path.time_start = data["entities"][i]["path"]["time_start"];
			path.speed = data["entities"][i]["path"]["speed"];
			path.time = 0;
			path.curve = path_cache[data["entities"][i]["path"]["bezier"]];

			ECS->AddComponent(entity, path);
		}

		if (!data["entities"][i]["collider"].is_null())
		{
			Collider collider;
			collider.category = data["entities"][i]["collider"]["category"];
			collider.collides_with = data["entities"][i]["collider"]["collides"];
			collider.radius = data["entities"][i]["collider"]["radius"];

			ECS->AddComponent(entity, collider);
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

			ECS->AddComponent(entity, renderable);
		}

	}
}