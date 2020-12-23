#ifndef ENEMY_EDITOR_H
#define ENEMY_EDITOR_H

#include <Core.h>
#include <Editor/EditorApplication.h>
#include <Graphics/SpriteBatch.h>
#include <imgui/ImGuiFileBrowser.h>


class StageEditor : public Editor
{
private:
	//needs to be a char* due to imgui
	static const std::size_t name_buffer_size = 80;
	char stage_name[name_buffer_size] = "Stage";
	
	
	StageData stage_data = { 30,0, "Stage", {},{},{} };
	
	
	//stores the filepath of the currently selected texture
	std::string current_texture;
	//all the textures we can select for enemies
	std::vector<std::string> texture_filepaths;

	//stores the filepath of the currently selected path
	std::string current_path;
	//all the paths we can select
	std::map<std::string, Bezier::Bezier<3>> paths;
	

	std::unique_ptr<SpriteBatch> s;

	/// <summary>
	/// Used for file dialog UI
	/// </summary>
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