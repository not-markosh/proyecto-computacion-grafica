#include <iostream>
#include <cmath>
#include <fstream> 

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

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();
void saveToFile();
void loadFromFile();

// Window dimensions
const GLuint WIDTH = 1000, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
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

glm::vec3 Light1 = glm::vec3(0);

// VARIABLES DE ANIMACIÓN DEL HUMANOIDE
float torsoPosX = 0.0f, torsoPosY = 0.0f, torsoPosZ = 0.0f;
float rotTorso = 0.0f;
float angCabeza = 0.0f;
float angBrazoIzq = 0.0f;
float angBrazoDer = 0.0f;
float angMusloIzq = 0.0f;
float angMusloDer = 0.0f;
float angPantorrillaIzq = 0.0f;
float angPantorrillaDer = 0.0f;

#define MAX_FRAMES 50
int i_max_steps = 190;
int i_curr_steps = 0;

typedef struct _frame {
	float torsoPosX, torsoPosY, torsoPosZ;
	float incX, incY, incZ;
	float rotTorso, rotTorsoInc;
	float angCabeza, angCabezaInc;
	float angBrazoIzq, angBrazoIzqInc;
	float angBrazoDer, angBrazoDerInc;
	float angMusloIzq, angMusloIzqInc;
	float angMusloDer, angMusloDerInc;
	float angPantorrillaIzq, angPantorrillaIzqInc;
	float angPantorrillaDer, angPantorrillaDerInc;
} FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;
bool play = false;
int playIndex = 0;


void saveFrame(void) {
	KeyFrame[FrameIndex].torsoPosX = torsoPosX;
	KeyFrame[FrameIndex].torsoPosY = torsoPosY;
	KeyFrame[FrameIndex].torsoPosZ = torsoPosZ;
	KeyFrame[FrameIndex].rotTorso = rotTorso;
	KeyFrame[FrameIndex].angCabeza = angCabeza;
	KeyFrame[FrameIndex].angBrazoIzq = angBrazoIzq;
	KeyFrame[FrameIndex].angBrazoDer = angBrazoDer;
	KeyFrame[FrameIndex].angMusloIzq = angMusloIzq;
	KeyFrame[FrameIndex].angMusloDer = angMusloDer;
	KeyFrame[FrameIndex].angPantorrillaIzq = angPantorrillaIzq;
	KeyFrame[FrameIndex].angPantorrillaDer = angPantorrillaDer;
	FrameIndex++;
}

void resetElements(void) {
	torsoPosX = KeyFrame[0].torsoPosX;
	torsoPosY = KeyFrame[0].torsoPosY;
	torsoPosZ = KeyFrame[0].torsoPosZ;
	rotTorso = KeyFrame[0].rotTorso;
	angCabeza = KeyFrame[0].angCabeza;
	angBrazoIzq = KeyFrame[0].angBrazoIzq;
	angBrazoDer = KeyFrame[0].angBrazoDer;
	angMusloIzq = KeyFrame[0].angMusloIzq;
	angMusloDer = KeyFrame[0].angMusloDer;
	angPantorrillaIzq = KeyFrame[0].angPantorrillaIzq;
	angPantorrillaDer = KeyFrame[0].angPantorrillaDer;
}

void interpolation(void) {
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].torsoPosX - KeyFrame[playIndex].torsoPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].torsoPosY - KeyFrame[playIndex].torsoPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].torsoPosZ - KeyFrame[playIndex].torsoPosZ) / i_max_steps;

	KeyFrame[playIndex].rotTorsoInc = (KeyFrame[playIndex + 1].rotTorso - KeyFrame[playIndex].rotTorso) / i_max_steps;
	KeyFrame[playIndex].angCabezaInc = (KeyFrame[playIndex + 1].angCabeza - KeyFrame[playIndex].angCabeza) / i_max_steps;
	KeyFrame[playIndex].angBrazoIzqInc = (KeyFrame[playIndex + 1].angBrazoIzq - KeyFrame[playIndex].angBrazoIzq) / i_max_steps;
	KeyFrame[playIndex].angBrazoDerInc = (KeyFrame[playIndex + 1].angBrazoDer - KeyFrame[playIndex].angBrazoDer) / i_max_steps;
	KeyFrame[playIndex].angMusloIzqInc = (KeyFrame[playIndex + 1].angMusloIzq - KeyFrame[playIndex].angMusloIzq) / i_max_steps;
	KeyFrame[playIndex].angMusloDerInc = (KeyFrame[playIndex + 1].angMusloDer - KeyFrame[playIndex].angMusloDer) / i_max_steps;
	KeyFrame[playIndex].angPantorrillaIzqInc = (KeyFrame[playIndex + 1].angPantorrillaIzq - KeyFrame[playIndex].angPantorrillaIzq) / i_max_steps;
	KeyFrame[playIndex].angPantorrillaDerInc = (KeyFrame[playIndex + 1].angPantorrillaDer - KeyFrame[playIndex].angPantorrillaDer) / i_max_steps;
}

