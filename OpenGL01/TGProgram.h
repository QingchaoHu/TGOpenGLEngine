#pragma once
#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "TGShader.h"
#include "TGShaderProgram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include "TGCamera.h"
#include "TGLocalPlayer.h"
#include "TGMeshGeometry.h"
#include "TGMeshFactory.h"
#include "ITGBaseLight.h"
#include "TGModel.h"
#include "TGSkyBox.h"
#include "TGFrameBuffer.h"

class TGProgram
{
public:

	static void fitScene(const std::shared_ptr<TGMeshGeometry> *geometries, const int size);

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

	/// <summary>
	/// 基于混合绘制植物模型（叶片）
	/// </summary>
	static void BlendSection_PlateLeaf();

	/// <summary>
	/// 基于混合绘制多层透明玻璃
	/// </summary>
	static void BlendSection_TranslucentGlass();

	/// <summary>
	/// 通过绘制两个盒子达成一个通过透明层看到盒体内部的效果
	/// </summary>
	static void FaceCullSection_CubeInside();

	static void FrameBufferSection_PostProcess();

	static void MultiRenderTarget_DeferRenderPipeline();

	static void TextureCubeMapSection_Skybox();

	static void MultiRenderTarget_TexLightBuffer1000();

	static void InstanceTech_MillionNumberCube();

	static std::unique_ptr<TGPointLight[]> AddPointLights();
	static std::unique_ptr<TGDirectionLight[]> AddDirectionLight();
	static std::unique_ptr<TGSpotLight[]> AddSpotLight();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void processInput(GLFWwindow* window);

	// settings
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;

	static bool firstMouse;
	static float lastX;
	static float lastY;
	static float fov;

	static float cameraMoveSpeed;

	// timing
	static float deltaTime;	// time between current frame and last frame
	static float lastFrame;

	static TGLocalPlayer* player;
};

// settings
unsigned int TGProgram::SCR_WIDTH = 1920;
unsigned int TGProgram::SCR_HEIGHT = 1080;
bool TGProgram::firstMouse = true;
float TGProgram::lastX = 800.0f / 2.0;
float TGProgram::lastY = 600.0 / 2.0;
float TGProgram::fov = 90.0f;
float TGProgram::deltaTime = 0.0f;
float TGProgram::lastFrame = 0.0f;
float TGProgram::cameraMoveSpeed = 0.0005f;
TGLocalPlayer* TGProgram::player = new TGLocalPlayer("PlayerMain");

