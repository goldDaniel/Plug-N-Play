#ifndef ENEMY_CREATOR_H
#define ENEMY_CREATOR_H

#include <Core.h>

#include <filesystem>

class EnemyCreator
{
private:

	//stores the filepath of the currently selected path
	std::string current_path = "";

	//stores the filepath of the currently selected texture
	std::string current_texture = "";
	
	//all the textures we can select for enemies
	std::vector<std::string> texture_filepaths;

	//all the paths we can select
	std::map<std::string, Bezier::Bezier<3>> paths;


	//these are parallel arrays and will be used to serialize data
	std::vector<float> enemy_start_times;
	std::vector<std::string> enemy_paths;
	std::vector<std::string> enemy_textures;

public:
	EnemyCreator()
	{
		std::filesystem::path path("Assets/Textures");
		path.make_preferred();

		for (const auto& file : std::filesystem::directory_iterator(path))
		{
			texture_filepaths.push_back(file.path().string());
		}

		path = std::filesystem::path("Assets/Paths");
		path.make_preferred();
		for (const auto& file : std::filesystem::directory_iterator(path))
		{
			const auto& path_str = file.path().string();
			Bezier::Bezier<3> curve = LoadPathFromFile(path_str);

			paths.insert({ path_str, curve });
		}
	}

	void EnemyCreationGUI()
	{
		if (ImGui::Button("New Enemy"))
		{
			ImGui::OpenPopup("New Enemy");
		}
		
		// Always center this window when appearing
		ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("New Enemy", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static float spawn_time;
			ImGui::InputFloat("Spawn Time", &spawn_time, 0.1f, 0.5f);

			if (ImGui::BeginCombo("Path", current_path.c_str()))
			{
				for (const auto& pair : paths)
				{
					bool is_selected = (current_path == pair.first);
					if (ImGui::Selectable(pair.first.c_str(), is_selected))
					{
						current_path = pair.first;
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			if (ImGui::BeginCombo("Texture", current_texture.c_str()))
			{
				for (const auto& path : texture_filepaths)
				{
					bool is_selected = (current_path == path);
					if (ImGui::Selectable(path.c_str(), is_selected))
					{
						current_texture = path;
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				int index = 0;
				bool found = false;
				for (index = 0;
					!found && index < enemy_start_times.size();
					index++)
				{
					if (enemy_start_times[index] >= spawn_time)
					{
						index--;
						found = true;
					}
				}
				if (found)
				{
					enemy_start_times.insert(enemy_start_times.begin() + index, spawn_time);
					enemy_paths.insert(enemy_paths.begin() + index, current_path);
					enemy_textures.insert(enemy_textures.begin() + index, current_texture);
				}
				else
				{
					enemy_start_times.push_back(spawn_time);
					enemy_paths.push_back(current_path);
					enemy_textures.push_back(current_texture);
				}

				current_path = "";
				current_texture = "";
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				current_path = "";
				current_texture = "";
			}
			ImGui::EndPopup();
		}
	}

	void Render(ShapeRenderer& sh)
	{
		sh.SetColor({ 1.f, 1.f, 1.f, 1.f });
		if (paths.find(current_path) != paths.end())
		{
			auto& curve = paths[current_path];

			float step = 0.05f;
			for (float t = 0; t < 1; t += step)
			{
				const auto& p00 = curve.valueAt(t);
				glm::vec2 p0 = { p00.x, p00.y };

				const auto& p10 = curve.valueAt(t + step);
				glm::vec2 p1 = { p10.x, p10.y };

				sh.Line(p0, p1);
			}
		}
	}

	void SetStartTimes(const std::vector<float>& data)
	{
		this->enemy_start_times = data;
	}
	
	void SetPaths(const std::vector<std::string>& data)
	{
		this->enemy_paths = data;
	}

	void SetTextures(const std::vector<std::string>& data)
	{
		this->enemy_textures = data;
	}


	const std::vector<float>& GetStartTimes() const
	{
		return enemy_start_times;
	}

	const std::vector<std::string>& GetPaths() const
	{
		return enemy_paths;
	}

	const std::vector<std::string>& GetTextures() const
	{
		return enemy_textures;
	}
};

#endif