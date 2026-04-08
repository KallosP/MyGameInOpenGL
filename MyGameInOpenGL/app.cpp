#include "app.h"
#include "camera.h"
#include "shader.h"
#include "material.h"
#include "cube.h"
#include "ground.h"
#include "terrain.h"

// settings
unsigned int SCR_WIDTH = 1920; 
unsigned int SCR_HEIGHT = 1080;
// camera
Camera camera{glm::vec3(0.0f, 2.0f, 5.0f)}; // brace initialization, always treated as variable (fixes vexing parse issue)
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


App::App() {
	// Initialize GLFW and GLAD
    initGLFW();
}

void App::run() {
	Shader shaderProgram("shader.vs", "shader.fs");
	shaderProgram.use();
	// Assign the uniform variables declared on the GPU (in the GLSL fragment shader code) with
	// the corresponding data we've declared here on the CPU (material & mask)
	shaderProgram.setInt("material", 0); // telling GPU which texture slot to sample from (these are uniforms)
	shaderProgram.setInt("mask", 1); 

	// Initialize the terrain and load the heightmap from a file
	BaseTerrain terrain;
	float worldScale = 10.0f;
	terrain.InitTerrain(worldScale);
	terrain.LoadFromFile("heightmap.save");

	camera.RenderDistance = 5000.0f;

	// Create a cube
	Cube cube("source.gif", "container.jpg");
	Ground ground("grass.jpg");

	// Render loop (every iteration is called a frame)
	while (!glfwWindowShouldClose(window)) // checks if GLFW has been instructed to close
	{
		// input
		processInput(); // checks for user input (e.g. keyboard input)

		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwSetCursorPosCallback(window, mouse_callback);  

		// rendering commands
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the buffer of the depth info from the previous frame 

		// activate the shader program
		//shaderProgram.use(); 
		terrain.Render(camera, (float)SCR_WIDTH, (float)SCR_HEIGHT);

		//cube.draw(shaderProgram, camera, (float) SCR_WIDTH, (float) SCR_HEIGHT);
		//ground.draw(shaderProgram, camera, (float) SCR_WIDTH, (float) SCR_HEIGHT);

        // check and call events and swap the buffers
		glfwSwapBuffers(window); // swaps the color buffer with the back buffer (solves issue of flickering and tearing that's present in a single buffer when it's being rendered)
        glfwPollEvents(); // checks if any events are triggered (e.g. keybaord input, mouse movement), calls corresponding functions (which we can register w/ callback functions)
    }
}

App::~App() {
    glfwTerminate();
}

// Initialize GLFW and set the OpenGL version to 3.3
void App::initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the window object and set it as the main context of the current thread
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyOpenGLGame", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	//glfwSetWindowPos(window, 560, 240); // set window pos 
	glfwMakeContextCurrent(window);
	// Register the callback function to adjust the viewport when the window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLAD (manages function pointers for OpenGL, simplifying the calling of OpenGL functions to what you would expect)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void App::processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.MovementSpeed = 300.0f;
	else
		camera.MovementSpeed = camera.getDefaultSpeed();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void App::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void App::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void App::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
