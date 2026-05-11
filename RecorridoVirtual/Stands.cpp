// Laura Reyes Carrillo
// Proyecto
// 29 de Abril de 2026
// 320015764

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include <string>
#include "Texture.h"


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void UpdateStandAnimation();
void DrawModel(Model& modelObject, Shader shader, GLint modelLoc, glm::vec3 pos, glm::vec3 scale, float rotX, float rotY, float rotZ);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool keys[1024];
bool firstMouse = true;

// ===============================
// ANIMACIÓN DE STANDS
// ===============================

// Activador de la animación
bool animStands = false;

// Tiempo de la animación
float standAnimTime = 0.0f;

// Escala global animada de los stands
float standScale = 0.0f;

// Cantidad de stands
const int NUM_STANDS = 8;

// Posiciones de los stands
glm::vec3 standPositions[NUM_STANDS] = {
	// Lado izquierdo del puente
	glm::vec3(-4.0f, -0.65f, 11.0f),
	glm::vec3(-4.0f, -0.65f, 1.5f),
	glm::vec3(-4.0f, -0.65f, -8.0f),
	glm::vec3(-4.0f, -0.65f, -18.0f),

	// Lado derecho del puente
	glm::vec3(4.0f, -0.65f, 11.0f),
	glm::vec3(4.0f, -0.65f, 1.5f),
	glm::vec3(4.0f, -0.65f, -8.0f),
	glm::vec3(4.0f, -0.65f, -18.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// Rotación de cada stand para que miren hacia el pasillo central
float standRotations[NUM_STANDS] = {
	-90.0f, -90.0f, -90.0f, -90.0f,
	 90.0f,  90.0f,  90.0f,  90.0f
};

// DeltaTime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	// Init GLFW
	glfwInit();

	// Create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Reyes Carrillo Laura", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Callbacks
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// Si quieres que el mouse quede capturado dentro de la ventana, descomenta:
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Init GLEW
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Shader principal
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader skyboxShader("Shader/SkyBox.vs", "Shader/SkyBox.frag");

	// Modelos principales
	Model Puente((char*)"Models/Puente/final_puente.obj");
	Model Stand((char*)"Models/Stands/stands.obj");
	Model Camara((char*)"Models/Camara/cámara.obj");
	Model Lampara((char*)"Models/Lampara/LamparaFI.obj");
	Model Pumagua((char*)"Models/Pumagua/Pumagua.obj");
	Model Router((char*)"Models/Router/Router.obj");
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};


	//VBO, VAO, EBO
	GLuint VAO,VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	//skybox
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//Load textures
	vector <const GLchar*> faces;
	faces.push_back("SkyBox/right.jpg");
	faces.push_back("SkyBox/left.jpg");
	faces.push_back("SkyBox/top.jpg");
	faces.push_back("SkyBox/bottom.jpg");
	faces.push_back("SkyBox/front.jpg");
	faces.push_back("SkyBox/back.jpg");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);


	// Projection matrix
	glm::mat4 projection = glm::perspective(
		camera.GetZoom(),
		(GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT,
		0.1f,
		100.0f
	);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// DeltaTime
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Events
		glfwPollEvents();
		DoMovement();
		UpdateStandAnimation();

		// Clear
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use shader
		lightingShader.Use();

		// Camera position
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(
			viewPosLoc,
			camera.GetPosition().x,
			camera.GetPosition().y,
			camera.GetPosition().z
		);

		// ===============================
		// ÚNICA LUZ: LUZ DIRECCIONAL TIPO SOL
		// ===============================
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.3f, -1.0f, -0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.45f, 0.45f, 0.45f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.75f, 0.75f, 0.75f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.25f, 0.25f, 0.25f);

		// Desactivar point lights si tu shader todavía las tiene declaradas
		for (int i = 0; i < 4; i++)
		{
			std::string base = "pointLights[" + std::to_string(i) + "]";

			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".position").c_str()), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".ambient").c_str()), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".diffuse").c_str()), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".specular").c_str()), 0.0f, 0.0f, 0.0f);

			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".linear").c_str()), 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".quadratic").c_str()), 0.0f);
		}

		// Desactivar spotlight si tu shader todavía lo tiene declarado
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), 0.0f, 0.0f, -1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

		// Material
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// View matrix
		glm::mat4 view = camera.GetViewMatrix();

		// Uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Send view and projection
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// ===============================
		// DIBUJAR PUENTE
		// ===============================
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puente.Draw(lightingShader);

		// ===============================
		// MODELOS DECORATIVOS
		// ===============================

