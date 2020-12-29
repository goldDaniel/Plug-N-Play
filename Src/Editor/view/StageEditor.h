#ifndef STAGE_EDITOR_VIEW_H
#define STAGE_EDITOR_VIEW_H

#include <Core.h>
#include <Editor/EditorApplication.h>
#include <imgui/ImGuiFileBrowser.h>


#include <Editor/Logic/StageSimulation.h>


class StageEditor : public Editor
{
private:
	
	// Used for file dialog UI
	imgui_addons::ImGuiFileBrowser file_dialog;

	std::unique_ptr<StageSimulation> simulation;

public:
	StageEditor();

	void OnGUIRender() override;

	void Render(ShapeRenderer& sh) override;

	void OnMouseButtonDown() override;

	void OnMouseButtonUp() override;

	void Update(glm::vec2 mouse_world_pos) override;
};


#endif