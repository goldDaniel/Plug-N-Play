#ifndef STAGE_PERSISTENCE_H
#define STAGE_PERSISTENCE_H

#include <Core.h>
#include <Application.h>
#include <filesystem>

class StagePersistence
{
private:

	std::map<Texture*, std::string> texture_cache;
	std::map<std::string, Bezier::Bezier<3>> path_cache;

public:

	StagePersistence()
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

			texture_cache.insert({ t, path_str });
		}
	}

	const std::map<Texture*, std::string>& GetTextureCache()
	{
		return texture_cache;
	}

	const std::map<std::string, Bezier::Bezier<3>>& GetPathCache()
	{
		return path_cache;
	}

	void CreateDefaultEnemy(Entity entity, ECSController* const ECS)
	{
		ECS->AddComponent(entity, Transform());
		

		BezierPath path;
		path.time_start = 0;
		path.speed = 1.f / 4.f;
		path.curve = path_cache.begin()->second;
		ECS->AddComponent(entity, path);

		Renderable renderable;
		renderable.texture = texture_cache.begin()->first;
		ECS->AddComponent(entity, renderable);
	}

	void SaveStage(ECSController * const ECS, const std::set<Entity>& active_entities, const std::string& filepath)
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
				output["entities"][i]["path"]["speed"]      = path.speed;

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

			if (ECS->HasComponent<Renderable>(entity))
			{
				const auto& renderable = ECS->GetComponent<Renderable>(entity);
				output["entities"][i]["renderable"]["color"] = {
																 renderable.color.r,
																 renderable.color.g,
																 renderable.color.b,
																 renderable.color.a,
				};

				output["entities"][i]["renderable"]["texture"] = texture_cache[renderable.texture];
			}


			SaveStringToFile(output.dump(), filepath);
			i++;
		}
	}

	void LoadStage(ECSController * const ECS, std::set<Entity>& active_entities, const std::string& filepath)
	{
		nlohmann::json data = LoadStageFromFile(filepath);


		for (std::size_t i = 0; i < data["entities"].size(); i++)
		{
			Entity enemy = ECS->CreateEntity();
			active_entities.insert(enemy);

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
				path.curve = path_cache[data["entities"][i]["path"]["bezier"]];

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
	}
};

#endif