#include <glad/glad.h>		// Include this...
#include <GLFW/glfw3.h>		// ...before this.
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Super bootleg constant C-string
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char* fragmentShaderSource1 = "#version 330 core\n"	// Top is blue
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(0.0f, 0.0f, 0.5f, 1.0f);\n"
	"}\n\0";
const char* fragmentShaderSource2 = "#version 330 core\n"	// Front is green
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(0.0f, 0.5f, 0.0f, 1.0f);\n"		
	"}\n\0";
const char* fragmentShaderSource3 = "#version 330 core\n"	// Back is red
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);\n"
	"}\n\0";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		
	GLFWwindow* window = glfwCreateWindow(1800, 1600, "LearnOpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// // //
	// Shader definitions

	// Vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Checking if vertex shader compilation failed
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment shader
	unsigned int fragmentShader1;
	unsigned int fragmentShader2;
	unsigned int fragmentShader3;
	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShader3 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);	// Used for top face
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);	// Used for front face
	glShaderSource(fragmentShader3, 1, &fragmentShaderSource3, NULL);	// Used for back face
	glCompileShader(fragmentShader1);
	glCompileShader(fragmentShader2);
	glCompileShader(fragmentShader3);

	// Checking if fragment shader compilation failed
	glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glGetShaderiv(fragmentShader3, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create shader program
	unsigned int shaderProgram1;		// Used for top face
	unsigned int shaderProgram2;		// Used for front face
	unsigned int shaderProgram3;		// Used for back face
	shaderProgram1 = glCreateProgram();
	shaderProgram2 = glCreateProgram();
	shaderProgram3 = glCreateProgram();

	// link shaders to shader programs
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram3, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader1);
	glAttachShader(shaderProgram2, fragmentShader2);
	glAttachShader(shaderProgram3, fragmentShader3);
	glLinkProgram(shaderProgram1);
	glLinkProgram(shaderProgram2);
	glLinkProgram(shaderProgram3);


	// check for linking errors
	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// We can delete these objects after linking them to the program.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader1);
	glDeleteShader(fragmentShader2);
	glDeleteShader(fragmentShader3);

	// // //
	// Defining geometry

	// Defining vertices
	/*float vertices[] = {
		-0.5f, 0.75f, 0.0f,
		0.75f, 0.75f, 0.0f,
		-0.75f,  0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.75f, -0.75f, 0.0f,
		0.5f, -0.75f, 0.0f,
		0.75f, -0.5f, 0.0f
	};*/

	float topVertices[] = {
		-0.5f, 0.75f, 0.0f,
		0.75f, 0.75f, 0.0f,
		-0.75f,  0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	float frontVertices[] = {
		-0.75f,  0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.75f, -0.75f, 0.0f,
		0.5f, -0.75f, 0.0f
	};

	float backVertices[] = {
		0.5f, 0.5f, 0.0f,
		0.75f, 0.75f, 0.0f,
		0.5f, -0.75f, 0.0f,
		0.75f, -0.5f, 0.0f
	};

	/*unsigned int indices[] = {
		0, 1, 2,	// Top left tri
		1, 2, 3,	// Bot righ tri
		2, 3, 4,	// Left front tri
		3, 4, 5,	// Right front tri
		1, 3, 5,	// Left back tri
		1, 6, 5		// Right back tri
	};*/

	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Create a vertex buffer object to send the vertices to the graphics card
	unsigned int VBOs[3];			// Buffer ID
	glGenBuffers(3, VBOs);			

	// Create a vertex array object
	unsigned int VAOs[3];
	glGenVertexArrays(3, VAOs);		

	// Create an element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);			

	/*// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(topVertices), topVertices, GL_STATIC_DRAW);
	// 3. copy our indices array in a buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);*/

	// Top face
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(topVertices), topVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);										// Need to bind VAO to EBO each time.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// front face
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(frontVertices), frontVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);										// Need to bind VAO to EBO each time.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Back face
	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(backVertices), backVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);										// Need to bind VAO to EBO each time.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Ubinding (VBO + VAO, not EBO)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// The render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		// // //
		// Rendering stuff

		// Make BG dark blue
		glClearColor(0.1f, 0.1f, 0.2f, 0.4f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw top triangle
		glUseProgram(shaderProgram1);
		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glUseProgram(shaderProgram2);
		glBindVertexArray(VAOs[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glUseProgram(shaderProgram3);
		glBindVertexArray(VAOs[2]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// // //
		// GLFW stuff
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glDeleteVertexArrays(3, VAOs);
	glDeleteBuffers(3, VBOs);
	glDeleteBuffers(1, &EBO);

	// Call terminate to cleanup GLFW resources.
	glfwTerminate();
	return 0;
}

// Handle user input
void processInput(GLFWwindow* window)
{
	// Exit application.
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// A callback function to resize the rendering window when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}