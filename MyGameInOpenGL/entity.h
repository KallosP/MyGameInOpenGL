#pragma once
#include "config.h"
#include "transform.h"
#include "model.h"

class Camera;

class Entity {
	public:
		Entity(const string& modelPath);

		void draw(Shader& shaderProgram, Camera& camera, float SCR_WIDTH, float SCR_HEIGHT,
			glm::vec3* pos, glm::vec3 scale, float yaw);

		Transform transform;

	protected:
		Model model;
};