void saveToFile() {
	std::ofstream file("animacion.txt");
	if (file.is_open()) {
		file << FrameIndex << "\n";
		for (int i = 0; i < FrameIndex; i++) {
			file << KeyFrame[i].torsoPosX << " " << KeyFrame[i].torsoPosY << " " << KeyFrame[i].torsoPosZ << " "
				<< KeyFrame[i].rotTorso << " " << KeyFrame[i].angCabeza << " "
				<< KeyFrame[i].angBrazoIzq << " " << KeyFrame[i].angBrazoDer << " "
				<< KeyFrame[i].angMusloIzq << " " << KeyFrame[i].angMusloDer << " "
				<< KeyFrame[i].angPantorrillaIzq << " " << KeyFrame[i].angPantorrillaDer << "\n";
		}
		file.close();
		printf("Animacion guardada exitosamente en 'animacion.txt'!\n");
	}
	else {
		printf("Error al abrir archivo para guardar.\n");
	}
}

void loadFromFile() {
	std::ifstream file("animacion.txt");
	if (file.is_open()) {
		file >> FrameIndex;
		for (int i = 0; i < FrameIndex; i++) {
			file >> KeyFrame[i].torsoPosX >> KeyFrame[i].torsoPosY >> KeyFrame[i].torsoPosZ
				>> KeyFrame[i].rotTorso >> KeyFrame[i].angCabeza
				>> KeyFrame[i].angBrazoIzq >> KeyFrame[i].angBrazoDer
				>> KeyFrame[i].angMusloIzq >> KeyFrame[i].angMusloDer
				>> KeyFrame[i].angPantorrillaIzq >> KeyFrame[i].angPantorrillaDer;
		}
		file.close();
		printf("Animacion cargada desde 'animacion.txt'! Frames totales: %d\n", FrameIndex);
		resetElements();
	}
	else {
		printf("No se encontro 'animacion.txt'.\n");
	}
}

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Animacion maquina de estados Lopez Lopez Jose Rodrigo", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	Model torso((char*)"Models/Persona/torso.obj");
	Model cabeza((char*)"Models/Persona/cabeza.obj");
	Model brazoIzq((char*)"Models/Persona/brazoIzq.obj");
	Model brazoDer((char*)"Models/Persona/brazoDer.obj");
	Model piernaIzq((char*)"Models/Persona/piernaIzq.obj");
	Model piernaDer((char*)"Models/Persona/piernaDer.obj");
	Model pantorrillaIzq((char*)"Models/Persona/pantorrillaIzq.obj");
	Model pantorrillaDer((char*)"Models/Persona/pantorrillaDer.obj");

	for (int i = 0; i < MAX_FRAMES; i++) {
		KeyFrame[i] = { 0 };
	}

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		DoMovement();
		Animation();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		glm::mat4 modelTemp = glm::mat4(1.0f);

		lightingShader.Use();
		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		glm::mat4 view;
		view = camera.GetViewMatrix();

		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);

		view = camera.GetViewMatrix();

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// 1. TORSO (Nodo Raiz)
		modelTemp = model = glm::mat4(1.0f);
		modelTemp = model = glm::translate(model, glm::vec3(torsoPosX, torsoPosY, torsoPosZ));
		modelTemp = model = glm::rotate(model, glm::radians(rotTorso), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		torso.Draw(lightingShader);

		// 2. CABEZA (Hija del Torso)
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.0725f, 0.0f));
		model = glm::rotate(model, glm::radians(angCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cabeza.Draw(lightingShader);

		// 3. BRAZO IZQUIERDO (Hijo del Torso)
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0187f, 0.0591f, 0.0f));
		model = glm::rotate(model, glm::radians(angBrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		brazoIzq.Draw(lightingShader);

		// 4. BRAZO DERECHO (Hijo del Torso)
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.0179f, 0.0594f, 0.0f));
		model = glm::rotate(model, glm::radians(angBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		brazoDer.Draw(lightingShader);

		// 5. MUSLO IZQUIERDO (Hijo del Torso)
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0103f, 0.0029f, 0.0f));
		model = glm::rotate(model, glm::radians(angMusloIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 matMusloIzq = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		piernaIzq.Draw(lightingShader);

		// 6. PANTORRILLA IZQUIERDA (Hija del Muslo Izquierdo)
		model = matMusloIzq;
		model = glm::translate(model, glm::vec3(0.0068f, -0.0384f, 0.0f));
		model = glm::rotate(model, glm::radians(angPantorrillaIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pantorrillaIzq.Draw(lightingShader);

		// 7. MUSLO DERECHO (Hijo del Torso)
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.0073f, 0.0068f, 0.0f));
		model = glm::rotate(model, glm::radians(angMusloDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 matMusloDer = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		piernaDer.Draw(lightingShader);

		// 8. PANTORRILLA DERECHA (Hija del Muslo Derecho)
		model = matMusloDer;
		model = glm::translate(model, glm::vec3(-0.0097f, -0.0438f, 0.0f));
		model = glm::rotate(model, glm::radians(angPantorrillaDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pantorrillaDer.Draw(lightingShader);

		lampShader.Use();
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void DoMovement()
{
	if (keys[GLFW_KEY_UP]) camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_DOWN]) camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_LEFT]) camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

	// MOVIMIENTO DEL TORSO (Posición y Giro)
	if (keys[GLFW_KEY_W]) torsoPosZ += 0.01f;
	if (keys[GLFW_KEY_S]) torsoPosZ -= 0.01f;
	if (keys[GLFW_KEY_A]) torsoPosX += 0.01f;
	if (keys[GLFW_KEY_D]) torsoPosX -= 0.01f;
	if (keys[GLFW_KEY_R]) torsoPosY += 0.01f;
	if (keys[GLFW_KEY_F]) torsoPosY -= 0.01f;

	if (keys[GLFW_KEY_Q]) rotTorso += 1.0f;
	if (keys[GLFW_KEY_E]) rotTorso -= 1.0f;

	// Cabeza
	if (keys[GLFW_KEY_1]) angCabeza += 1.0f;
	if (keys[GLFW_KEY_2]) angCabeza -= 1.0f;
	// Brazos
	if (keys[GLFW_KEY_3]) angBrazoIzq += 1.0f;
	if (keys[GLFW_KEY_4]) angBrazoIzq -= 1.0f;
	if (keys[GLFW_KEY_5]) angBrazoDer += 1.0f;
	if (keys[GLFW_KEY_6]) angBrazoDer -= 1.0f;
	// Muslos
	if (keys[GLFW_KEY_T]) angMusloIzq += 1.0f;
	if (keys[GLFW_KEY_G]) angMusloIzq -= 1.0f;
	if (keys[GLFW_KEY_Y]) angMusloDer += 1.0f;
	if (keys[GLFW_KEY_H]) angMusloDer -= 1.0f;
	// Pantorrillas
	if (keys[GLFW_KEY_U]) angPantorrillaIzq += 1.0f;
	if (keys[GLFW_KEY_J]) angPantorrillaIzq -= 1.0f;
	if (keys[GLFW_KEY_I]) angPantorrillaDer += 1.0f;
	if (keys[GLFW_KEY_K]) angPantorrillaDer -= 1.0f;

	// Luces
	if (keys[GLFW_KEY_Y]) pointLightPositions[0].x += 0.01f;
	if (keys[GLFW_KEY_H]) pointLightPositions[0].z -= 0.1f;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Reproducir Animacion
	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{
			resetElements();
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}
	}

	// Guardar Fotograma en Memoria
	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}

	// Guardar en archivo
	if (keys[GLFW_KEY_O])
	{
		saveToFile();
	}

	// Cargar desde archivo
	if (keys[GLFW_KEY_P])
	{
		loadFromFile();
	}

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

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
		}
		else
		{
			Light1 = glm::vec3(0);
		}
	}
}

void Animation() {
	if (play) {
		if (i_curr_steps >= i_max_steps) {
			playIndex++;
			if (playIndex > FrameIndex - 2) {
				playIndex = 0;
				play = false;
			}
			else {
				i_curr_steps = 0;
				interpolation();
			}
		}
		else {
			torsoPosX += KeyFrame[playIndex].incX;
			torsoPosY += KeyFrame[playIndex].incY;
			torsoPosZ += KeyFrame[playIndex].incZ;
			rotTorso += KeyFrame[playIndex].rotTorsoInc;
			angCabeza += KeyFrame[playIndex].angCabezaInc;
			angBrazoIzq += KeyFrame[playIndex].angBrazoIzqInc;
			angBrazoDer += KeyFrame[playIndex].angBrazoDerInc;
			angMusloIzq += KeyFrame[playIndex].angMusloIzqInc;
			angMusloDer += KeyFrame[playIndex].angMusloDerInc;
			angPantorrillaIzq += KeyFrame[playIndex].angPantorrillaIzqInc;
			angPantorrillaDer += KeyFrame[playIndex].angPantorrillaDerInc;
			i_curr_steps++;
		}
	}
}

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