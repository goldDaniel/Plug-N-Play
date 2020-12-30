#include "StageEditor.h"


StageEditor::StageEditor()
{
	simulation = std::make_unique<StageSimulation>();
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
			simulation->Stop();
		}
		ImGui::SameLine();
		if (ImGui::Button("Save Stage"))
		{
			save_dialog = true;
			simulation->Pause();
		}
		ImGui::SameLine();
		if (ImGui::Button("Open Stage"))
		{
			open_dialog = true;
			simulation->Pause();
		}
		ImGui::Separator();

		ImGui::Text("Stage Info");

		if (ImGui::Button("Play"))
		{
			//run the enemy sim
			simulation->Start();
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			//pause the enemy sim
			simulation->Pause();
		}
		ImGui::SameLine();
		std::string running_text = simulation->IsRunning() ? "running " : "paused";
		ImGui::Text(running_text.c_str());

		ImGui::NewLine();

		float local_stage_time = simulation->GetStageTime();
		if (ImGui::SliderFloat("Current Stage Time", &local_stage_time, 0, simulation->GetStageLength(), 0, 1))
		{
			simulation->Pause();
		}
		simulation->SetStageTime(local_stage_time);

		ImGui::Separator();
	}

	ImGui::End();



	ImGui::ShowDemoWindow();

	if (open_dialog) ImGui::OpenPopup("Open Stage");
	if (save_dialog) ImGui::OpenPopup("Save Stage");

	if (file_dialog.showFileDialog("Open Stage", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".stage"))
	{
		std::string file_path = file_dialog.selected_path;
		simulation->LoadStage(file_path);
		
	}
	if (file_dialog.showFileDialog("Save Stage", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".stage"))
	{

		std::string file_path = file_dialog.selected_path;

		nlohmann::json output;

		output["name"] = "Stage";
		output["length"] = simulation->GetStageLength();


		const auto& stage_data = simulation->GetStageData();

		output["enemy data"]["times"] = stage_data.enemy_start_times;
		output["enemy data"]["paths"] = stage_data.enemy_paths;
		output["enemy data"]["textures"] = stage_data.enemy_textures;

		SaveStringToFile(output.dump(), file_path + ".stage");
	}
}

void StageEditor::Render(ShapeRenderer& sh)
{

}

void StageEditor::OnMouseButtonDown() 
{

}

void StageEditor::OnMouseButtonUp() 
{

}

void StageEditor::Update(glm::vec2 mouse_world_pos)
{
	simulation->Update();
}