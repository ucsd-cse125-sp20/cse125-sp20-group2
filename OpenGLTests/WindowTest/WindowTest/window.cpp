#include <glad/glad.h>		// Include this...
#include <GLFW/glfw3.h>		// ...before this.
#include <iostream>

// Func prototype definitions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// Initializing the window with the respective context (version 3.3, core profile)
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		

	// Creating an 800x600 window with the title "LearnOpenGL"
	GLFWwindow* window = glfwCreateWindow(1800, 1600, "LearnOpenGL", NULL, NULL);

	// Terminate program on window creation failure.
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Docs regarding this function:
	// https://www.glfw.org/docs/3.3/group__context.html#ga1c04dc242268f827290fe40aa1c91157
	glfwMakeContextCurrent(window);

	// Register our callback function to modifying rendering window dimensions on window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize GLAD, which will load our function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// The render loop
	// Keep rendering until the window should close, as instructed.
	while (!glfwWindowShouldClose(window))
	{
		// Process user input every loop (every frame).
		processInput(window);
		
		// // //
		// Rendering stuff

		// Choose color to clear the screen with (green)
		glClearColor(0.3f, 0.9f, 0.5f, 0.8f);

		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// // //
		// GLFW stuff

		// Swap the color buffer
		glfwSwapBuffers(window);

		// Check if any events are triggered (such as keyboard/mouse events)
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
	// Sets the size of the rendering window (NOT the same as the window itself- can be smaller)
	// Sets bottom-left corner to point (0, 0) and dimensions
	glViewport(0, 0, width, height);
}