#ifndef ENEMY_EDITOR_H
#define ENEMY_EDITOR_H

#include <Core.h>
#include <Editor/EditorApplication.h>

struct EnemyData
{
	float time_start = 0;
	
	std::string path_filepath;
	std::string texture_filepath;
};


class EnemyEditor : public Editor
{
private:
	EnemyData data;

public:
	EnemyEditor()
	{

	}

	void OnGUIRender() override
	{
		ImGui::Begin("Enemy Data");
		{

		}
		ImGui::End();
	}

	void Render(ShapeRenderer& sh) override
	{

	}

	void OnMouseButtonDown() override
	{

	}

	void OnMouseButtonUp() override
	{

	}

	void Update(glm::vec2 mouse_world_pos) override
	{

	}
};
	

#endif