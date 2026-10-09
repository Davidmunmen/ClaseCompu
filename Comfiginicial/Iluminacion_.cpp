// David Muñoz Mendoza 
// Número de Cuenta: 320168327
// previo practica 8
// Fecha de entrega 04 de Octubre de 2026
// Lab de Computación Gráfica grupo 1

// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 1100, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 2.5f, 25.0f)); // Ajustada distancia Z para mejor perspectiva a nueva escala
glm::vec3 lightPos(0.25f, 0.25f, 1.25f);      
float movelightPos = 0.0f;

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Light attributes
float rot = 0.0f, rotLuna=0.0f;
bool activanim = false;

int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Carga de modelos y camara sintetica con iluminación P_8 David Muñoz Mendoza", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Load models
    Model dog((char*)"Models/RedDog.obj");
    Model heno((char*)"Models/heno/feno.obj");
    Model carro((char*)"Models/camioneta/Hilux.obj");
    Model camino((char*)"Models/camino/Road.obj");
    Model arbol((char*)"Models/arboles/Gledista_Triacanthos_6.obj");
    Model arbol_1((char*)"Models/arboles/Gledista_Triacanthos_3.obj");
    Model rana((char*)"Models/bee/CRAPAUD_TOAD_PBR_OBJ_NO-MOD.obj");
	Model sol((char*)"Models/sol/Neptune.obj");
	Model luna((char*)"Models/luna/Moon.obj");
    // Setup and compile shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //pivote
        glm::mat4 pivot = glm::mat4(1.0f);
        pivot = glm::rotate(pivot, glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 pivotlun = glm::mat4(1.0f);
        pivotlun = glm::rotate(pivotlun, glm::radians(rotLuna), glm::vec3(0.0f, 0.0f, 1.0f));

        //sol
        glm::mat4 modelSolPos = pivot;
        modelSolPos = glm::translate(modelSolPos, glm::vec3(15.0f, 2.0f, 0.0f));
        glm::vec3 currentLightPos = glm::vec3(modelSolPos[3]);

        //luna
        glm::mat4 modelLunaPos = pivotlun;
        modelLunaPos = glm::translate(modelLunaPos, glm::vec3(-15.0f, 2.0f, 0.0f));
        glm::vec3 light2Pos = glm::vec3(modelLunaPos[3]);

        // ACTIVAR Y CONFIGURAR SHADER DE ILUMINACIÓN
        lightingShader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"), currentLightPos.x, currentLightPos.y, currentLightPos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.2f, 0.05f, 0.02f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.9f, 0.35f, 0.15f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.6f, 0.3f, 0.2f);

        //luna 
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.position"), light2Pos.x, light2Pos.y, light2Pos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.02f, 0.05f, 0.15f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.15f, 0.3f, 0.7f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.2f, 0.3f, 0.6f);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

        // Perro
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(lightingShader);

        // Heno 1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.1f, 0.15f, -0.25f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        heno.Draw(lightingShader);

        // Heno 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.5f, 0.15f, -0.25f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        heno.Draw(lightingShader);

        // Heno 3
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.3f, 0.65f, -0.25f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        heno.Draw(lightingShader);

        // Carro
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.00f, -0.10f, -0.75f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.45f, 0.40f, 0.40f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        carro.Draw(lightingShader);

        // Camino
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.00f, -0.60f, -0.75f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 0.40f, 0.90f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        camino.Draw(lightingShader);

        // arboles
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.00f, -0.60f, -5.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.225f, 0.05f, 0.225f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.00f, -0.60f, -5.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.225f, 0.05f, 0.225f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.00f, -0.60f, -15.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.225f, 0.20f, 0.225f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol_1.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.00f, -0.60f, -15.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.225f, 0.20f, 0.225f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol_1.Draw(lightingShader);

        // Rana
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.15f, -0.15f, 0.0f));
        model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        rana.Draw(lightingShader);

        // sol
        shader.Use();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        model = modelSolPos;
        model = glm::scale(model, glm::vec3(0.35f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        sol.Draw(shader);

        // luna
        shader.Use();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        model = modelLunaPos;
        model = glm::scale(model, glm::vec3(5.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        luna.Draw(shader);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

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

    if (keys[GLFW_KEY_O])
    {
        rot += 50.0f * deltaTime; // Rotación hacia la derecha
    }

    if (keys[GLFW_KEY_L])
    {
        rot -= 50.0f * deltaTime; // Rotación hacia la izquierda
    }
    if (keys[GLFW_KEY_I])
    {
        rotLuna += 50.0f * deltaTime;
    }
    if (keys[GLFW_KEY_K])
    {
        rotLuna -= 50.0f * deltaTime;
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