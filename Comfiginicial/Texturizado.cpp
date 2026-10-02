//David Muñoz Mendoza 
// Número de Cuenta: 320168327
// practica 7
//Fecha de entrega 02 de Octubre de 2026
// Lab de Conputación Grafica grupo 1
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

// Other includes
#include "Shader.h"
#include "Camera.h"


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Texturizado David Muñoz Mendoza ", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);


	// Build and compile our shader program
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	/*	glGenVertexArrays(1, &VAO_1);
	glGenBuffers(1, &VBO_1);
	glGenVertexArrays(1, &VAO_2);
	glGenBuffers(1, &VBO_2);
	glGenVertexArrays(1, &VAO_3);
	glGenBuffers(1, &VBO_3);
	glGenVertexArrays(1, &VAO_4);
	glGenBuffers(1, &VBO_4);
	glGenVertexArrays(1, &VAO_5);
	glGenBuffers(1, &VBO_5);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO_1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_1);
	glBindVertexArray(VAO_2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_2);
	glBindVertexArray(VAO_3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_3);
	glBindVertexArray(VAO_4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_4);
	glBindVertexArray(VAO_5);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_5);*/
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =//1
	{
		// Positions            // Colors              // Texture Coords
		-0.5f, -0.5f, 0.0f,    1.0f, 1.0f,1.0f,		0.08f,0.15f,//inf iz
		0.5f, -0.5f, 0.0f,	   1.0f, 1.0f,1.0f,		0.21f,0.15f,
		0.5f,  0.5f, 0.0f,     1.0f, 1.0f,1.0f,	    0.21f,0.38f,// el numero de la derecha viene en conjunto 
		-0.5f,  0.5f, 0.0f,    1.0f, 1.0f,1.0f,		0.08f,0.38f,//inf der


	};
	// Dado 2 (Última fila)
	GLfloat vertices_2[] =
	{
		// Positions            // Colors              // Texture Coords
		-0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.225f, 0.15f, // Esquina inferior izquierda
		 0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.355f, 0.15f, // Esquina inferior derecha
		 0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.355f, 0.38f, // Esquina superior derecha
		-0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.225f, 0.38f  // Esquina superior izquierda
	};

	// Dado 3 (Última fila)
	GLfloat vertices_3[] =
	{
		// Positions            // Colors              // Texture Coords
		-0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.370f, 0.15f, // Esquina inferior izquierda
		 0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.500f, 0.15f, // Esquina inferior derecha
		 0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.500f, 0.38f, // Esquina superior derecha
		-0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.370f, 0.38f  // Esquina superior izquierda
	};

	// Dado 4 (Última fila)
	GLfloat vertices_4[] =
	{
		// Positions            // Colors              // Texture Coords
		-0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.522f, 0.15f, // Esquina inferior izquierda
		 0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.632f, 0.15f, // Esquina inferior derecha
		 0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.632f, 0.38f, // Esquina superior derecha
		-0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.522f, 0.38f  // Esquina superior izquierda
	};

	// Dado 5 (Última fila)
	GLfloat vertices_5[] =
	{
		// Positions            // Colors              // Texture Coords
		-0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.65f, 0.15f, // Esquina inferior izquierda
		 0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.77f, 0.15f, // Esquina inferior derecha
		 0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.77f, 0.38f, // Esquina superior derecha
		-0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.65f, 0.38f  // Esquina superior izquierda
	};

	// Dado 6 (Última fila)
	GLfloat vertices_6[] =
	{
		// Positions            // Colors              // Texture Coords
		-0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.800f, 0.15f, // Esquina inferior izquierda
		 0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.930f, 0.15f, // Esquina inferior derecha
		 0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.930f, 0.38f, // Esquina superior derecha
		-0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.800f, 0.38f  // Esquina superior izquierda
	};

GLuint indices[] =
{  // Note that we start from 0!
	0,1,3,
	1,2,3

};

// First, set the container's VAO (and VBO)
GLuint VBO, VAO, EBO,
VBO1, VAO1, EBO1,
VBO2, VAO2, EBO2,
VBO3, VAO3, EBO3,
VBO4, VAO4, EBO4,
VBO5, VAO5, EBO5;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glGenBuffers(1, &EBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);

glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

// Position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);
// Color attribute
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
glEnableVertexAttribArray(1);
// Texture Coordinate attribute
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
glEnableVertexAttribArray(2);
glBindVertexArray(0);

//PARA EL VAO 1
glGenVertexArrays(1, &VAO1);
glGenBuffers(1, &VBO1);
glGenBuffers(1, &EBO1);

glBindVertexArray(VAO1);
glBindBuffer(GL_ARRAY_BUFFER, VBO1);

glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

// Position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);
// Color attribute
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
glEnableVertexAttribArray(1);
// Texture Coordinate attribute
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
glEnableVertexAttribArray(2);
glBindVertexArray(0);
//PARA EL VAO 2
glGenVertexArrays(1, &VAO2);
glGenBuffers(1, &VBO2);
glGenBuffers(1, &EBO2);

glBindVertexArray(VAO2);
glBindBuffer(GL_ARRAY_BUFFER, VBO2);

glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_3), vertices_3, GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

