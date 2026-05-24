#pragma once
#include "config.h"
#include "entity.h"
#include "rigidbody.h"
#include "model.h"

class Camera;
class Shader;

class Player : public Entity {
	public:
		Player(Model& model);
		void update();
		void draw(Shader& shaderProgram, Camera& camera, float SCR_WIDTH, float SCR_HEIGHT,
			glm::vec3* pos, glm::vec3 scale, float yaw);

		Rigidbody rb;

		bool isGrounded = true;

		float Yaw = 0.0f;
		glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);
	private:
		Model* model;
};