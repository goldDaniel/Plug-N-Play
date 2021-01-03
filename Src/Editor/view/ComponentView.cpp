#include "ComponentView.h"

#include <Core.h>

#include <filesystem>
#include <Application.h>

template<>
void ComponentView::OnGUIRender<Transform>(Transform * component)
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

template<>
void ComponentView::OnGUIRender<BezierPath>(BezierPath * component)
{	
	ImGui::Text("BEZIER PATH");
	ImGui::Separator();

	float local_start_time = component->time_start;
	ImGui::InputFloat("Start Time", &local_start_time, 0.1, 1);
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

template<>
void ComponentView::OnGUIRender<Renderable>(Renderable * component)
{
	std::map<std::string, Texture*> texture_cache;

	//caching  
	auto path = std::filesystem::path("Assets/Textures");
	path.make_preferred();
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		const auto& path_str = file.path().string();

		Texture* t = Texture::CreateTexture(path_str);

		texture_cache.insert({ path_str, t });
	}

	ImGui::Text("RENDERABLE");
	ImGui::Separator();

	ImGui::Text("Texture: ");
	ImGui::SameLine();
	std::vector<std::string> paths;
	std::string current;
	for (const auto& pair : texture_cache)
	{
		paths.push_back(pair.first);
		if (pair.second == component->texture)
		{
			current = pair.first;
		}
	}
	if (ImGui::BeginCombo("", current.c_str()))
	{
		for (const auto& path_option : paths)
		{
			bool is_selected = (current == path_option);
			if (ImGui::Selectable(path_option.c_str(), is_selected))
			{
				current = path_option;
				component->texture = texture_cache[current];
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

	ImGui::ColorPicker4("Color", (float*)&component->color);
	
	ImGui::PopItemWidth();
	ImGui::Separator();
}