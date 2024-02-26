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
#include "TGModel.h"

class TGProgram
{
public:
	static void Section1();

	static void Section2();

	/// <summary>
	/// 绘制以深度表示的 Cube
	/// </summary>
	static void DrawCubeByDepthShader();

	/// <summary>
	/// 使用模板缓冲绘制描边
	/// </summary>
	static void StencilTestSection_CubeOutline();

	static void BlendTestSection_PlateLeaf();

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
	lightManager.AddPointLight(glm::vec3(4, 4, 4), glm::vec3(1, 1, 1), 1.0f, 0.09f, 0.032f);
	lightManager.AddPointLight(glm::vec3(-4, -4, -4), glm::vec3(1, 1, 1), 1.0f, 0.09f, 0.032f);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// Close Depth Test
	// glDepthFunc(GL_ALWAYS);

	// build and compile our shader zprogram
	// ------------------------------------
	TGVertexShader* baseVS = new TGVertexShader("Shaders/Light.vert", "MyVS");
	TGFragmentShader* basePS = new TGFragmentShader("Shaders/Light.frag", "MyPS");
	std::shared_ptr<TGShaderProgram> ourShader(new TGShaderProgram());
	ourShader->AddVertexShader(baseVS);
	ourShader->AddFragmentShader(basePS);
	ourShader->BindShaderProgram();

	TGVertexShader* groundVS = new TGVertexShader("Shaders/ground.vert", "MyVS2");
	TGFragmentShader* groundPS = new TGFragmentShader("Shaders/ground.frag", "MyPS2");
	std::shared_ptr<TGShaderProgram> groundShader(new TGShaderProgram());
	groundShader->AddVertexShader(groundVS);
	groundShader->AddFragmentShader(groundPS);
	groundShader->BindShaderProgram();

	TGVertexShader* depthTestVS = new TGVertexShader("Shaders/depthTest.vert", "MyVS3");
	TGFragmentShader* depthTestPS = new TGFragmentShader("Shaders/depthTest.frag", "MyPS3");
	std::shared_ptr<TGShaderProgram> depthShader(new TGShaderProgram());
	depthShader->AddVertexShader(depthTestVS);
	depthShader->AddFragmentShader(depthTestPS);
	depthShader->BindShaderProgram();

	ourShader = depthShader;

	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  2.0f,  0.0f),
		glm::vec3(2.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  2.0f)
	};

	std::shared_ptr<TGMeshGeometry> cubeGeometry = TGMeshFactory::Get().CreateCube2(2.0);
	cubeGeometry->AddTexture("Textures/wall.png", "texture_diffuse");
	cubeGeometry->AddTexture("Textures/tree.png", "texture_diffuse");

	std::shared_ptr<TGMeshGeometry> groundGeometry = TGMeshFactory::Get().CreateGrid(10.0);
	groundGeometry->AddTexture("Textures/Grass.png", "texture_diffuse");

	std::shared_ptr<TGModel> bagModel = std::make_shared<TGModel>("Meshes/leaf/scene.gltf");

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		// render boxes
		for (unsigned int i = 0; i < 4; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader->SetMatrix4x4("model", glm::value_ptr(model));
			cubeGeometry->DrawMesh(ourShader);
		}

		groundShader->UseProgram();
		groundShader->SetMatrix4x4("projection", glm::value_ptr(projection));
		groundShader->SetMatrix4x4("view", glm::value_ptr(view));
		groundShader->SetVector3("viewPos", player->GetPlayerCamera()->GetCameraPosition());
		lightManager.ApplyLightsToShaderPass(groundShader);
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(0, 0, -1));
		groundShader->SetMatrix4x4("model", glm::value_ptr(model2));
		groundGeometry->DrawMesh(groundShader);
		
		//model2 = glm::translate(model2, glm::vec3(0, 0, -2));
		//groundShader->SetMatrix4x4("model", glm::value_ptr(model2));
		//bagModel->Draw(groundShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}

void TGProgram::DrawCubeByDepthShader()
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
	lightManager.AddPointLight(glm::vec3(4, 4, 4), glm::vec3(1, 1, 1), 1.0f, 0.09f, 0.032f);
	lightManager.AddPointLight(glm::vec3(-4, -4, -4), glm::vec3(1, 1, 1), 1.0f, 0.09f, 0.032f);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// Close Depth Test
	// glDepthFunc(GL_ALWAYS);

	// build and compile our shader zprogram
	// ------------------------------------
	TGVertexShader* baseVS = new TGVertexShader("Shaders/depthTest.vert", "MyVS");
	TGFragmentShader* basePS = new TGFragmentShader("Shaders/depthTest.frag", "MyPS");
	std::shared_ptr<TGShaderProgram> ourShader(new TGShaderProgram());
	ourShader->AddVertexShader(baseVS);
	ourShader->AddFragmentShader(basePS);
	ourShader->BindShaderProgram();

	TGVertexShader* groundVS = new TGVertexShader("Shaders/ground.vert", "MyVS2");
	TGFragmentShader* groundPS = new TGFragmentShader("Shaders/ground.frag", "MyPS2");
	std::shared_ptr<TGShaderProgram> groundShader(new TGShaderProgram());
	groundShader->AddVertexShader(groundVS);
	groundShader->AddFragmentShader(groundPS);
	groundShader->BindShaderProgram();

	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  2.0f,  0.0f),
		glm::vec3(2.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  2.0f)
	};

	std::shared_ptr<TGMeshGeometry> cubeGeometry = TGMeshFactory::Get().CreateCube2(2.0);
	cubeGeometry->AddTexture("Textures/wall.png", "texture_diffuse");
	cubeGeometry->AddTexture("Textures/tree.png", "texture_diffuse");

	std::shared_ptr<TGMeshGeometry> groundGeometry = TGMeshFactory::Get().CreateGrid(10.0);
	groundGeometry->AddTexture("Textures/Grass.png", "texture_diffuse");

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		// render boxes
		for (unsigned int i = 0; i < 4; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader->SetMatrix4x4("model", glm::value_ptr(model));
			cubeGeometry->DrawMesh(ourShader);
		}

		groundShader->UseProgram();
		groundShader->SetMatrix4x4("projection", glm::value_ptr(projection));
		groundShader->SetMatrix4x4("view", glm::value_ptr(view));
		groundShader->SetVector3("viewPos", player->GetPlayerCamera()->GetCameraPosition());
		lightManager.ApplyLightsToShaderPass(groundShader);
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(0, 0, -1));
		groundShader->SetMatrix4x4("model", glm::value_ptr(model2));
		groundGeometry->DrawMesh(groundShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}

