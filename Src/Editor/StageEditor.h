#ifndef ENEMY_EDITOR_H
#define ENEMY_EDITOR_H

#include <Core.h>
#include <Editor/EditorApplication.h>
#include <Graphics/SpriteBatch.h>
#include <imgui/ImGuiFileBrowser.h>

#include "EnemyCreator.h"

class StageEditor : public Editor
{
private:
	//needs to be a char* due to imgui
	static const std::size_t name_buffer_size = 80;
	char stage_name[name_buffer_size] = "Stage";
	
	int stage_length = 30;
	float current_stage_time = 0;

	std::unique_ptr<EnemyCreator> enemy_creator;


	/// Used for file dialog UI
	imgui_addons::ImGuiFileBrowser file_dialog;
	 
public:
	StageEditor();

	void OnGUIRender();

	void Render(ShapeRenderer& sh) override;

	void OnMouseButtonDown() override;

	void OnMouseButtonUp() override;

	void Update(glm::vec2 mouse_world_pos) override;
};
	

#endif