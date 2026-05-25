#include "entity.h"
#include "camera.h"

// Uses an initializer list to initialize the model object of this entity
Entity::Entity(const string& modelPath) : model(Model(modelPath)) {
}

void Entity::draw(Shader& shaderProgram, Camera& camera, 
	float SCR_WIDTH, float SCR_HEIGHT, glm::vec3* pos, 
	glm::vec3 scale, float yaw)
{
	// model matrix
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, *pos);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(yaw), glm::vec3(0, 1, 0));
	modelMatrix = glm::scale(modelMatrix, scale);
	shaderProgram.setMat4("model", modelMatrix);
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

	model.Draw(shaderProgram);
}