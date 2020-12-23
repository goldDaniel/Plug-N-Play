#include "StageEditor.h"


#include <filesystem>



StageEditor::StageEditor()
{
	s = SpriteBatch::CreateSpriteBatch();

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

void StageEditor::OnGUIRender() 
{
	bool open_dialog = false;
	bool save_dialog = false;
	ImGui::Begin("Stage Editor", 0, ImGuiWindowFlags_NoMove |
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
		ImGui::InputInt("Stage Length", &stage_data.stage_length, 1, 5);
		ImGui::SliderFloat("Current Stage Time", &stage_data.current_time, 0, stage_data.stage_length, 0, 1);

		if (stage_data.current_time > stage_data.stage_length) stage_data.current_time = stage_data.stage_length;
		if (stage_data.current_time < 0) stage_data.current_time = 0;

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
				int index = 0;
				bool found = false;
				for (index = 0;
					 !found && index < stage_data.enemy_start_times.size();
					 index++)
				{
					if (stage_data.enemy_start_times[index] >= spawn_time)
					{
						index--;
						found = true;
					}
				}
				if (found)
				{
					stage_data.enemy_start_times.insert(stage_data.enemy_start_times.begin() + index, spawn_time);
					stage_data.enemy_paths.insert(stage_data.enemy_paths.begin() + index, current_path);
					stage_data.enemy_textures.insert(stage_data.enemy_textures.begin() + index, current_texture);
				}
				else
				{
					stage_data.enemy_start_times.push_back(spawn_time);
					stage_data.enemy_paths.push_back(current_path);
					stage_data.enemy_textures.push_back(current_texture);
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
	ImGui::End();

	ImGui::Begin("Stage Data", 0, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize);
	{

		ImGui::NewLine();

		ImVec4 time_color({ .6f, .2f, .2f, 1.f });
		ImVec4 path_color({ 0.4f, 0.6f, 0.4f, 1.f });
		ImVec4 tex_color({ 0.4f, 0.4f, 0.6f, 1.f });


		ImGui::TextColored(time_color, "Start Time");
		ImGui::SameLine();
		ImGui::TextColored(path_color, "Path to follow");
		ImGui::SameLine();
		ImGui::TextColored(tex_color, "Texture");

		for (std::size_t i = 0; 
			 i < stage_data.enemy_start_times.size(); 
			 i++)
		{

			ImGui::NewLine();

			ImGui::TextColored(time_color, "%.2f", stage_data.enemy_start_times[i]);
			ImGui::SameLine();
			ImGui::TextColored(path_color, "%s", stage_data.enemy_paths[i].c_str());
			ImGui::SameLine();
			ImGui::TextColored(tex_color, "%s", stage_data.enemy_textures[i].c_str());
		}
	}
	ImGui::End();


	ImGui::ShowDemoWindow();

	if (open_dialog) ImGui::OpenPopup("Open Stage");
	if (save_dialog) ImGui::OpenPopup("Save Stage");

	if (file_dialog.showFileDialog("Open Stage", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".stage"))
	{
		std::string file_path = file_dialog.selected_path;
		stage_data = LoadStageFromFile(file_path);
	}
	if (file_dialog.showFileDialog("Save Stage", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".stage"))
	{

		std::string file_path = file_dialog.selected_path;

		nlohmann::json output;

		output["name"] = std::string(stage_name);
		output["length"] = stage_data.stage_length;


		output["enemy data"]["times"] = stage_data.enemy_start_times;
		output["enemy data"]["paths"] = stage_data.enemy_paths;
		output["enemy data"]["textures"] = stage_data.enemy_textures;

		SaveStringToFile(output.dump(), file_path + ".stage");
	}
}

void StageEditor::Render(ShapeRenderer& sh) 
{
	sh.SetColor({ 1.f, 1.f, 1.f, 1.f });
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

	if (current_texture != "")
	{
		s->Begin(sh.GetProjectionMatrix(), sh.GetViewMatrix());
		//the texture system manages allocation, this wont leak memory
		s->Draw(Texture::CreateTexture(current_texture), 0, 0, 2, 2);
		s->End();
	}
}


void StageEditor::OnMouseButtonDown() 
{

}

void StageEditor::OnMouseButtonUp() 
{

}

void StageEditor::Update(glm::vec2 mouse_world_pos) 
{

}