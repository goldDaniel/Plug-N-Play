#ifndef APPLICATION_H
#define APPLICATION_H

#include <Core.h>
#include <fstream>

namespace
{
    struct StageData
    {
        int stage_length;
        float current_time;

        std::string stage_name;

        //these are parallel arrays
        std::vector<float> enemy_start_times;
        std::vector<float> enemy_speeds;
        std::vector<std::string> enemy_paths;
        std::vector<std::string> enemy_textures;
    };

    

    StageData LoadStageFromFile(const std::string& filepath)
    {
        std::string json_string;
        try
        {
            std::ifstream in_stream(filepath);

            json_string = std::string((std::istreambuf_iterator<char>(in_stream)),
                std::istreambuf_iterator<char>());

            in_stream.close();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::PATH::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            std::cout << filepath << std::endl;
        }

        auto output = nlohmann::json::parse(json_string);

        StageData result;
        result.stage_name = output["name"];
        result.stage_length = output["length"];
        

        for (std::size_t i = 0; 
             i < output["enemy data"]["times"].size(); 
             i++)
        {
            result.enemy_start_times.push_back(output["enemy data"]["times"][i]);
            result.enemy_speeds.push_back(output["enemy data"]["speeds"][i]);
            result.enemy_paths.push_back(output["enemy data"]["paths"][i]);
            result.enemy_textures.push_back(output["enemy data"]["textures"][i]);
        }

        return result;
    }


    Bezier::Bezier<3> LoadPathFromFile(const std::string& filepath)
    {
        std::string json_string;

        try
        {
            std::ifstream in_stream(filepath);

            json_string = std::string((std::istreambuf_iterator<char>(in_stream)),
                std::istreambuf_iterator<char>());

            in_stream.close();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::PATH::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            std::cout << filepath << std::endl;
        }

        auto output = nlohmann::json::parse(json_string);
        assert(output["path"].is_array());
        assert(output["path"].size() == 4);

        std::vector<Bezier::Point> control_points;

        for (std::size_t i = 0; i < output["path"].size(); i++)
        {
            Bezier::Point p;
            p.x = output["path"][i][0];
            p.y = output["path"][i][1];

            control_points.push_back(p);
        }

        return Bezier::Bezier<3>(control_points);
    }

    void SaveStringToFile(const std::string& data, const std::string& filepath)
    {
        std::ofstream out(filepath);
        out << data;
        out.close();
    }
}


class Application
{
protected:
    int window_width;
    int window_height;

    SDL_Window* window;
    SDL_GLContext context;

public:
    Application(SDL_Window* window, SDL_GLContext context, int window_width, int window_height)
        : window(window), context(context), window_width(window_width), window_height(window_height) {}

    virtual ~Application() {}

    virtual void Run() = 0;

    void Resize(int w, int h)
    {
        window_width = w;
        window_height = h;
    }
};

#endif