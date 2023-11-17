#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/define.hpp"
#include <iostream>
#include "utils/camera.hpp"
#include "utils/shader.hpp"
#include "utils/cube.hpp"
#include "utils/victor_utils.hpp"
#include "utils/skybox.hpp"
#include "utils/sphere.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lights
// ------
glm::vec3 lightPositions[] = {
    glm::vec3(-10.0f,  10.0f, 10.0f),
    glm::vec3(10.0f,  10.0f, 10.0f),
    glm::vec3(-10.0f, -10.0f, 10.0f),
    glm::vec3(10.0f, -10.0f, 10.0f),
};
glm::vec3 lightColors[] = {
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f)
};


int main()
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
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // init projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // build and compile our shader zprogram
    // ------------------------------------
    const std::string shaderFolder = "src/shaders/";
    Shader lightingShader("src/shaders/1.colors.vs", "src/shaders/1.colors.fs");
    Shader lightCubeShader("src/shaders/1.light_cube.vs", "src/shaders/1.light_cube.fs");
    Shader skyboxShader("src/shaders/skybox.vs", "src/shaders/skybox.fs");
    Shader manualPBR("src/shaders/manual_pbr.vs", "src/shaders/manual_pbr.fs");


    // INIT STATIC UNIFORM PARAMETER 
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    manualPBR.use();
    manualPBR.setVec3("albedo", 0.5f, 0.0f, 0.0f);
    manualPBR.setFloat("ao", 1.0f);
    manualPBR.use();
    manualPBR.setMat4("projection", projection);

    // DECLARE GEOMETRY
    Cube cube;
    Skybox skybox;
    Sphere sphere;

    // INIT IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    //IMGUI VARIABLES
    bool drawTriangle = true;
    float roughness = 0.5f;
    float metallic = 0.5f;


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

        //camera update
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //// be sure to activate shader when setting uniforms/drawing objects
        //lightingShader.use();
        //lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        //lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        //// view/projection transformation
        //lightingShader.setMat4("projection", projection);
        //lightingShader.setMat4("view", view);
        //// world transformation
        //lightingShader.setMat4("model", model);


        manualPBR.use();
        manualPBR.setMat4("view", view);
        manualPBR.setVec3("camPos", camera.Position);
         // we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
         // on direct lighting.
        manualPBR.setFloat("metallic",metallic);
        manualPBR.setFloat("roughness", roughness);
        model = glm::mat4(1.0f);
        manualPBR.setMat4("model", model);
        manualPBR.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
        // render the cube
        sphere.render();





        // render light source (simply re-render sphere at light positions)
       // this looks a bit off as we use the same shader, but it'll make their positions obvious and 
       // keeps the codeprint small.
        for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
        {
            glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
            newPos = lightPositions[i];
            manualPBR.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            manualPBR.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

            model = glm::mat4(1.0f);
            model = glm::translate(model, newPos);
            model = glm::scale(model, glm::vec3(0.5f));
            manualPBR.setMat4("model", model);
            manualPBR.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
            sphere.render();
        }

        // render skybox 
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        skybox.render();


        ImGui::Begin("ImGui Window");
        ImGui::Text("Text");
        ImGui::Checkbox("DrawTriangle", &drawTriangle);
        ImGui::SliderFloat("Rougness", &roughness, 0.0f, 1.0f);
        ImGui::SliderFloat("Metallic", &metallic, 0.05f, 1.0f);


        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // ImGui stop
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    cube.cleanUp();
    skybox.cleanUp();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(ROTATE_RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(ROTATE_RIGHT, deltaTime);
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if (!io.WantCaptureMouse) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
    
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}