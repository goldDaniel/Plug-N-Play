#include "ComponentView.h"

#include <Core.h>

#include <filesystem>
#include <Application.h>

void ComponentView::OnGUIRender(Transform * component)
{
	ImGui::Text("TRANSFORM");
	ImGui::Separator();

	float w = ImGui::GetWindowContentRegionWidth();
	ImGui::PushItemWidth(w / 3);

	ImGui::PushID(1);
	ImGui::Text("Position");
	ImGui::InputFloat("X", &component->position.x, 0.1, 1);
	ImGui::SameLine();
	ImGui::InputFloat("Y", &component->position.y, 0.1, 1);
	ImGui::PopID();

	ImGui::PushID(2);
	ImGui::Text("Scale");
	ImGui::InputFloat("X", &component->scale.x, 0.1, 1);
	ImGui::SameLine();
	ImGui::InputFloat("Y", &component->scale.y, 0.1, 1);
	ImGui::PopID();

	ImGui::PushID(3);
	ImGui::Text("Rotation");
	float rotation_local = glm::degrees(component->rotation);
	ImGui::InputFloat("", &rotation_local, 1, 30);
	component->rotation = glm::radians(rotation_local);
	ImGui::PopID();

	ImGui::PopItemWidth();
	ImGui::Separator();
}

void ComponentView::OnGUIRender(BezierPath * component)
{	
	ImGui::Text("BEZIER PATH");
	ImGui::Separator();

	float local_start_time = component->time_start;
	ImGui::InputFloat("Start Time", &local_start_time, 0.1, 1);
	if (local_start_time < 0) local_start_time = 0;
	component->time_start = local_start_time;

	float local_completion_speed = 1.f / component->speed;
	ImGui::InputFloat("Time To Complete (S)", &local_completion_speed, 0.1, 1);
	component->speed = 1.f / local_completion_speed;

	ImGui::Text("Control Points");
	auto control_points = component->curve.getControlPoints();
	for (std::size_t i = 0; i < control_points.size(); i++)
	{
		ImGui::Text("%u - X: %.2f, Y: %.2f", i, control_points[i].x, control_points[i].y);
	}

	ImGui::Separator();
}

void ComponentView::OnGUIRender(const std::map<Texture*, std::string>& texture_cache, Renderable * renderable)
{

	ImGui::Text("RENDERABLE");
	ImGui::Separator();

	ImGui::Text("Texture: ");
	ImGui::SameLine();
	std::string current = texture_cache.find(renderable->texture)->second;
	if (ImGui::BeginCombo("", current.c_str()))
	{
		for (const auto& pair : texture_cache)
		{
			bool is_selected = (current == pair.second);
			if (ImGui::Selectable(pair.second.c_str(), is_selected))
			{
				current = pair.second;
				renderable->texture = pair.first;
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	float w = ImGui::GetWindowContentRegionWidth();
	ImGui::PushItemWidth(w / 3);

	ImGui::ColorPicker4("Color", (float*)&renderable->color);
	
	ImGui::PopItemWidth();
	ImGui::Separator();
}