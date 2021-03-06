#include <glad/glad.h> // Should be included before deps that require opengl
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learn_opengl/shader_s.h>

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float mixValue = 0.2f;

int main()
{
	glfwInit();

	// Setting to version 3.3 & use the core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Window Creation
	GLFWwindow* window = glfwCreateWindow(
		SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLAD manages function pointers, so call it before any openGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// Init shaders
	Shader ourShader("shaders/6_1_coordsys.vs", "shaders/6_1_coordsys.fs");

	// set up vertex data (and buffers) and config vertex attributes
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,	 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	 1.0f, 0.0f,
					   
		 0.5f,  0.5f,  0.5f,	 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	 0.0f, 1.0f

	};
	unsigned int indices[] = {
		0, 1, 2, // first triangle
		0, 2, 3  // second triangle
	};

	// VBO VAO EBO
	unsigned int VBO, VAO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// uncomment to draw in wireframe polygons
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// LOADING TEXTURES
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;

	unsigned char* data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
	
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	
	stbi_image_free(data);

	// TEXTURE 2
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// RENDER LOOP

	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	ourShader.setInt("texture2", 1);
	

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		

		float pulsing = sin(glfwGetTime());
		float pulsing2 = cos(glfwGetTime());
		float rising = glfwGetTime();

		// Rendering commands here

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);



		// Init identity matrices
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);


		model = glm::rotate(
			model,
			rising * glm::radians(50.0f),
			glm::vec3(0.5f, 1.0f, 0.0f)
		);
		view = glm::translate(
			view,
			glm::vec3(0.0f, 1.0f, -5.0f)
		);
		projection = glm::perspective(
			glm::radians(pulsing * 100.0f),
			(float)SCR_WIDTH / (float)SCR_HEIGHT,
			0.1f,
			100.0f
		);
		unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

		ourShader.setMat4("projection", projection);


		//glm::mat4 transform = glm::mat4(1.0f); // identity matrix
		//transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.5f, 0.6f, 1.0f));
		//transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.5f));

		ourShader.setFloat("mixValue", mixValue);
		//unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAO);

		for (unsigned int i = 0; i < 10; ++i) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pulsing * cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, rising * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//transform = glm::mat4(1.0f);
		//transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0f, 0.2f, 0.5f));
		//transform = glm::translate(transform, glm::vec3(0.0f, 0.5f, 0.0f));
		//float scaleAmount = sin(glfwGetTime());
		//transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, scaleAmount));

		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Check and call events and swap buffers
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.001f;
		if (mixValue >= 1.0f) mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.001f;
		if (mixValue <= 0.0f) mixValue = 0.0f;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
