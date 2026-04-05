#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>


class CubeMesh {
	public:
		CubeMesh();
		void draw();
		~CubeMesh();
	private:
		unsigned int VBO, VAO, EBO;
};