void TGProgram::fitScene(const std::shared_ptr<TGMeshGeometry> *geometries, const int size) {
	if (size == 0) return;

	Box bb;
	for (int i = 0; i < size; i ++) {
		auto geometry = geometries[i];
		geometry->computeBoundingBox();
		// std::cout << *geometry->mBoundingBox << std::endl;
		if (i == 0) bb.set(*geometry->mBoundingBox);
		else bb.expand(*geometry->mBoundingBox);
	}
	Sphere bs(bb);
	std::cout << bs << std::endl;
	glm::vec3 position = bs.center + glm::vec3(bs.radius, bs.radius, bs.radius);
	player->SetPlayerPosition(position);
	player->SetPlayerLookAt(bs.center);
}

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
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	// lightManager.AddPointLight(glm::vec3(4, 4, 4), glm::vec3(1, 1, 1), 1.0f, 0.09f, 0.032f);
	// lightManager.AddPointLight(glm::vec3(-4, -4, 4), glm::vec3(1, 1, 1), 1.0f, 0.09f, 0.032f);

	for (int i = -10; i < 10; i += 2)
	{
		for (int j = -10; j < 10; j += 2)
		{
			double randR = (rand() % 10000) / 10000.0;
			double randG = (rand() % 10000) / 10000.0;
			double randB = (rand() % 10000) / 10000.0;
			double randZ = (rand() % 10000) / 10000.0 * 10;
			lightManager.AddPointLight(glm::vec3(i * 10, j * 10, randZ), glm::vec3(randR, randG, randB), 0.1f, 0.09f, 0.032f);
		}
	}

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

	// world space positions of our cubes
	//glm::vec3 cubePositions[] = {
	//	glm::vec3(0.0f,  0.0f,  0.0f),
	//	glm::vec3(0.0f,  2.0f,  0.0f),
	//	glm::vec3(2.0f,  0.0f,  0.0f),
	//	glm::vec3(0.0f,  0.0f,  2.0f)
	//};

	std::vector<glm::vec3> cubePositions;
	for (int i = -100; i <= 100; i += 20)
	{
		for (int j = -100; j <= 100; j += 20)
		{
			double randZ = (rand() % 10000) / 10000.0 * 4;
			cubePositions.push_back(glm::vec3(i, j, randZ));
		}
	}

	std::shared_ptr<TGMeshGeometry> cubeGeometry = TGMeshFactory::Get().CreateCube2(2.0);
	std::shared_ptr<TGMaterial> cubeMaterial = std::make_shared<TGMaterial>();
	cubeMaterial->AddTexture("Textures/wall.png", ETGTextureUseType_Diffuse);
	cubeMaterial->AddTexture("Textures/tree.png", ETGTextureUseType_Diffuse);
	cubeGeometry->SetMaterial(cubeMaterial);

	std::shared_ptr<TGMeshGeometry> groundGeometry = TGMeshFactory::Get().CreateGrid(100.0, 10.0);
	std::shared_ptr<TGMaterial> groundMaterial = std::make_shared<TGMaterial>();
	int textureSlotIndex = groundMaterial->AddTexture("Textures/Grass.png", ETGTextureUseType_Diffuse);
	groundMaterial->GetTexture(0)->SetTextureAddressType(ETGTextureAddressType_Repeat, ETGTextureAddressType_Repeat);
	groundGeometry->SetMaterial(groundMaterial);

	std::shared_ptr<TGModel> bagModel = std::make_shared<TGModel>("Meshes/bag/Survival_BackPack_2.fbx");

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
		for (unsigned int i = 0; i < cubePositions.size(); i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader->SetMatrix4x4("model", glm::value_ptr(model));
			// cubeGeometry->DrawMesh(ourShader);
			bagModel->Draw(ourShader);
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
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
	{
		return;
	}
	const char* glver = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	std::cout << "gl version: " << glver;

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
	std::shared_ptr<TGMaterial> cubeMaterial = std::make_shared<TGMaterial>();
	cubeMaterial->AddTexture("Textures/wall.png", ETGTextureUseType_Diffuse);
	cubeMaterial->AddTexture("Textures/tree.png", ETGTextureUseType_Diffuse);
	cubeGeometry->SetMaterial(cubeMaterial);

	std::shared_ptr<TGMeshGeometry> groundGeometry = TGMeshFactory::Get().CreateGrid(10.0);
	std::shared_ptr<TGMaterial> groundMaterial = std::make_shared<TGMaterial>();
	int textureSlotIndex = groundMaterial->AddTexture("Textures/Grass.png", ETGTextureUseType_Diffuse);
	groundMaterial->GetTexture(0)->SetTextureAddressType(ETGTextureAddressType_Repeat, ETGTextureAddressType_Repeat);
	groundGeometry->SetMaterial(groundMaterial);

	const std::shared_ptr<TGMeshGeometry> geometries[] = { cubeGeometry, groundGeometry };
	TGProgram::fitScene(geometries, 2); // todo: cubePositions

	float fpsTime = 0, frames = 0;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		fpsTime += deltaTime;
		frames ++;

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
		
		if (fpsTime > 1) {
			glfwSetWindowTitle(window, (std::to_string(int(frames / fpsTime)) + " FPS").c_str());
			fpsTime = 0;
			frames = 0;
		}
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
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	std::shared_ptr<TGMaterial> cubeMaterial = std::make_shared<TGMaterial>();
	cubeMaterial->AddTexture("Textures/wall.png", ETGTextureUseType_Diffuse);
	cubeMaterial->AddTexture("Textures/tree.png", ETGTextureUseType_Diffuse);
	cubeGeometry->SetMaterial(cubeMaterial);

	std::shared_ptr<TGMeshGeometry> bigCubeGeometry = TGMeshFactory::Get().CreateCube2(2.2);
	bigCubeGeometry->SetMaterial(cubeMaterial);

	std::shared_ptr<TGMeshGeometry> groundGeometry = TGMeshFactory::Get().CreateGrid(100.0, 10.0);
	std::shared_ptr<TGMaterial> groundMaterial = std::make_shared<TGMaterial>();
	int textureSlotIndex = groundMaterial->AddTexture("Textures/Grass.png", ETGTextureUseType_Diffuse);
	groundMaterial->GetTexture(0)->SetTextureAddressType(ETGTextureAddressType_Repeat, ETGTextureAddressType_Repeat);
	groundGeometry->SetMaterial(groundMaterial);

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

void TGProgram::BlendSection_PlateLeaf()
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
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	player->SetPlayerPosition(glm::vec3(10, 10, 0));
	player->SetPlayerLookAt(glm::vec3(0, 0, 0));

	TGLightManager lightManager;
	lightManager.AddPointLight(glm::vec3(4, 4, 4), glm::vec3(1, 0, 0), 0.1f, 0.09f, 0.032f);
	lightManager.AddPointLight(glm::vec3(-4, -4, -4), glm::vec3(1, 1, 1), 0.1f, 0.09f, 0.032f);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// Close Depth Test
	// glDepthFunc(GL_ALWAYS);

	// build and compile our shader zprogram
	// ------------------------------------
	TGVertexShader* baseVS = new TGVertexShader("Shaders/ground.vert", "MyVS");
	TGFragmentShader* basePS = new TGFragmentShader("Shaders/ground.frag", "MyPS");
	std::shared_ptr<TGShaderProgram> ourShader(new TGShaderProgram());
	ourShader->AddVertexShader(baseVS);
	ourShader->AddFragmentShader(basePS);
	ourShader->BindShaderProgram();

	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  2.0f,  0.0f),
		glm::vec3(2.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  2.0f)
	};

	std::shared_ptr<TGMeshGeometry> groundGeometry = TGMeshFactory::Get().CreateGrid(100.0, 10.0);
	std::shared_ptr<TGMaterial> groundMaterial = std::make_shared<TGMaterial>();
	int textureSlotIndex = groundMaterial->AddTexture("Textures/Grass.png", ETGTextureUseType_Diffuse);
	groundMaterial->GetTexture(0)->SetTextureAddressType(ETGTextureAddressType_Repeat, ETGTextureAddressType_Repeat);
	groundGeometry->SetMaterial(groundMaterial);

	std::shared_ptr<TGModel> grassModel = std::make_shared<TGModel>("Meshes/leaf/scene.gltf");

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

		ourShader->UseProgram();
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(0, 0, 0));
		ourShader->SetMatrix4x4("model", glm::value_ptr(model2));
		groundGeometry->DrawMesh(ourShader);

		model2 = glm::translate(model2, glm::vec3(0, 0, 1));
		model2 = glm::rotate(model2, glm::radians(180.0f), glm::vec3(1, 0, 0));
		ourShader->SetMatrix4x4("model", glm::value_ptr(model2));
		grassModel->Draw(ourShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}

