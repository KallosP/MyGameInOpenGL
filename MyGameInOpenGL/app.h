#pragma once
#include "config.h"
#include "camera.h"
#include "shader.h"
#include "material.h"
#include "cube.h"
#include "ground.h"
#include "terrain.h"
#include "fault_formation_terrain.h"
#include "midpoint_disp_terrain.h"
#include "texture_generator.h"

using namespace std;

class App {
	public:
		App();
		~App();
		void run();
	private:


		void processInput();
		void initGLFW();
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void processInput(GLFWwindow* window);
		GLFWwindow* window;
};