// Cámaras de seguridad
		DrawModel(
			Camara,
			lightingShader,
			modelLoc,
			glm::vec3(0.0f, 4.5f, 11.0f),
			glm::vec3(0.12f, 0.12f, 0.12f),
			180.0f,
			0.0f,
			0.0f
		);

		DrawModel(
			Camara,
			lightingShader,
			modelLoc,
			glm::vec3(0.0f, 4.5f, -18.0f),
			glm::vec3(0.12f, 0.12f, 0.12f),
			180.0f,
			180.0f,
			0.0f
		);

		// Lámparas
		DrawModel(
			Lampara,
			lightingShader,
			modelLoc,
			glm::vec3(-4.0f, 4.0f, 11.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			0.0f,
			0.0f,
			0.0f
		);

		DrawModel(
			Lampara,
			lightingShader,
			modelLoc,
			glm::vec3(4.0f, 4.0f, 1.5f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			0.0f,
			0.0f,
			0.0f
		);

		DrawModel(
			Lampara,
			lightingShader,
			modelLoc,
			glm::vec3(-4.0f, 4.0f, -8.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			0.0f,
			0.0f,
			0.0f
		);

		DrawModel(
			Lampara,
			lightingShader,
			modelLoc,
			glm::vec3(4.0f, 4.0f, -18.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			0.0f,
			0.0f,
			0.0f
		);

		// Estación Pumagua
		DrawModel(
			Pumagua,
			lightingShader,
			modelLoc,
			glm::vec3(4.0f, -0.65f, 16.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			0.0f,
			-90.0f,
			0.0f
		);

		// Router
		DrawModel(
			Router,
			lightingShader,
			modelLoc,
			glm::vec3(0.0f, 4.5f, -14.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			0.0f,
			0.0f,
			0.0f
		);

		// ===============================
		// DIBUJAR STANDS ANIMADOS
		// ===============================
		for (int i = 0; i < NUM_STANDS; i++)
		{
			glm::vec3 finalScale = glm::vec3(1.0f, 1.0f, 1.0f) * standScale;

			DrawModel(
				Stand,
				lightingShader,
				modelLoc,
				standPositions[i],
				finalScale,
				0.0f,
				standRotations[i],
				0.0f
			);

		}
		//Draw skybox last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default


		// Swap buffers
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);

	glfwTerminate();

	return 0;
}

// ===============================
// MOVIMIENTO DE CÁMARA
// ===============================
void DoMovement()
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

// ===============================
// TECLADO
// ===============================
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	// Tecla 1: activa o desactiva la animación de los stands
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		animStands = !animStands;
	}
}

// ===============================
// ANIMACIÓN DE STANDS CON KEYFRAMES
// ===============================
void UpdateStandAnimation()
{
	if (animStands)
	{
		standAnimTime += deltaTime;

		if (standAnimTime > 5.0f)
			standAnimTime = 5.0f;
	}
	else
	{
		standAnimTime -= deltaTime;

		if (standAnimTime < 0.0f)
			standAnimTime = 0.0f;
	}

	// KeyFrames:
	// KF0 = 0.0s -> Puente vacío, scale = 0.0
	// KF1 = 1.0s -> Aparición inicial, scale = 0.3
	// KF2 = 2.0s -> Crecimiento, scale = 0.6
	// KF3 = 3.5s -> Posicionamiento, scale = 0.9
	// KF4 = 5.0s -> Estado final, scale = 1.0

	if (standAnimTime < 1.0f)
	{
		standScale = glm::mix(0.0f, 0.3f, standAnimTime / 1.0f);
	}
	else if (standAnimTime < 2.0f)
	{
		standScale = glm::mix(0.3f, 0.6f, (standAnimTime - 1.0f) / 1.0f);
	}
	else if (standAnimTime < 3.5f)
	{
		standScale = glm::mix(0.6f, 0.9f, (standAnimTime - 2.0f) / 1.5f);
	}
	else
	{
		standScale = glm::mix(0.9f, 1.0f, (standAnimTime - 3.5f) / 1.5f);
	}
}

// ===============================
// DIBUJAR MODELO DE STAND
// ===============================
void DrawModel(Model& modelObject, Shader shader, GLint modelLoc, glm::vec3 pos, glm::vec3 scale, float rotX, float rotY, float rotZ)
{
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, pos);

	// Rotación en X: inclinar hacia arriba o hacia abajo
	model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));

	// Rotación en Y: girar izquierda/derecha
	model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));

	// Rotación en Z: ladear el modelo
	model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::scale(model, scale);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	modelObject.Draw(shader);
}

// ===============================
// MOUSE
// ===============================
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}