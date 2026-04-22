#pragma once
#include "config.h"
#include "entity.h"
#include "cube.h"

class Camera;
class Shader;

// FIXME: fix build issues from refactor (problem with headers??)
class Player : public Entity {
	public:
		// TODO: replace cube with real player/ATV model later, cube is temporary
		Player(Cube& cubeModel);
		void update();
		void draw(Shader& shaderProgram, Camera& camera, float SCR_WIDTH, float SCR_HEIGHT);

		glm::vec3 Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
		float Yaw = 0.0f;
		glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);
	private:
		Cube* cubeModel;
};