void TGProgram::StencilTestSection_CubeOutline()
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
	lightManager.AddPointLight(glm::vec3(4, 4, 4), glm::vec3(1, 1, 1), 1.0f, 0.09f, 0.032f);
	lightManager.AddPointLight(glm::vec3(-4, -4, -4), glm::vec3(1, 1, 1), 1.0f, 0.09f, 0.032f);

	// 开启深度测试
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// 开始模板测试
	glEnable(GL_STENCIL_TEST);
	// param1: 模板测试失败怎么办
	// param2: 模板测试通过但深度测试失败怎么办
	// param3: 模板和深度测试都通过怎么办
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// build and compile our shader zprogram
	// ------------------------------------
	TGVertexShader* baseVS = new TGVertexShader("Shaders/Light.vert", "MyVS");
	TGFragmentShader* basePS = new TGFragmentShader("Shaders/Light.frag", "MyPS");
	std::shared_ptr<TGShaderProgram> ourShader(new TGShaderProgram());
	ourShader->AddVertexShader(baseVS);
	ourShader->AddFragmentShader(basePS);
	ourShader->BindShaderProgram();

	TGVertexShader* outlineVS = new TGVertexShader("Shaders/stencilOutline.vert", "MyVS2");
	TGFragmentShader* outlinePS = new TGFragmentShader("Shaders/stencilOutline.frag", "MyPS2");
	std::shared_ptr<TGShaderProgram> outlineShader(new TGShaderProgram());
	outlineShader->AddVertexShader(outlineVS);
	outlineShader->AddFragmentShader(outlinePS);
	outlineShader->BindShaderProgram();

	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  2.0f,  0.0f),
		glm::vec3(2.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  2.0f)
	};

	std::shared_ptr<TGMeshGeometry> cubeGeometry = TGMeshFactory::Get().CreateCube2(2.0);
	cubeGeometry->AddTexture("Textures/wall.png", "texture_diffuse");
	cubeGeometry->AddTexture("Textures/tree.png", "texture_diffuse");

	std::shared_ptr<TGMeshGeometry> bigCubeGeometry = TGMeshFactory::Get().CreateCube2(2.2);
	bigCubeGeometry->AddTexture("Textures/wall.png", "texture_diffuse");
	bigCubeGeometry->AddTexture("Textures/tree.png", "texture_diffuse");

	std::shared_ptr<TGMeshGeometry> groundGeometry = TGMeshFactory::Get().CreateGrid(10.0);
	groundGeometry->AddTexture("Textures/Grass.png", "texture_diffuse");

	std::shared_ptr<TGModel> bagModel = std::make_shared<TGModel>("Meshes/leaf/scene.gltf");

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// 当模板测试通过，将模板值设置为 1
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
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

		// render boxes
		for (unsigned int i = 0; i < 4; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader->SetMatrix4x4("model", glm::value_ptr(model));
			cubeGeometry->DrawMesh(ourShader);
		}

		glStencilMask(0x00);
		ourShader->SetMatrix4x4("projection", glm::value_ptr(projection));
		ourShader->SetMatrix4x4("view", glm::value_ptr(view));
		ourShader->SetVector3("viewPos", player->GetPlayerCamera()->GetCameraPosition());
		lightManager.ApplyLightsToShaderPass(ourShader);
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(0, 0, -1));
		ourShader->SetMatrix4x4("model", glm::value_ptr(model2));
		groundGeometry->DrawMesh(ourShader);


		// activate shader
		outlineShader->UseProgram();
		outlineShader->SetMatrix4x4("projection", glm::value_ptr(projection));
		outlineShader->SetMatrix4x4("view", glm::value_ptr(view));

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		// render boxes
		for (unsigned int i = 0; i < 4; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			outlineShader->SetMatrix4x4("model", glm::value_ptr(model));
			bigCubeGeometry->DrawMesh(outlineShader);
		}
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}

void TGProgram::BlendTestSection_PlateLeaf()
{
	
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