void TGProgram::BlendSection_TranslucentGlass()
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
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	player->SetPlayerPosition(glm::vec3(10, 0, 10));
	player->SetPlayerLookAt(glm::vec3(0, 0, 0));

	TGLightManager lightManager;
	lightManager.AddPointLight(glm::vec3(4, 4, 4), glm::vec3(1, 1, 1), 0.1f, 0.09f, 0.032f);
	lightManager.AddPointLight(glm::vec3(-4, -4, -4), glm::vec3(1, 1, 1), 0.1f, 0.09f, 0.032f);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// Close Depth Test
	// glDepthFunc(GL_ALWAYS);

	// build and compile our shader zprogram
	// ------------------------------------
	TGVertexShader* baseVS = new TGVertexShader("Shaders/ground.vert", "MyVS");
	TGFragmentShader* basePS = new TGFragmentShader("Shaders/ground.frag", "MyPS");
	std::shared_ptr<TGShaderProgram> ourShader(new TGShaderProgram());
	ourShader->AddVertexShader(baseVS);
	ourShader->AddFragmentShader(basePS);
	ourShader->BindShaderProgram();

	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, -2.0f,  0.0f),
		glm::vec3(6.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  6.0f,  0.0f)
	};

	glm::vec3 windowPositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(3.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 2.0f, 0.0f)
	};

	std::shared_ptr<TGMeshGeometry> groundGeometry = TGMeshFactory::Get().CreateGrid(100.0, 10.0);
	std::shared_ptr<TGMaterial> groundMaterial = std::make_shared<TGMaterial>();
	int textureSlotIndex = groundMaterial->AddTexture("Textures/Grass.png", ETGTextureUseType_Diffuse);
	groundMaterial->GetTexture(0)->SetTextureAddressType(ETGTextureAddressType_Repeat, ETGTextureAddressType_Repeat);
	groundGeometry->SetMaterial(groundMaterial);

	std::shared_ptr<TGMeshGeometry> windowGeometry = TGMeshFactory::Get().CreateGrid(2.0);
	std::shared_ptr<TGMaterial> windowMaterial = std::make_shared<TGMaterial>();
	windowMaterial->AddTexture("Textures/RedWindow.png", ETGTextureUseType_Diffuse);
	windowGeometry->SetMaterial(windowMaterial);

	std::shared_ptr<TGMeshGeometry> cubeGeometry = TGMeshFactory::Get().CreateCube2(2.0);
	std::shared_ptr<TGMaterial> cubeMaterial = std::make_shared<TGMaterial>();
	cubeMaterial->AddTexture("Textures/wall.png", ETGTextureUseType_Diffuse);
	cubeGeometry->SetMaterial(cubeMaterial);

	// 开启混合
	glEnable(GL_BLEND);
	// 混合的前后颜色参数
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 混合的前后颜色参数，RGB 和 A 分开设置
	// glBlendFuncSeparate();

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

		ourShader->UseProgram();
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(0, 0, -1));
		ourShader->SetMatrix4x4("model", glm::value_ptr(model2));
		groundGeometry->DrawMesh(ourShader);

		// 先画不透明的物体
		for (int i = 0; i < 3; i++)
		{
			glm::mat4 cubeModelTransform = glm::mat4(1.0);
			cubeModelTransform = glm::translate(cubeModelTransform, cubePositions[i]);
			ourShader->SetMatrix4x4("model", glm::value_ptr(cubeModelTransform));
			cubeGeometry->DrawMesh(ourShader);
		}

		// 对透明的物体要有先后绘制顺序，做透明排序
		std::vector<std::tuple<int, double>> toCameraDistanceArray;
		glm::vec3 cameraPosition = player->GetPlayerCamera()->GetCameraPosition();
		for (int i = 0; i < 4; i++)
		{
			double toCameraDistance = glm::length(cameraPosition - windowPositions[i]);
			toCameraDistanceArray.push_back(std::tuple<int, double>(i, toCameraDistance));
		}

		std::sort(toCameraDistanceArray.begin(), toCameraDistanceArray.end(), [](std::tuple<int, double>& item1, std::tuple<int, double>& item2)
			{
				return std::get<1>(item1) > std::get<1>(item2);
			});

		for (int i = 0; i < 4; i++)
		{
			glm::mat4 windowModelTransform = glm::mat4(1.0);
			windowModelTransform = glm::translate(windowModelTransform, windowPositions[std::get<0>(toCameraDistanceArray[i])]);
			windowModelTransform = glm::rotate(windowModelTransform, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
			ourShader->SetMatrix4x4("model", glm::value_ptr(windowModelTransform));
			windowGeometry->DrawMesh(ourShader);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}

void TGProgram::FaceCullSection_CubeInside()
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
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	player->SetPlayerPosition(glm::vec3(10, 0, 10));
	player->SetPlayerLookAt(glm::vec3(0, 0, 0));

	TGLightManager lightManager;
	lightManager.AddPointLight(glm::vec3(4, 4, 4), glm::vec3(1, 0, 0), 0.1f, 0.09f, 0.032f);
	lightManager.AddPointLight(glm::vec3(-4, -4, -4), glm::vec3(0, 1, 1), 0.1f, 0.09f, 0.032f);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// Close Depth Test
	// glDepthFunc(GL_ALWAYS);

	// build and compile our shader zprogram
	// ------------------------------------
	TGVertexShader* baseVS = new TGVertexShader("Shaders/ground.vert", "MyVS");
	TGFragmentShader* basePS = new TGFragmentShader("Shaders/ground.frag", "MyPS");
	std::shared_ptr<TGShaderProgram> ourShader(new TGShaderProgram());
	ourShader->AddVertexShader(baseVS);
	ourShader->AddFragmentShader(basePS);
	ourShader->BindShaderProgram();

	std::shared_ptr<TGMeshGeometry> groundGeometry = TGMeshFactory::Get().CreateGrid(100.0, 10.0);
	std::shared_ptr<TGMaterial> groundMaterial = std::make_shared<TGMaterial>();
	int textureSlotIndex = groundMaterial->AddTexture("Textures/Grass.png", ETGTextureUseType_Diffuse);
	groundMaterial->GetTexture(0)->SetTextureAddressType(ETGTextureAddressType_Repeat, ETGTextureAddressType_Repeat);
	groundGeometry->SetMaterial(groundMaterial);

	std::shared_ptr<TGMeshGeometry> cubeGeometry = TGMeshFactory::Get().CreateCube2(6.0);
	std::shared_ptr<TGMaterial> cubeMaterial = std::make_shared<TGMaterial>();
	cubeMaterial->AddTexture("Textures/wall.png", ETGTextureUseType_Diffuse);
	cubeGeometry->SetMaterial(cubeMaterial);

	std::shared_ptr<TGMeshGeometry> cubeGeometry2 = TGMeshFactory::Get().CreateCube2(6.0);
	std::shared_ptr<TGMaterial> cubeMaterial2 = std::make_shared<TGMaterial>();
	cubeMaterial2->AddTexture("Textures/GrayGlass.png", ETGTextureUseType_Diffuse);
	cubeGeometry2->SetMaterial(cubeMaterial2);

	std::shared_ptr<TGModel> plantModel = std::make_shared<TGModel>("Meshes/leaf/scene.gltf");

	// 先画地面，再用剔除正面绘制一个 Cube
	// 再绘制植物，再绘制一个透明的正面盒子

	// 开启混合
	glEnable(GL_BLEND);
	// 混合的前后颜色参数
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 混合的前后颜色参数，RGB 和 A 分开设置
	// glBlendFuncSeparate();

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

		ourShader->UseProgram();
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(0, 0, -1));
		ourShader->SetMatrix4x4("model", glm::value_ptr(model2));
		groundGeometry->DrawMesh(ourShader);

		// 开启面剔除
		glEnable(GL_CULL_FACE);
		// 剔除正面 Or 背面
		glCullFace(GL_FRONT);
		// 正面是顺时针 Or 逆时针
		glFrontFace(GL_CW);
		glm::mat4 cubeModelTransform = glm::mat4(1.0);
		cubeModelTransform = glm::translate(cubeModelTransform, glm::vec3(0, 0, 2.1));
		ourShader->SetMatrix4x4("model", glm::value_ptr(cubeModelTransform));
		cubeGeometry->DrawMesh(ourShader);

		glDisable(GL_CULL_FACE);
		glm::mat4 plantModelTransform = glm::mat4(1.0);
		plantModelTransform = glm::translate(plantModelTransform, glm::vec3(0, 0, 2.5));
		plantModelTransform = glm::rotate(plantModelTransform, glm::radians(180.0f), glm::vec3(1, 0, 0));
		plantModelTransform = glm::scale(plantModelTransform, glm::vec3(0.3f, 0.3f, 0.3f));
		ourShader->SetMatrix4x4("model", glm::value_ptr(plantModelTransform));
		plantModel->Draw(ourShader);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
		glm::mat4 cube2ModelTransform = glm::mat4(1.0);
		cube2ModelTransform = glm::translate(cube2ModelTransform, glm::vec3(0, 0, 2.1));
		ourShader->SetMatrix4x4("model", glm::value_ptr(cube2ModelTransform));
		cubeGeometry2->DrawMesh(ourShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}

