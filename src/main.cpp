#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Camera.h"
#include "Model.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Screen settings
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

static float mixValue = 0.4f;

// Camera setup
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static bool firstMouse = true;
static float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;

// Timing value
static float deltaTime = 0.0f; // time between current frame and last frame.
static float lastFrame = 0.0f;

static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create the window.
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Test", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Vsync => switch to 0 to test true performance.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    stbi_set_flip_vertically_on_load(true);

    // Disable cursor.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader myShader(ASSETS_DIR "shaders/shader.vert", ASSETS_DIR "shaders/lighting.frag");
    Model myModel(ASSETS_DIR "models/backpack/backpack.obj");

    int numberFrame = 0;
    float timeFrame = 0.0f;

    while(!glfwWindowShouldClose(window)) {

        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        timeFrame += deltaTime;

        // Process the input of the user.
        processInput(window);

        // Put a teal color background.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rendering pipeline.
        myShader.use();
        // be sure to activate shader when setting uniforms/drawing objects
        myShader.setVec3("viewPos", camera.Position);
        myShader.setFloat("material.shininess", 32.0f);

        /*
           Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
           the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
           by defining light types as classes and set their values in there, or by using a more efficient uniform approach
           by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
        */
        // directional light
        myShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        myShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        myShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        myShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        myShader.setVec3("pointLights[0].position", glm::vec3( 0.7f,  0.2f,  2.0f));
        myShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        myShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        myShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        myShader.setFloat("pointLights[0].constant", 1.0f);
        myShader.setFloat("pointLights[0].linear", 0.09f);
        myShader.setFloat("pointLights[0].quadratic", 0.032f);
        myShader.setVec3("spotLight.position", camera.Position);
        myShader.setVec3("spotLight.direction", camera.Front);
        myShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        myShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        myShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        myShader.setFloat("spotLight.constant", 1.0f);
        myShader.setFloat("spotLight.linear", 0.09f);
        myShader.setFloat("spotLight.quadratic", 0.032f);
        myShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        myShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));  

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        myShader.setMat4("projection", projection); // this matrix rarely changes, thus executing this operation outside the main loop could prove useful.
        myShader.setMat4("view", view);

        auto model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        myShader.setMat4("model", model);
        myModel.Draw(myShader);
        numberFrame++;

        if (timeFrame >= 1.0f) {
            unsigned int fps = numberFrame / timeFrame;
            std::string title = "FPS: " + std::to_string(fps);
            glfwSetWindowTitle(window, title.c_str());
            numberFrame = 0;
            timeFrame = 0.0f;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        mixValue = std::clamp(mixValue + 0.005f, 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        mixValue = std::clamp(mixValue - 0.005f, 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}