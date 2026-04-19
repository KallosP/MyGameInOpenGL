#ifndef CUBE_H
#define CUBE_H
#include "config.h"
#include "camera.h"
#include "material.h"
#include "cube_mesh.h"
#include "shader.h"

class Cube {
	public:
		CubeMesh* cubeMesh;
		Material* material;
		Material* mask;
		Cube(const char* textureMaterialSrc, const char* textureMaskSrc = NULL);
		void draw(Shader& shaderProgram, Camera& camera, float SCR_WIDTH, float SCR_HEIGHT, glm::vec3 pos, glm::vec3 scale);
	private: 
};

#endif