void TGProgram::FrameBufferSection_PostProcess()
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
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	player->SetPlayerPosition(glm::vec3(10, 0, 10));
	player->SetPlayerLookAt(glm::vec3(0, 0, 0));

	TGLightManager lightManager;
	lightManager.AddPointLight(glm::vec3(4, 4, 4), glm::vec3(1, 0, 0), 0.1f, 0.09f, 0.032f);
	lightManager.AddPointLight(glm::vec3(-4, -4, -4), glm::vec3(0, 1, 1), 0.1f, 0.09f, 0.032f);

	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, -2.0f,  2.1f),
		glm::vec3(6.0f,  0.0f,  2.1f),
		glm::vec3(0.0f,  6.0f,  2.1f)
	};

	std::shared_ptr<TGMeshGeometry> groundGeometry = TGMeshFactory::Get().CreateGrid(100.0, 10.0);
	std::shared_ptr<TGMaterial> groundMaterial = std::make_shared<TGMaterial>();
	int textureSlotIndex = groundMaterial->AddTexture("Textures/Grass.png", ETGTextureUseType_Diffuse);
	groundMaterial->GetTexture(textureSlotIndex)->SetTextureAddressType(ETGTextureAddressType_Repeat, ETGTextureAddressType_Repeat);
	groundGeometry->SetMaterial(groundMaterial);

	std::shared_ptr<TGMeshGeometry> cubeGeometry = TGMeshFactory::Get().CreateCube2(2.0);
	std::shared_ptr<TGMaterial> cubeMaterial = std::make_shared<TGMaterial>();
	textureSlotIndex = cubeMaterial->AddTexture("Textures/wall.png", ETGTextureUseType_Diffuse);
	cubeMaterial->GetTexture(textureSlotIndex)->SetTextureAddressType(ETGTextureAddressType_Repeat, ETGTextureAddressType_Repeat);
	cubeGeometry->SetMaterial(cubeMaterial);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// Close Depth Test
	// glDepthFunc(GL_ALWAYS);

	// build and compile our shader zprogram
	// ------------------------------------
	TGVertexShader* baseVS = new TGVertexShader("Shaders/ground.vert", "MyVS");
	TGFragmentShader* basePS = new TGFragmentShader("Shaders/ground.frag", "MyPS");
	std::shared_ptr<TGShaderProgram> ourShader(new TGShaderProgram());
	ourShader->AddVertexShader(baseVS);
	ourShader->AddFragmentShader(basePS);
	ourShader->BindShaderProgram();

	TGVertexShader* screenVS = new TGVertexShader("Shaders/frameBuffer.vert", "SVS");
	TGFragmentShader* screenPS = new TGFragmentShader("Shaders/frameBuffer.frag", "SPS");
	std::shared_ptr<TGShaderProgram> screenShader(new TGShaderProgram());
	screenShader->AddVertexShader(screenVS);
	screenShader->AddFragmentShader(screenPS);
	screenShader->BindShaderProgram();

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// FrameBufferObject，帧缓冲
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	unsigned int frameBufferTexture;
	glGenTextures(1, &frameBufferTexture);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);

	//unsigned int frameDepthStencilBufferTexture;
	//glGenTextures(1, &frameDepthStencilBufferTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	//glBindTexture(GL_TEXTURE_2D, frameDepthStencilBufferTexture);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, frameDepthStencilBufferTexture, 0);

	// RenderBufferObject，渲染缓冲
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	// 解绑
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glBindFramebuffer(GL_FRAMEBUFFER, rbo);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader->UseProgram();
		lightManager.ApplyLightsToShaderPass(ourShader);
		glm::mat4 view = player->GetPlayerCamera()->GetCameraViewMatrix();
		ourShader->SetMatrix4x4("view", glm::value_ptr(view));
		glm::mat4 projection = player->GetPlayerCamera()->GetCameraProjectionMatrix();
		ourShader->SetMatrix4x4("projection", glm::value_ptr(projection));
		ourShader->SetVector3("viewPos", player->GetPlayerCamera()->GetCameraPosition());

		for (int i = 0; i < 3; i++)
		{
			glm::mat4 model = glm::mat4(1.0);
			model = glm::translate(model, cubePositions[i]);
			ourShader->SetMatrix4x4("model", glm::value_ptr(model));
			cubeGeometry->DrawMesh(ourShader);
		}

		glm::mat4 groundModel = glm::mat4(1.0);
		ourShader->SetMatrix4x4("model", glm::value_ptr(groundModel));
		groundGeometry->DrawMesh(ourShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		screenShader->UseProgram();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, frameBufferTexture);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}

