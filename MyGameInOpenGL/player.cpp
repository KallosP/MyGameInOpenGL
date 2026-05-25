#include "player.h"
#include "camera.h"
#include "shader.h"

// Player constructor uses initializer list to initialize the model object of the Entity class (which Player inherits from)
Player::Player(const string& modelPath) : Entity(modelPath) {}

void Player::update() {
	// Calculate/update forward vector of player
	Forward.x = cos(glm::radians(Yaw));
	Forward.y = 0.0f;
	Forward.z = -sin(glm::radians(Yaw)); // -Z is forward in Right handed coord system
	Forward = glm::normalize(Forward);
}

void Player::draw(Shader& shaderProgram, Camera& camera, 
	float SCR_WIDTH, float SCR_HEIGHT, glm::vec3* pos, 
	glm::vec3 scale, float yaw)
{
	// model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, *pos);
	model = glm::rotate(model, glm::radians(yaw) + glm::radians(90.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, scale);
	shaderProgram.setMat4("model", model);
	// camera/view transformation
	glm::mat4 view;
	// transforms the model from world space to camera space, so it appears correctly
	// relative to the camera's position/orientation (which is stationary, the objects/world
	// move around the camera)
	view = camera.GetViewMatrix();
	shaderProgram.setMat4("view", view);
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 1000.0f);
	shaderProgram.setMat4("projection", projection);

	this->model.Draw(shaderProgram);
}