// Position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);
// Color attribute
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
glEnableVertexAttribArray(1);
// Texture Coordinate attribute
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
glEnableVertexAttribArray(2);
glBindVertexArray(0);
//vAO 3
glGenVertexArrays(1, &VAO3);
glGenBuffers(1, &VBO3);
glGenBuffers(1, &EBO3);

glBindVertexArray(VAO3);
glBindBuffer(GL_ARRAY_BUFFER, VBO3);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_4), vertices_4, GL_STATIC_DRAW);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
glEnableVertexAttribArray(1);
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
glEnableVertexAttribArray(2);
glBindVertexArray(0);
//vAO 4

glGenVertexArrays(1, &VAO4);
glGenBuffers(1, &VBO4);
glGenBuffers(1, &EBO4);

glBindVertexArray(VAO4);
glBindBuffer(GL_ARRAY_BUFFER, VBO4);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_5), vertices_5, GL_STATIC_DRAW);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
glEnableVertexAttribArray(1);
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
glEnableVertexAttribArray(2);
glBindVertexArray(0);
// VAO 5

glGenVertexArrays(1, &VAO5);
glGenBuffers(1, &VBO5);
glGenBuffers(1, &EBO5);

glBindVertexArray(VAO5);
glBindBuffer(GL_ARRAY_BUFFER, VBO5);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_6), vertices_6, GL_STATIC_DRAW);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
glEnableVertexAttribArray(1);
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
glEnableVertexAttribArray(2);
glBindVertexArray(0);
// Load textures
GLuint texture1, texture2, texture3;
glGenTextures(1, &texture1);
glGenTextures(1, &texture2);
glGenTextures(1, &texture3);

glBindTexture(GL_TEXTURE_2D, texture1);
int textureWidth, textureHeight, nrChannels;
stbi_set_flip_vertically_on_load(true);
unsigned char* image;
unsigned char* image_2;//textura 2
unsigned char* image_3;//textura 3
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
// Diffuse map
image = stbi_load("images/dado.png", &textureWidth, &textureHeight, &nrChannels, 0);
glBindTexture(GL_TEXTURE_2D, texture1);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
glGenerateMipmap(GL_TEXTURE_2D);
if (image)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
}
else
{
	std::cout << "Failed to load texture" << std::endl;
}
stbi_image_free(image);

//imagen 2

glBindTexture(GL_TEXTURE_2D, texture2);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

image_2 = stbi_load("images/dado.png", &textureWidth, &textureHeight, &nrChannels, 0);


if (image_2)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_2);
	glGenerateMipmap(GL_TEXTURE_2D);
}
else
{
	std::cout << "Failed to load texture" << std::endl;
}
stbi_image_free(image_2);
//imagen 3 

glBindTexture(GL_TEXTURE_2D, texture3);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

image_3 = stbi_load("images/dado.png", &textureWidth, &textureHeight, &nrChannels, 0);


if (image_3)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_3);
	glGenerateMipmap(GL_TEXTURE_2D);
}
else
{
	std::cout << "Failed to load texture" << std::endl;
}
stbi_image_free(image_3);
//imagen 4
// 	
glBindTexture(GL_TEXTURE_2D, texture3);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

image_3 = stbi_load("images/dado.png", &textureWidth, &textureHeight, &nrChannels, 0);


if (image_3)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_3);
	glGenerateMipmap(GL_TEXTURE_2D);
}
else
{
	std::cout << "Failed to load texture" << std::endl;
}
stbi_image_free(image_3);
// Game loop
while (!glfwWindowShouldClose(window))
{
	// Calculate deltatime of current frame
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
	glfwPollEvents();
	DoMovement();

	// Clear the colorbuffer
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lampShader.Use();
	//// Create camera transformations
	glm::mat4 view;
	view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 model(1);
	// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
	GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

	// Bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Reutilizamos la variable 'model' en lugar de volver a escribir 'glm::mat4'
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//textura 2 arriba

	// Bind diffuse map

	glBindTexture(GL_TEXTURE_2D, texture2);

	// Reutilizamos la variable 'model' en lugar de volver a escribir 'glm::mat4'
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.5f, -0.5f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO1);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//textura 3 atras

		// Bind diffuse map

	glBindTexture(GL_TEXTURE_2D, texture2);

	// Reutilizamos la variable 'model' en lugar de volver a escribir 'glm::mat4'
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, -1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//textura 4 derecha

	glBindTexture(GL_TEXTURE_2D, texture3);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -0.5f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Set matrices
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	// Draw the light object (using light's vertex attributes)
	glBindVertexArray(VAO3);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//textura 5 izquierda

	glBindTexture(GL_TEXTURE_2D, texture3);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, -0.5f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// Set matrices
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	// Draw the light object (using light's vertex attributes)
	glBindVertexArray(VAO4);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//textura 6 abajo

	glBindTexture(GL_TEXTURE_2D, texture3);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, -0.5f, -0.5f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// Set matrices
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	// Draw the light object (using light's vertex attributes)
	glBindVertexArray(VAO5);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Swap the screen buffers
	glfwSwapBuffers(window);
}

glDeleteVertexArrays(1, &VAO);
glDeleteBuffers(1, &VBO);
glDeleteBuffers(1, &EBO);
// Terminate GLFW, clearing any resources allocated by GLFW.
glfwTerminate();

return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
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

// Is called whenever a key is pressed/released via GLFW
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
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}