void TGProgram::MultiRenderTarget_DeferRenderPipeline()
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
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
	{
		return;
	}

	TGCameraViewInfo playerCameraViewInfo;
	playerCameraViewInfo.mAspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	playerCameraViewInfo.mFov = fov;
	playerCameraViewInfo.mNear = 0.001f;
	playerCameraViewInfo.mFar = 1000.0f;
	player->GetPlayerCamera()->SetCameraViewInfo(playerCameraViewInfo);
	player->SetPlayerPosition(glm::vec3(10, 0, 10));
	player->SetPlayerLookAt(glm::vec3(0, 0, 0));

	TGLightManager lightManager;
	//lightManager.AddPointLight(glm::vec3(10, 0, 1), glm::vec3(1, 0, 0), 0.1f, 0.09f, 0.032f);
	//lightManager.AddPointLight(glm::vec3(-10, 0, 1), glm::vec3(0, 1, 0), 0.1f, 0.09f, 0.032f);
	//lightManager.AddPointLight(glm::vec3(0, 10, 1), glm::vec3(0, 0, 1), 0.1f, 0.09f, 0.032f);
	//lightManager.AddPointLight(glm::vec3(0, -10, 1), glm::vec3(1, 1, 1), 0.1f, 0.09f, 0.032f);

	//for (int i = 0; i < 150; i++)
	//{
	//	double randX = -20.0 + (rand() % 40000) / 1000.0;
	//	double randY = -20.0 + (rand() % 40000) / 1000.0;
	//	double randZ = -20.0 + (rand() % 40000) / 1000.0;
	//	randZ = 0;

	//	double randR = (rand() % 10000) / 10000.0;
	//	double randG = (rand() % 10000) / 10000.0;
	//	double randB = (rand() % 10000) / 10000.0;

	//	lightManager.AddPointLight(glm::vec3(randX, randY, randZ), glm::vec3(randR, randG, randB), 0.1f, 0.09f, 0.032f);
	//}

	for (int i = -10; i < 10; i += 2)
	{
		for (int j = -10; j < 10; j++)
		{
			double randR = (rand() % 10000) / 10000.0;
			double randG = (rand() % 10000) / 10000.0;
			double randB = (rand() % 10000) / 10000.0;
			double randZ = (rand() % 10000) / 10000.0 * 10;
			lightManager.AddPointLight(glm::vec3(i * 10, j * 10, randZ), glm::vec3(randR, randG, randB), 0.1f, 0.09f, 0.1f);
		}
	}

	// world space positions of our cubes
	//glm::vec3 cubePositions[] = {
	//	glm::vec3(0.0f, -2.0f,  2.0f),
	//	glm::vec3(6.0f, 0.0f,  2.0f),
	//	glm::vec3(0.0f, 6.0f,  2.0f),
	//	glm::vec3(20.0f, 0.0f,  2.0f),
	//	glm::vec3(50.0f, 0.0f,  2.0f),
	//	glm::vec3(-30.0f, 0.0f,  2.0f),
	//	glm::vec3(-60.0f, 0.0f,  2.0f),
	//	glm::vec3(0.0f, 60.0f,  2.0f),
	//	glm::vec3(0.0f, -45.0f,  2.0f),
	//	glm::vec3(0.0f, -20.0f,  2.0f),
	//};

	std::shared_ptr<TGModel> bagModel = std::make_shared<TGModel>("Meshes/bag/Survival_BackPack_2.fbx");

	std::vector<glm::vec3> cubePositions;
	for (int i = -100; i <= 100; i += 20)
	{
		for (int j = -100; j <= 100; j += 20)
		{
			double randZ = (rand() % 10000) / 10000.0 * 4;
			cubePositions.push_back(glm::vec3(i, j, randZ));
		}
	}

	std::shared_ptr<TGMeshGeometry> groundGeometry = TGMeshFactory::Get().CreateGrid(100.0, 10.0);
	std::shared_ptr<TGMaterial> groundMaterial = std::make_shared<TGMaterial>();
	int textureSlotIndex = groundMaterial->AddTexture("Textures/Grass.png", ETGTextureUseType_Diffuse);
	groundMaterial->GetTexture(0)->SetTextureAddressType(ETGTextureAddressType_Repeat, ETGTextureAddressType_Repeat);
	groundGeometry->SetMaterial(groundMaterial);

	std::shared_ptr<TGMeshGeometry> cubeGeometry = TGMeshFactory::Get().CreateCube2(2.0);
	std::shared_ptr<TGMaterial> cubeMaterial = std::make_shared<TGMaterial>();
	cubeMaterial->AddTexture("Textures/wall.png", ETGTextureUseType_Diffuse);
	cubeMaterial->AddTexture("Textures/wall_speca.png", ETGTextureUseType_Specular);
	cubeGeometry->SetMaterial(cubeMaterial);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// Close Depth Test
	// glDepthFunc(GL_ALWAYS);

	// build and compile our shader zprogram
	// ------------------------------------
	TGVertexShader* baseVS = new TGVertexShader("Shaders/gbufferMainPass.vert", "MyVS");
	TGFragmentShader* basePS = new TGFragmentShader("Shaders/gbufferMainPass.frag", "MyPS");
	std::shared_ptr<TGShaderProgram> ourShader(new TGShaderProgram());
	ourShader->AddVertexShader(baseVS);
	ourShader->AddFragmentShader(basePS);
	ourShader->BindShaderProgram();

	TGVertexShader* screenVS = new TGVertexShader("Shaders/gbufferLightPass.vert", "SVS");
	TGFragmentShader* screenPS = new TGFragmentShader("Shaders/gbufferLightPass.frag", "SPS");
	std::shared_ptr<TGShaderProgram> screenShader(new TGShaderProgram());
	screenShader->AddVertexShader(screenVS);
	screenShader->AddFragmentShader(screenPS);
	screenShader->BindShaderProgram();

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	std::shared_ptr<TGFrameBuffer> mainPassFrameBuffer = std::make_shared<TGFrameBuffer>(SCR_WIDTH, SCR_HEIGHT);
	int mYaw = 0;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		mainPassFrameBuffer->Use();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader->UseProgram();
		lightManager.ApplyLightsToShaderPass(ourShader);
		glm::mat4 view = player->GetPlayerCamera()->GetCameraViewMatrix();
		ourShader->SetMatrix4x4("view", glm::value_ptr(view));
		glm::mat4 projection = player->GetPlayerCamera()->GetCameraProjectionMatrix();
		ourShader->SetMatrix4x4("projection", glm::value_ptr(projection));

		for (int i = 0; i < cubePositions.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0);
			model = glm::translate(model, cubePositions[i]);
			ourShader->SetMatrix4x4("model", glm::value_ptr(model));
			cubeGeometry->DrawMesh(ourShader);
		}

		glm::mat4 groundModel = glm::mat4(1.0);
		ourShader->SetMatrix4x4("model", glm::value_ptr(groundModel));
		groundGeometry->DrawMesh(ourShader);
		mainPassFrameBuffer->EndUse();

		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		screenShader->UseProgram();
		lightManager.ApplyLightsToShaderPass(screenShader);
		screenShader->SetVector3("viewPos", player->GetPlayerCamera()->GetCameraPosition());

		glActiveTexture(GL_TEXTURE0);
		screenShader->SetInt("gPosition", 0);
		glBindTexture(GL_TEXTURE_2D, mainPassFrameBuffer->GetGPositionTexture()); // use the color attachment texture as the texture of the quad plane
		glActiveTexture(GL_TEXTURE1);
		screenShader->SetInt("gNormal", 1);
		glBindTexture(GL_TEXTURE_2D, mainPassFrameBuffer->GetGNormalTexture());
		glActiveTexture(GL_TEXTURE2);
		screenShader->SetInt("gAlbedoSpec", 2);
		glBindTexture(GL_TEXTURE_2D, mainPassFrameBuffer->GetGAlbedoSpecTexture());

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}

