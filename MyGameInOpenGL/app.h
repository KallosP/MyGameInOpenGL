#pragma once
#include "config.h"

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
