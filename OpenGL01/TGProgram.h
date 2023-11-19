#pragma once
#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "TGShader.h"
#include "TGShaderProgram.h"
#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/gtc/matrix_transform.hpp"
#include "ThirdParty/glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include "TGCamera.h"
#include "TGLocalPlayer.h"
#include "TGMeshGeometry.h"
#include "TGMeshFactory.h"
#include "ITGBaseLight.h"

class TGProgram
{
public:
	static void Section1();
	static void Section2();

	static std::unique_ptr<TGPointLight[]> AddPointLights();
	static std::unique_ptr<TGDirectionLight[]> AddDirectionLight();
	static std::unique_ptr<TGSpotLight[]> AddSpotLight();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void processInput(GLFWwindow* window);

	// settings
	const static unsigned int SCR_WIDTH = 1920;
	const static unsigned int SCR_HEIGHT = 1080;

	static bool firstMouse;
	static float lastX;
	static float lastY;
	static float fov;

	// timing
	static float deltaTime;	// time between current frame and last frame
	static float lastFrame;

	static TGLocalPlayer* player;
};

bool TGProgram::firstMouse = true;
float TGProgram::lastX = 800.0f / 2.0;
float TGProgram::lastY = 600.0 / 2.0;
float TGProgram::fov = 90.0f;
float TGProgram::deltaTime = 0.0f;
float TGProgram::lastFrame = 0.0f;
TGLocalPlayer* TGProgram::player = new TGLocalPlayer("PlayerMain");

void TGProgram::Section1()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
	{
		return;
	}

	TGCameraViewInfo playerCameraViewInfo;
	playerCameraViewInfo.mAspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	playerCameraViewInfo.mFov = fov;
	playerCameraViewInfo.mNear = 0.001f;
	playerCameraViewInfo.mFar = 100.0f;
	player->GetPlayerCamera()->SetCameraViewInfo(playerCameraViewInfo);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	TGVertexShader* baseVS = new TGVertexShader("Shaders/VS.vert", "MyVS");
	TGFragmentShader* basePS = new TGFragmentShader("Shaders/PS.frag", "MyPS");

	TGShaderProgram* ourShader = new TGShaderProgram();
	ourShader->AddVertexShader(baseVS);
	ourShader->AddFragmentShader(basePS);
	ourShader->BindShaderProgram();

	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  2.0f,  0.0f),
		glm::vec3(2.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  2.0f) //,
		//glm::vec3(2.4f, -0.4f, -3.5f),
		//glm::vec3(-1.7f,  3.0f, -7.5f),
		//glm::vec3(1.3f, -2.0f, -2.5f),
		//glm::vec3(1.5f,  2.0f, -2.5f),
		//glm::vec3(1.5f,  0.2f, -1.5f),
		//glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//unsigned int VBO, VAO;
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	std::shared_ptr<TGMeshGeometry> cubeGeometry = TGMeshFactory::Get().CreateCube(2.0);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// texcoor0
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);
	// texcoor1
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);
	// texcoor2
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(13 * sizeof(float)));
	glEnableVertexAttribArray(5);

	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("Textures/wall.jpg", &width, &height, &nrChannels, 0);
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
	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("Textures/tree.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader->UseProgram();
	ourShader->SetInt("texture1", 0);
	ourShader->SetInt("texture2", 1);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// activate shader
		ourShader->UseProgram();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = player->GetPlayerCamera()->GetCameraProjectionMatrix();
		ourShader->SetMatrix4x4("projection", glm::value_ptr(projection));

		// camera/view transformation
		glm::mat4 view = player->GetPlayerCamera()->GetCameraViewMatrix();
		ourShader->SetMatrix4x4("view", glm::value_ptr(view));

		// render boxes
		// glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubePositions[i]);
			// float angle = 20.0f * i;
			// model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader->SetMatrix4x4("model", glm::value_ptr(model));

			cubeGeometry->DrawMesh();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}