void TGProgram::TextureCubeMapSection_Skybox()
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
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
	{
		return;
	}

	TGCameraViewInfo playerCameraViewInfo;
	playerCameraViewInfo.mAspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	playerCameraViewInfo.mFov = fov;
	playerCameraViewInfo.mNear = 0.001f;
	playerCameraViewInfo.mFar = 300.0f;
	player->GetPlayerCamera()->SetCameraViewInfo(playerCameraViewInfo);
	player->SetPlayerPosition(glm::vec3(10, 10, 0));
	player->SetPlayerLookAt(glm::vec3(0, 0, 0));

	TGLightManager lightManager;
	lightManager.AddPointLight(glm::vec3(4, 4, 4), glm::vec3(1, 1, 1), 0.1f, 0.09f, 0.032f);
	lightManager.AddPointLight(glm::vec3(-4, -4, -4), glm::vec3(1, 1, 1), 0.1f, 0.09f, 0.032f);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	TGVertexShader* baseVS = new TGVertexShader("Shaders/boxReflection.vert", "MyVS");
	TGFragmentShader* basePS = new TGFragmentShader("Shaders/boxReflection.frag", "MyPS");
	std::shared_ptr<TGShaderProgram> ourShader(new TGShaderProgram());
	ourShader->AddVertexShader(baseVS);
	ourShader->AddFragmentShader(basePS);
	ourShader->BindShaderProgram();

	std::shared_ptr<TGMeshGeometry> cubeGeometry = TGMeshFactory::Get().CreateCube2(6.0);
	std::shared_ptr<TGMaterial> cubeMaterial = std::make_shared<TGMaterial>();
	cubeMaterial->AddTexture("Textures/wall.png", ETGTextureUseType_Diffuse);
	cubeMaterial->AddTexture("Textures/wall_speca.png", ETGTextureUseType_Specular);
	cubeGeometry->SetMaterial(cubeMaterial);

	std::shared_ptr<TGSkyBox> skyBox = std::make_shared<TGSkyBox>(100.0);
	cubeMaterial->AddTexture(skyBox->GetSkyboxTexture());

	// 开启混合
	glEnable(GL_BLEND);
	// 混合的前后颜色参数
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 混合的前后颜色参数，RGB 和 A 分开设置
	// glBlendFuncSeparate();

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

		glm::mat4 cubeModelTransform = glm::mat4(1.0);
		cubeModelTransform = glm::translate(cubeModelTransform, glm::vec3(0, 0, 2.1));
		ourShader->SetMatrix4x4("model", glm::value_ptr(cubeModelTransform));
		cubeGeometry->DrawMesh(ourShader);

		skyBox->Draw(player);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}

