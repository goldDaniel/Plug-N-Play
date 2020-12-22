#ifndef ENEMY_EDITOR_H
#define ENEMY_EDITOR_H

#include <Core.h>
#include <Editor/EditorApplication.h>
#include <Graphics/SpriteBatch.h>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

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

	std::unique_ptr<SpriteBatch> s;

	std::vector<Texture*> textures;

public:
	EnemyEditor()
	{
		s = SpriteBatch::CreateSpriteBatch();

		std::string path = "Assets/Textures";
		std::vector<std::string> files;
		for (const auto& file : std::experimental::filesystem::directory_iterator(path))
		{
			std::string filename(file.path().u8string());
			textures.push_back(Texture::CreateTexture(filename));
		}
	}

	void OnGUIRender() override
	{
		bool open_dialog = false;
		bool save_dialog = false;
		ImGui::Begin("Curve Editor", 0, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New Enemy"))
					{
						
					}
					if (ImGui::MenuItem("Save Enemy"))
					{
						save_dialog = true;
					}
					if (ImGui::MenuItem("Open Enemy"))
					{
						open_dialog = true;
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
		}
		ImGui::End();
	}

	void Render(ShapeRenderer& sh) override
	{
		s->Begin(sh.GetProjectionMatrix(), sh.GetViewMatrix());


		int x = -Constants::WORLD_WIDTH;
		int y = Constants::WORLD_HEIGHT;
		for(std::size_t i = 0; i < textures.size(); i++)
		{
			s->Draw(textures[i], x, y, 1, 1);

			x += 2;
			if (x > Constants::WORLD_WIDTH)
			{
				x = -Constants::WORLD_WIDTH;
				y += 2;
			}
		}

		s->End();
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