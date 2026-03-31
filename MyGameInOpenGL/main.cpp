#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Other includes
#include "shader.h"
#include "stb_image.h"
#include "config.h"
#include "material.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// Initialize GLFW and set the OpenGL version to 3.3
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Create the window object and set it as the main context of the current thread
    GLFWwindow* window = glfwCreateWindow(800, 600, "MyOpenGLGame", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

	// Initialize GLAD (manages function pointers for OpenGL, simplifying the calling of OpenGL functions to what you would expect)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	// Register the callback function to adjust the viewport when the window is resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader shaderProgram("shader.vs", "shader.fs");
  
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
    };
	// create the vertex buffer object, vertex array object, and element buffer object
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    // copies data (vertices) from CPU to GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // ALWAYS must bind a buffer before calling glBufferData (saying: "ok the VBO is now in focus, that is what we're currently operating on")

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // we're telling OpenGL: "ok we're using the element array buffer now"
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

	// define how the GPU should read the vertex data (per vertex attribute) and enable the vertex attributes
    // ------------------
	// position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // "turn on the vertex attribute at location 0" (i.e. the position attribute, we defined this in the vertex shader)
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1); // "turn on the vertex attribute at location 1" (i.e. color attribute)
    // texture position attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2); // "same...location 2" (i.e. texture position)

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // same here, we're cleaning up
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    // TODO: UNDERSTAND TEXTURES BETTER, understand the flow:
    //         why do we need setInt? -> assigns our data we decalred here in this program (e.g. material and mask) to the corresponding variables we declared on the GPU in the fragment shader
    //         why do we need to call use() (i.e. glActiveTexture/glBindTexture) for every texture in the render loop? -> because we have to continuously redraw those textures every frame. And the only way to draw a texture is if they are bound 
	//         what does it mean to bind a texture? -> means we're telling opengl that's the one we're currently working with
	Material* material = new Material("container.jpg");
	Material* mask = new Material("awesomeface.png");


    shaderProgram.use();
    // Assign the uniform variables declared on the GPU (in the GLSL fragment shader code) with
    // the corresponding data we've declared here on the CPU (material & mask)
	shaderProgram.setInt("material", 0); // telling GPU which texture slot to sample from (these are uniforms)
	shaderProgram.setInt("mask", 1); 

    // Render loop (every iteration is called a frame)
    while (!glfwWindowShouldClose(window)) // checks if GLFW has been instructed to close
    {
        // input
		processInput(window); // checks for user input (e.g. keyboard input)

        // rendering commands
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // activate the shader program
        shaderProgram.use();
		material->use(0);
        mask->use(1);

        glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        // draw our first triangle
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 

        // check and call events and swap the buffers
		glfwSwapBuffers(window); // swaps the color buffer with the back buffer (solves issue of flickering and tearing that's present in a single buffer when it's being rendered)
        glfwPollEvents(); // checks if any events are triggered (e.g. keybaord input, mouse movement), calls corresponding functions (which we can register w/ callback functions)
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

	// Clean up and exit
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    // Checks if ESC key is pressed, if yes, sets window to close
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