void TGProgram::MultiRenderTarget_TexLightBuffer1000()
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
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
	{
		return;
	}

	TGCameraViewInfo playerCameraViewInfo;
	playerCameraViewInfo.mAspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	playerCameraViewInfo.mFov = fov;
	playerCameraViewInfo.mNear = 0.001f;
	playerCameraViewInfo.mFar = 1000.0f;
	player->GetPlayerCamera()->SetCameraViewInfo(playerCameraViewInfo);
	player->SetPlayerPosition(glm::vec3(10, 0, 10));
	player->SetPlayerLookAt(glm::vec3(0, 0, 0));

	TGLightManager lightManager;
	for (int i = -100; i < 100; i += 10)
	{
		for (int j = -100; j < 100; j += 10)
		{
			double randR = (rand() % 10000) / 10000.0;
			double randG = (rand() % 10000) / 10000.0;
			double randB = (rand() % 10000) / 10000.0;
			double randZ = (rand() % 10000) / 10000.0 * 2;
			lightManager.AddPointLight(glm::vec3(i, j, randZ), glm::vec3(randR, randG, randB), 1.0f, 1.0f, 0.6f);
		}
	}

	lightManager.AddDirectionLight(glm::vec3(1.0, 1.0, 0.0), glm::vec3(1.0, 0.0, 0.0));

	for (int i = -45; i < 50; i += 10)
	{
		for (int j = -45; j < 50; j++)
		{
			double randR = (rand() % 10000) / 10000.0;
			double randG = (rand() % 10000) / 10000.0;
			double randB = (rand() % 10000) / 10000.0;
			double randZ = (rand() % 10000) / 10000.0 * 10;

			double cutoff = 0.99;

			lightManager.AddSpotLight(glm::vec3(i, j, randZ), glm::vec3(randR, randG, randB), glm::vec3(0, 0, 1), cutoff);
		}
	}

	std::vector<glm::vec3> cubePositions;
	for (int i = -100; i <= 100; i += 20)
	{
		for (int j = -100; j <= 100; j += 20)
		{
			double randZ = 1.0 + (rand() % 10000) / 10000.0 * 0;
			cubePositions.push_back(glm::vec3(i, j, randZ));
		}
	}

	std::shared_ptr<TGMeshGeometry> groundGeometry = TGMeshFactory::Get().CreateGrid(100.0, 10.0);
	std::shared_ptr<TGMaterial> groundMaterial = std::make_shared<TGMaterial>();
	int textureSlotIndex = groundMaterial->AddTexture("Textures/Grass.png", ETGTextureUseType_Diffuse);
	groundMaterial->AddTexture("Textures/wall_speca.png", ETGTextureUseType_Specular);
	groundMaterial->GetTexture(0)->SetTextureAddressType(ETGTextureAddressType_Repeat, ETGTextureAddressType_Repeat);
	groundGeometry->SetMaterial(groundMaterial);

	std::shared_ptr<TGMeshGeometry> cubeGeometry = TGMeshFactory::Get().CreateCube2(2.0);
	std::shared_ptr<TGMaterial> cubeMaterial = std::make_shared<TGMaterial>();
	cubeMaterial->AddTexture("Textures/wall.png", ETGTextureUseType_Diffuse);
	cubeMaterial->AddTexture("Textures/wall_speca.png", ETGTextureUseType_Specular);
	cubeGeometry->SetMaterial(cubeMaterial);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// Close Depth Test
	// glDepthFunc(GL_ALWAYS);

	// build and compile our shader zprogram
	// ------------------------------------
	TGVertexShader* baseVS = new TGVertexShader("Shaders/gbufferMainPass.vert", "MyVS");
	TGFragmentShader* basePS = new TGFragmentShader("Shaders/gbufferMainPass.frag", "MyPS");
	std::shared_ptr<TGShaderProgram> ourShader(new TGShaderProgram());
	ourShader->AddVertexShader(baseVS);
	ourShader->AddFragmentShader(basePS);
	ourShader->BindShaderProgram();

	TGVertexShader* screenVS = new TGVertexShader("Shaders/gbufferLightPass.vert", "SVS");
	TGFragmentShader* screenPS = new TGFragmentShader("Shaders/gBufferLightPassMass.frag", "SPS");
	std::shared_ptr<TGShaderProgram> screenShader(new TGShaderProgram());
	screenShader->AddVertexShader(screenVS);
	screenShader->AddFragmentShader(screenPS);
	screenShader->BindShaderProgram();

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	std::shared_ptr<TGFrameBuffer> mainPassFrameBuffer = std::make_shared<TGFrameBuffer>(SCR_WIDTH, SCR_HEIGHT);
	int mYaw = 0;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		mainPassFrameBuffer->Use();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader->UseProgram();
		lightManager.ApplyLightsToShaderPass(ourShader);
		glm::mat4 view = player->GetPlayerCamera()->GetCameraViewMatrix();
		ourShader->SetMatrix4x4("view", glm::value_ptr(view));
		glm::mat4 projection = player->GetPlayerCamera()->GetCameraProjectionMatrix();
		ourShader->SetMatrix4x4("projection", glm::value_ptr(projection));

		for (int i = 0; i < cubePositions.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0);
			model = glm::translate(model, cubePositions[i]);
			ourShader->SetMatrix4x4("model", glm::value_ptr(model));
			cubeGeometry->DrawMesh(ourShader);
		}

		glm::mat4 groundModel = glm::mat4(1.0);
		ourShader->SetMatrix4x4("model", glm::value_ptr(groundModel));
		groundGeometry->DrawMesh(ourShader);
		mainPassFrameBuffer->EndUse();

		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		screenShader->UseProgram();
		screenShader->SetVector3("viewPos", player->GetPlayerCamera()->GetCameraPosition());

		glActiveTexture(GL_TEXTURE0);
		screenShader->SetInt("gPosition", 0);
		glBindTexture(GL_TEXTURE_2D, mainPassFrameBuffer->GetGPositionTexture()); // use the color attachment texture as the texture of the quad plane
		glActiveTexture(GL_TEXTURE1);
		screenShader->SetInt("gNormal", 1);
		glBindTexture(GL_TEXTURE_2D, mainPassFrameBuffer->GetGNormalTexture());
		glActiveTexture(GL_TEXTURE2);
		screenShader->SetInt("gAlbedoSpec", 2);
		glBindTexture(GL_TEXTURE_2D, mainPassFrameBuffer->GetGAlbedoSpecTexture());

		lightManager.ApplyLightsToShaderUsingTexBuffer(screenShader);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}

