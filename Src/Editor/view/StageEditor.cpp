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


	ImGui::Begin("Stage Info", 0, ImGuiWindowFlags_NoMove |
								  ImGuiWindowFlags_NoCollapse |
								  ImGuiWindowFlags_NoResize);
	{
		auto& sim_data = simulation->GetStageData();

		ImVec2 outer_size = ImVec2(-FLT_MIN, 16 * 16);
		if (ImGui::BeginTable("##table1", 4, ImGuiTableFlags_ScrollX | 
											 ImGuiTableFlags_ScrollY | 
											 ImGuiTableFlags_RowBg | 
											 ImGuiTableFlags_BordersOuter | 
											 ImGuiTableFlags_BordersV | 
											 ImGuiTableFlags_Resizable | 
											 ImGuiTableFlags_Reorderable | 
										     ImGuiTableFlags_Hideable, outer_size))
		{
			ImGui::TableSetupColumn("ID");
			ImGui::TableSetupColumn("Start Time");
			ImGui::TableSetupColumn("Speed");
			ImGui::TableSetupColumn("Path");
			ImGui::TableHeadersRow();

			for (std::size_t i = 0; i < sim_data.stage_data.enemy_paths.size(); i++)
			{
				ImGui::TableNextRow();
				
				ImGui::PushID(i);
				{

					ImGui::PushID(i * 1000 + 0);
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("%i", sim_data.entity_map[i]);
					ImGui::PopID();

					ImGui::PushID(i * 1000 + 1);
					ImGui::TableSetColumnIndex(1);
					ImGui::InputFloat("", &sim_data.stage_data.enemy_start_times[i], 0.05f, 1.f);
					ImGui::PopID();

					ImGui::PushID(i * 1000 + 2);
					ImGui::TableSetColumnIndex(2);
					ImGui::InputFloat("", &sim_data.stage_data.enemy_speeds[i], 0.05f, 1.f);
					ImGui::PopID();

					ImGui::PushID(i * 1000 + 3);
					ImGui::TableSetColumnIndex(3);
					ImGui::Text("%s", sim_data.stage_data.enemy_paths[i].c_str());
					ImGui::PopID();
				}
				ImGui::PopID();
			}
			ImGui::EndTable();
		}
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


		const auto& sim_data = simulation->GetStageData();

		output["enemy data"]["times"] = sim_data.stage_data.enemy_start_times;
		output["enemy data"]["speeds"] = sim_data.stage_data.enemy_speeds;
		output["enemy data"]["paths"] = sim_data.stage_data.enemy_paths;
		output["enemy data"]["textures"] = sim_data.stage_data.enemy_textures;

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