void TGProgram::Section2()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
	{
		return;
	}

	TGCameraViewInfo playerCameraViewInfo;
	playerCameraViewInfo.mAspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	playerCameraViewInfo.mFov = fov;
	playerCameraViewInfo.mNear = 0.001f;
	playerCameraViewInfo.mFar = 100.0f;
	player->GetPlayerCamera()->SetCameraViewInfo(playerCameraViewInfo);

	TGLightManager lightManager;
	lightManager.AddPointLight(glm::vec3(10, 10, 10), glm::vec3(1, 1, 1), 1.0, 0.09, 0.032);
	lightManager.AddPointLight(glm::vec3(-10, -10, -10), glm::vec3(1, 1, 1), 1.0, 0.09, 0.032);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	TGVertexShader* baseVS = new TGVertexShader("Shaders/Light.vert", "MyVS");
	TGFragmentShader* basePS = new TGFragmentShader("Shaders/Light.frag", "MyPS");

	std::shared_ptr<TGShaderProgram> ourShader(new TGShaderProgram());
	ourShader->AddVertexShader(baseVS);
	ourShader->AddFragmentShader(basePS);
	ourShader->BindShaderProgram();

	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  2.0f,  0.0f),
		glm::vec3(2.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  2.0f) //,
		//glm::vec3(2.4f, -0.4f, -3.5f),
		//glm::vec3(-1.7f,  3.0f, -7.5f),
		//glm::vec3(1.3f, -2.0f, -2.5f),
		//glm::vec3(1.5f,  2.0f, -2.5f),
		//glm::vec3(1.5f,  0.2f, -1.5f),
		//glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	std::shared_ptr<TGMeshGeometry> cubeGeometry = TGMeshFactory::Get().CreateCube(2.0);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// texcoor0
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);
	// texcoor1
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);
	// texcoor2
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(13 * sizeof(float)));
	glEnableVertexAttribArray(5);

	// load and create a texture
	// -------------------------
	unsigned int texture1, texture2;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("Textures/wall.jpg", &width, &height, &nrChannels, 0);
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

	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("Textures/tree.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader->UseProgram();
	ourShader->SetInt("texture1", 0);
	ourShader->SetInt("texture2", 1);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// activate shader
		ourShader->UseProgram();

		lightManager.ApplyLightsToShaderPass(ourShader);

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = player->GetPlayerCamera()->GetCameraProjectionMatrix();
		ourShader->SetMatrix4x4("projection", glm::value_ptr(projection));

		// camera/view transformation
		glm::mat4 view = player->GetPlayerCamera()->GetCameraViewMatrix();
		ourShader->SetMatrix4x4("view", glm::value_ptr(view));

		ourShader->SetVector3("viewPos", player->GetPlayerCamera()->GetCameraPosition());
		ourShader->SetVector3("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		ourShader->SetVector3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		ourShader->SetVector3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		ourShader->SetFloat("material.shininess", 32.0f);

		// render boxes
		for (unsigned int i = 0; i < 4; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubePositions[i]);
			// float angle = 20.0f * i;
			// model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader->SetMatrix4x4("model", glm::value_ptr(model));

			cubeGeometry->DrawMesh();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}

std::unique_ptr<TGPointLight[]> TGProgram::AddPointLights()
{
	std::unique_ptr<TGPointLight[]> samplePointLights(new TGPointLight[2]);

	samplePointLights[0].mPosition = glm::vec3(3.0, 3.0, 3.0);
	samplePointLights[1].mPosition = glm::vec3(-3.0, -3.0, 0.0);

	return std::move(samplePointLights);
}

std::unique_ptr<TGDirectionLight[]> TGProgram::AddDirectionLight()
{
	std::unique_ptr<TGDirectionLight[]> sampleDirLights(new TGDirectionLight[1]);

	sampleDirLights[0].mDirection = glm::vec3(-1, -1, -1);

	return std::move(sampleDirLights);
}

std::unique_ptr<TGSpotLight[]> TGProgram::AddSpotLight()
{
	std::unique_ptr<TGSpotLight[]> sampleSpotLights(new TGSpotLight[1]);

	sampleSpotLights[0].mPosition = glm::vec3(3, 0, 0);
	sampleSpotLights[0].mDirection = glm::vec3(-1, 0, 0);

	return std::move(sampleSpotLights);
}

void TGProgram::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	// std::cout << "Mouse: x:" << xoffset << ", " << "y:" << yoffset << std::endl;

	player->ViewRight(xoffset);
	player->ViewUp(yoffset);
}

void TGProgram::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

void TGProgram::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void TGProgram::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		player->MoveForward(0.0005f);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		player->MoveForward(-0.0005f);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		player->MoveRight(0.0005f);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		player->MoveRight(-0.0005f);
	}
}