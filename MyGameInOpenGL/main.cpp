#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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

    // build and compile our shader program
	// ------------------------------------
	// vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // attach source code to shader object
    glCompileShader(vertexShader); // compile the vertex shader
    // check for shader compile errors
    int success;
    char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // check if compilation was successful
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
	unsigned int shaderProgram = glCreateProgram(); // returns ID reference to newly created program object
    glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader); 
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // bottom left  
         0.5f, 0.5f, 0.0f, // top right 
         -0.5f,  0.5f, 0.0f,  // top left  
         0.5f,  -0.5f, 0.0f  // bottom right
    };
    int indices[] = {
		0, 2, 3, // first triangle
		1, 3, 2 // second triangle
    };
    // create the vertex buffer object and vertex array object
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copies data from CPU to GPU

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // we're telling OpenGL: "ok we're using the element array buffer now"
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // Render loop (every iteration is called a frame)
    while (!glfwWindowShouldClose(window)) // checks if GLFW has been instructed to close
    {
        // input
		processInput(window); // checks for user input (e.g. keyboard input)

        // rendering commands
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 

        // check and call events and swap the buffers
		glfwSwapBuffers(window); // swaps the color buffer with the back buffer (solves issue of flickering and tearing that's present in a single buffer when it's being rendered)
        glfwPollEvents(); // checks if any events are triggered (e.g. keybaord input, mouse movement), calls corresponding functions (which we can register w/ callback functions)
    }

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
