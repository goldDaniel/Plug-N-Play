#include "ComponentView.h"

#include <Core.h>

#include <filesystem>
#include <Application.h>

void ComponentView::OnGUIRender(Transform * component)
{
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

void ComponentView::OnGUIRender(const std::unordered_map<std::string, Bezier::Bezier<3>>& path_cache, BezierPath * component)
{	
	ImGui::Separator();

	std::string current;
	for (auto& pair : path_cache)
	{
		bool same_curve = true;
		const auto& points_0 = component->curve.getControlPoints();
		Bezier::Bezier<3> cpy(pair.second);
		const auto& points_1 = cpy.getControlPoints();

		//couldnt get operator overloading working cause im dumb
		//so we verify the data here
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
			current = pair.first;
		}
	}

	ImGui::PushID(12355);
	if (ImGui::BeginCombo("", current.c_str()))
	{
		for (const auto& pair : path_cache)
		{
			bool is_selected = (current == pair.first);
			if (ImGui::Selectable(pair.first.c_str(), is_selected))
			{
				current = pair.first;
				component->curve = pair.second;
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}


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
	ImGui::PopID();

	ImGui::Separator();
}

void ComponentView::OnGUIRender(const std::unordered_map<std::string, Texture*>& texture_cache, Renderable * renderable)
{
	ImGui::Separator();

	std::string current;
	for (auto& pair : texture_cache)
	{
		if (pair.second == renderable->texture)
		{
			current = pair.first;
		}
	}

	if (ImGui::BeginCombo("Texture", current.c_str()))
	{
		for (const auto& pair : texture_cache)
		{
			bool is_selected = (current == pair.first);
			if (ImGui::Selectable(pair.first.c_str(), is_selected))
			{
				current = pair.first;
				renderable->texture = pair.second;
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	
	ImGui::ColorPicker4("Color", (float*)&renderable->color);

	ImGui::Separator();
}