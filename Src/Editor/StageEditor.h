#ifndef ENEMY_EDITOR_H
#define ENEMY_EDITOR_H

#include <Core.h>
#include <Editor/EditorApplication.h>
#include <Graphics/SpriteBatch.h>

#include <filesystem>

struct EnemyData
{
	float time_start = 0;
	
	std::string path_filepath;
	std::string texture_filepath;
};

class StageEditor : public Editor
{
private:

	//needs to be a char* due to imgui
	static const std::size_t name_buffer_size = 80;
	char stage_name[name_buffer_size] = "Stage";
	
	
	float stage_length = 30;
	float current_stage_time = 0;

	
	std::string current_path;
	std::string current_texture;

	std::map<std::string, Bezier::Bezier<3>> paths;
	std::vector<std::string> texture_filepaths;
	std::vector<EnemyData> enemies; 

	/// <summary>
	/// Used for file dialog UI
	/// </summary>
	imgui_addons::ImGuiFileBrowser file_dialog;
	 
public:
	StageEditor()
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

	void OnGUIRender() override
	{
		bool open_dialog = false;
		bool save_dialog = false;
		ImGui::Begin("Stage Editor", 0, ImGuiWindowFlags_NoMove     |
									    ImGuiWindowFlags_NoCollapse |
									    ImGuiWindowFlags_NoResize);
		{
			if (ImGui::Button("New Stage"))
			{

			}
			ImGui::SameLine();
			if (ImGui::Button("Save Stage"))
			{
				save_dialog = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Open Stage"))
			{
				open_dialog = true;
			}
			ImGui::Separator();

			ImGui::Text("Stage Info");
			ImGui::InputText("Stage Name", stage_name, name_buffer_size);
			ImGui::SliderFloat("Stage Length", &stage_length, 0, 120, 0, 1);
			ImGui::SliderFloat("Current Stage Time", &current_stage_time, 0, stage_length, 0, 1);

			ImGui::Separator();

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
					EnemyData data;
					data.time_start = spawn_time;
					data.path_filepath = current_path;
					data.texture_filepath = current_texture;

					enemies.push_back(data);

					//sort the list when we add a new element
					std::sort(enemies.begin(), enemies.end(), [](const EnemyData& a, const EnemyData& b)
					{
						return a.time_start < b.time_start;
					});

					current_path = "";
					ImGui::CloseCurrentPopup(); 
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) 
				{ 
					ImGui::CloseCurrentPopup(); 
					current_path = "";
				}					
				ImGui::EndPopup();
			}



			
		}
		ImGui::End();
		
		ImGui::Begin("Stage Data", 0, ImGuiWindowFlags_NoMove     | 
									  ImGuiWindowFlags_NoCollapse | 
		                              ImGuiWindowFlags_NoResize);
		{
			ImGui::BeginTable("Enemies", 3);
			for (const auto& data : enemies)
			{
				ImGui::TableNextColumn();
				ImGui::Text("%.1f", data.time_start);
				ImGui::TableNextColumn();
				ImGui::Text("%s", data.path_filepath.c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%s", data.texture_filepath.c_str());
			}
			ImGui::EndTable();
		}
		ImGui::End();
		

		ImGui::ShowDemoWindow(); 
		
		if (open_dialog) ImGui::OpenPopup("Open Stage");
		if (save_dialog) ImGui::OpenPopup("Save Stage");

		if (file_dialog.showFileDialog("Open Stage", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".path"))
		{
		}
		if (file_dialog.showFileDialog("Save Stage", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".path"))
		{
		}
	}

	void Render(ShapeRenderer& sh) override
	{
		sh.SetColor({1.f, 1.f, 1.f, 1.f});
		if (paths.find(current_path) != paths.end())
		{
			auto& curve = paths[current_path];

			float step = 0.05f;
			for (float t = step; t <= 1; t += step)
			{
				const auto& p00 = curve.valueAt(t - step);
				glm::vec2 p0 = { p00.x, p00.y };
				
				const auto& p10 = curve.valueAt(t);
				glm::vec2 p1 = { p10.x, p10.y };

				sh.Line(p0, p1);
			}
		}
	}

	void OnMouseButtonDown() override
	{

	}

	void OnMouseButtonUp() override
	{

	}

	void Update(glm::vec2 mouse_world_pos) override
	{

	}
};
	

#endif