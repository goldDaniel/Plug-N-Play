#ifndef CORE_H
#define CORE_H

#include <SDL2/SDL.h>

#ifdef __arm__
	#include <SDL2/SDL_opengles2.h>
	#define GL_VERSION_MAJOR 2
	#define GL_VERSION_MINOR 0
#else
	#include <glad/glad/glad.h>
	#define GL_VERSION_MAJOR 3
	#define GL_VERSION_MINOR 3
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <array>
#include <cassert>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstring>

#include "ECS/ECSController.h"
#include "Game/Components.h"

struct Constants
{
	static const int WORLD_WIDTH = 3;
	static const int WORLD_HEIGHT = 6;
};


#endif
