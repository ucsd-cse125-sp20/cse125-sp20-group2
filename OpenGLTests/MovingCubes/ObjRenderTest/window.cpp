
#include <glad/glad.h>		// Include this...
#include <GLFW/glfw3.h>		// ...before this.
#include <iostream>
#include <assimp/config.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// // //
	// Perspective setup

	// Width and height (TEMP)
	float winWidth = 1800;
	float winHeight = 1600;

	// // //
	// Setup

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		
	GLFWwindow* window = glfwCreateWindow((int)winWidth, (int)winHeight, "LearnOpenGL", NULL, NULL);

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

	// Enable depth testing
	// This will be used later to make sure we occlude textures when a
	// surface is obscured by another surface (i.e. in 3D objects).
	glEnable(GL_DEPTH_TEST);

	// // //
	// Geometry

	// Cube
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// Defining cube positions.
	glm::vec3 cubePositions[] = {
	  glm::vec3(0.0f,  0.0f,  0.0f),
	  glm::vec3(2.0f,  5.0f, -15.0f),
	  glm::vec3(-1.5f, -2.2f, -2.5f),
	  glm::vec3(-3.8f, -2.0f, -12.3f),
	  glm::vec3(2.4f, -0.4f, -3.5f),
	  glm::vec3(-1.7f,  3.0f, -7.5f),
	  glm::vec3(1.3f, -2.0f, -2.5f),
	  glm::vec3(1.5f,  2.0f, -2.5f),
	  glm::vec3(1.5f,  0.2f, -1.5f),
	  glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Defining vertex buffer, vertex array, and element buffer objects
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// // //
	// Textures

	// Texture definition and binding (1) - NOTE: Should be done prior to shader use.
	unsigned int texture1ID;
	glGenTextures(1, &texture1ID);
	glBindTexture(GL_TEXTURE_2D, texture1ID);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("../../../assets/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Texture definition and binding (2) - NOTE: Should be done prior to shader use.
	unsigned int texture2ID;
	glGenTextures(1, &texture2ID);
	glBindTexture(GL_TEXTURE_2D, texture2ID);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	data = stbi_load("../../../assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// // //
	// Shaders

	// Shader definition
	Shader* shader = new Shader("shaders/vert_shader.glsl", "shaders/frag_shader.glsl");
	shader->use();

	// Locations of our needed uniform variables (NOTE: shader needs to be used beforehand)
	//int modelLoc = glGetUniformLocation(shader->ID, "model");
	//int viewLoc = glGetUniformLocation(shader->ID, "view");
	//int projLoc = glGetUniformLocation(shader->ID, "projection");

	// Set texture units (aka texture location values) in the shader
	// (this sets uniform values, so shader still needs to remain active before this step)
	shader->setInt("texture1", 0);
	shader->setInt("texture2", 1);

	// Model matrices (10 objects) + initial translations
	glm::mat4 model[10];
	for (int j = 0; j < 10; j++) {
		model[j] = glm::mat4(1.0f);
		model[j] = glm::translate(model[j], cubePositions[j]);
	}

	// // //
	// The render loop
	while (!glfwWindowShouldClose(window))
	{

		// // //
		// Input
		processInput(window);
		
		// create transformations

		// Rotating camera around a target
		//const float radius = 10.0f;
		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;
		//glm::mat4 view;
		//view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

		// // //
		// View + perspective projection matrices

		// create transformations
		glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		// pass transformation matrices to the shader
		shader->setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		shader->setMat4("view", view);

		// // //
		// Textures

		// Texture bindings
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2ID);


		// // //
		// Rendering stuff

		// Make BG dark blue
		glClearColor(0.1f, 0.1f, 0.2f, 0.4f);
		
		// Clear color + depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Ignored, for the time being. See fragment_shader.glsl.
		shader->set4f("inColor", glm::vec4(0.0, 0.0, 1.0f, 1.0f));

		// Draw cubes
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			float angle = 1.0f * i;
			model[i] = glm::rotate(model[i], glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			shader->setMat4("model", model[i]);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// // //
		// GLFW stuff
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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