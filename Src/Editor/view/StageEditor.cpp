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

}