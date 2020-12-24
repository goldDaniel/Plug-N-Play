#include "StageEditor.h"


StageEditor::StageEditor()
{
	enemy_creator = std::make_unique<EnemyCreator>();
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
		ImGui::InputInt("Stage Length", &stage_length, 1, 5);
		ImGui::SliderFloat("Current Stage Time", &current_stage_time, 0, stage_length, 0, 1);

		if (current_stage_time > stage_length) current_stage_time = stage_length;
		if (current_stage_time < 0) current_stage_time = 0;

		ImGui::Separator();

		enemy_creator->OnGUIRender();
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

		const auto& times = enemy_creator->GetStartTimes();
		const auto& enemy_paths = enemy_creator->GetPaths();
		const auto& enemy_textures = enemy_creator->GetTextures();
		for (std::size_t i = 0; i < times.size(); i++)
		{

			ImGui::NewLine();

			ImGui::TextColored(time_color, "%.2f", times[i]);
			ImGui::SameLine();
			ImGui::TextColored(path_color, "%s", enemy_paths[i].c_str());
			ImGui::SameLine();
			ImGui::TextColored(tex_color, "%s", enemy_textures[i].c_str());
		}
	}
	ImGui::End();


	ImGui::ShowDemoWindow();

	if (open_dialog) ImGui::OpenPopup("Open Stage");
	if (save_dialog) ImGui::OpenPopup("Save Stage");

	if (file_dialog.showFileDialog("Open Stage", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".stage"))
	{
		std::string file_path = file_dialog.selected_path;
		StageData stage_data = LoadStageFromFile(file_path);

		strcpy(stage_name, stage_data.stage_name.substr(0, name_buffer_size).c_str());
		
		enemy_creator->SetStartTimes(stage_data.enemy_start_times);
		enemy_creator->SetPaths(stage_data.enemy_paths);
		enemy_creator->SetTextures(stage_data.enemy_textures);
	}
	if (file_dialog.showFileDialog("Save Stage", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".stage"))
	{

		std::string file_path = file_dialog.selected_path;

		nlohmann::json output;

		output["name"] = std::string(stage_name);
		output["length"] = stage_length;


		output["enemy data"]["times"] = enemy_creator->GetStartTimes();
		output["enemy data"]["paths"] = enemy_creator->GetPaths();
		output["enemy data"]["textures"] = enemy_creator->GetTextures();

		SaveStringToFile(output.dump(), file_path + ".stage");
	}
}

void StageEditor::Render(ShapeRenderer& sh) 
{
	enemy_creator->Render(sh);
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