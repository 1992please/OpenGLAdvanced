#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"

enum ShadingMode
{
	SHADING_MODE_WIREFRAME,
	SHADING_MODE_SHADED,
};