void TGProgram::InstanceTech_MillionNumberCube()
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
	playerCameraViewInfo.mFar = 1000.0f;
	player->GetPlayerCamera()->SetCameraViewInfo(playerCameraViewInfo);
	player->SetPlayerPosition(glm::vec3(10, 0, 10));
	player->SetPlayerLookAt(glm::vec3(0, 0, 0));

	TGLightManager lightManager;
	for (int i = -100; i < 100; i += 10)
	{
		for (int j = -100; j < 100; j += 10)
		{
			double randR = (rand() % 10000) / 10000.0;
			double randG = (rand() % 10000) / 10000.0;
			double randB = (rand() % 10000) / 10000.0;
			double randZ = (rand() % 10000) / 10000.0 * 2;
			lightManager.AddPointLight(glm::vec3(i, j, randZ), glm::vec3(randR, randG, randB), 1.0f, 1.0f, 0.6f);
		}
	}

	lightManager.AddDirectionLight(glm::vec3(1.0, 1.0, 0.0), glm::vec3(1.0, 1.0, 1.0));

	for (int i = -45; i < 50; i += 5)
	{
		for (int j = -45; j < 50; j += 5)
		{
			double randR = (rand() % 10000) / 10000.0;
			double randG = (rand() % 10000) / 10000.0;
			double randB = (rand() % 10000) / 10000.0;
			double randZ = (rand() % 10000) / 10000.0 * 5;

			double cutoff = 0.99;

			lightManager.AddSpotLight(glm::vec3(i, j, randZ), glm::vec3(randR, randG, randB), glm::vec3(0, 0, 1), cutoff);
		}
	}

	std::vector<glm::vec3> cubePositions;
	for (int i = -500; i <= 500; i += 1)
	{
		for (int j = -500; j <= 500; j += 1)
		{
			double randZ = 1.0 + (rand() % 10000) / 10000.0 * 0;
			cubePositions.push_back(glm::vec3(i, j, randZ));
		}
	}

	std::shared_ptr<TGMeshGeometry> groundGeometry = TGMeshFactory::Get().CreateGrid(100.0, 10.0);
	std::shared_ptr<TGMaterial> groundMaterial = std::make_shared<TGMaterial>();
	int textureSlotIndex = groundMaterial->AddTexture("Textures/Grass.png", ETGTextureUseType_Diffuse);
	groundMaterial->AddTexture("Textures/wall_speca.png", ETGTextureUseType_Specular);
	groundMaterial->GetTexture(0)->SetTextureAddressType(ETGTextureAddressType_Repeat, ETGTextureAddressType_Repeat);
	groundGeometry->SetMaterial(groundMaterial);

	std::shared_ptr<TGMeshGeometry> cubeGeometry = TGMeshFactory::Get().CreateCube2(0.5);
	std::shared_ptr<TGMaterial> cubeMaterial = std::make_shared<TGMaterial>();
	cubeMaterial->AddTexture("Textures/wall.png", ETGTextureUseType_Diffuse);
	cubeMaterial->AddTexture("Textures/wall_speca.png", ETGTextureUseType_Specular);
	cubeGeometry->SetMaterial(cubeMaterial);

	std::shared_ptr<TGModel> cubeModel = std::make_shared<TGModel>();
	cubeModel->AddMeshObject(cubeGeometry);

	std::shared_ptr<TGInstancedModel> cubeInstanceComponent = std::make_shared<TGInstancedModel>(cubeModel);
	float* cubeCustomData = new float[cubePositions.size() * 4];
	cubeInstanceComponent->mPerInstancedCustomDataNumber = 4;
	for (int i = 0; i < cubePositions.size(); i++)
	{
		glm::mat4 transformMat = glm::mat4(1.0);
		transformMat = glm::translate(transformMat, cubePositions[i]);
		double randS = (rand() % 10000) / 10000.0 * 2.0;
		transformMat = glm::scale(transformMat, glm::vec3(randS, randS, randS));
		cubeInstanceComponent->AddInstance(transformMat);

		float randR = (rand() % 10000) / 10000.0f;
		float randG = (rand() % 10000) / 10000.0f;
		float randB = (rand() % 10000) / 10000.0f;
		float randA = (rand() % 10000) / 10000.0f;

		cubeCustomData[i * 4 + 0] = randR;
		cubeCustomData[i * 4 + 1] = randG;
		cubeCustomData[i * 4 + 2] = randB;
		cubeCustomData[i * 4 + 3] = randA;
	}
	cubeInstanceComponent->mCustomDataBuffer = cubeCustomData;

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	TGVertexShader* baseVS = new TGVertexShader("Shaders/instancedGBufferMainPass.vert", "MyVS");
	TGFragmentShader* basePS = new TGFragmentShader("Shaders/instancedGBufferMainPass.frag", "MyPS");
	std::shared_ptr<TGShaderProgram> ourShader(new TGShaderProgram());
	ourShader->AddVertexShader(baseVS);
	ourShader->AddFragmentShader(basePS);
	ourShader->BindShaderProgram();

	TGVertexShader* groundVS = new TGVertexShader("Shaders/gbufferMainPass.vert", "GVS");
	TGFragmentShader* groundPS = new TGFragmentShader("Shaders/gbufferMainPass.frag", "GPS");
	std::shared_ptr<TGShaderProgram> groundShader(new TGShaderProgram());
	groundShader->AddVertexShader(groundVS);
	groundShader->AddFragmentShader(groundPS);
	groundShader->BindShaderProgram();

	TGVertexShader* screenVS = new TGVertexShader("Shaders/gbufferLightPass.vert", "SVS");
	TGFragmentShader* screenPS = new TGFragmentShader("Shaders/gBufferLightPassMass.frag", "SPS");
	std::shared_ptr<TGShaderProgram> screenShader(new TGShaderProgram());
	screenShader->AddVertexShader(screenVS);
	screenShader->AddFragmentShader(screenPS);
	screenShader->BindShaderProgram();

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	std::shared_ptr<TGFrameBuffer> mainPassFrameBuffer = std::make_shared<TGFrameBuffer>(SCR_WIDTH, SCR_HEIGHT);
	int mYaw = 0;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		mainPassFrameBuffer->Use();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 使用实例化 VS + PS 渲染大批量立方体
		ourShader->UseProgram();
		glm::mat4 view = player->GetPlayerCamera()->GetCameraViewMatrix();
		ourShader->SetMatrix4x4("view", glm::value_ptr(view));
		glm::mat4 projection = player->GetPlayerCamera()->GetCameraProjectionMatrix();
		ourShader->SetMatrix4x4("projection", glm::value_ptr(projection));
		cubeInstanceComponent->Draw(ourShader);

		// 使用正常的 VS + PS 渲染地面
		groundShader->UseProgram();
		groundShader->SetMatrix4x4("view", glm::value_ptr(view));
		groundShader->SetMatrix4x4("projection", glm::value_ptr(projection));
		glm::mat4 groundModel = glm::mat4(1.0);
		groundShader->SetMatrix4x4("model", glm::value_ptr(groundModel));
		groundGeometry->DrawMesh(groundShader);

		mainPassFrameBuffer->EndUse();

		// 延迟渲染光照信息
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		screenShader->UseProgram();
		screenShader->SetVector3("viewPos", player->GetPlayerCamera()->GetCameraPosition());

		glActiveTexture(GL_TEXTURE0);
		screenShader->SetInt("gPosition", 0);
		glBindTexture(GL_TEXTURE_2D, mainPassFrameBuffer->GetGPositionTexture()); // use the color attachment texture as the texture of the quad plane
		glActiveTexture(GL_TEXTURE1);
		screenShader->SetInt("gNormal", 1);
		glBindTexture(GL_TEXTURE_2D, mainPassFrameBuffer->GetGNormalTexture());
		glActiveTexture(GL_TEXTURE2);
		screenShader->SetInt("gAlbedoSpec", 2);
		glBindTexture(GL_TEXTURE_2D, mainPassFrameBuffer->GetGAlbedoSpecTexture());

		lightManager.ApplyLightsToShaderUsingTexBuffer(screenShader);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

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

	player->ViewRight(-xoffset);
	player->ViewUp(yoffset);
}

void TGProgram::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

void TGProgram::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	auto vi = player->GetPlayerCamera()->getCameraViewInfo();
	vi.mAspectRatio = (float)width / (float)height;
	player->GetPlayerCamera()->SetCameraViewInfo(vi);
}

void TGProgram::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		player->MoveForward(cameraMoveSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		player->MoveForward(-cameraMoveSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		player->MoveRight(cameraMoveSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		player->MoveRight(-cameraMoveSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		player->MoveUp(-cameraMoveSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		player->MoveUp(cameraMoveSpeed);
	}
}
