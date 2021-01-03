#ifndef COMPONENT_VIEW_H
#define COMPONENT_VIEW_H

#include <Core.h>

template<class T>
class ComponentView
{
protected:
	T * component;

public:

	ComponentView(T* comp) : component(comp) {}


	virtual ~ComponentView() {}

	virtual void OnGUIRender() = 0;
};

class TransformView : public ComponentView<Transform>
{
public:
	
	TransformView(Transform* t) : ComponentView(t) {}

	void OnGUIRender() 
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
};

#endif