#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "common/object.hpp"
#include "common/light.hpp"
#include "common/camera.hpp"

#include "common/lvlloader.hpp"

const int Width = 1280;
const int Height = 720;

// Camera parameters
Camera camera = Camera();

float lastX = Width / 2.0f;
float lastY = Height / 2.0f;
bool firstMouse = true;

void errorCallback(int error, const char *description)
{
    std::cerr << "Error: " << description << std::endl;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    static float yaw = -90.0f;
    static float pitch = 0.0f;
    static float sensitivity = 0.1f;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed: y ranges bottom to top
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // Update camera direction
    camera.setYawPitch(yaw, pitch);
}

GLFWwindow *initialize()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Error initializing GLFW" << std::endl;
        return nullptr;
        // return -1;
    }

    // Set GLFW to throw errors
    glfwSetErrorCallback(errorCallback);

    // Set anti alaising
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Set GLFW to use OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(Width, Height, "OpenGL_Test", NULL, NULL);
    if (!window)
    {
        std::cerr << "Error creating GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
        // return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Error initializing GLEW" << std::endl;
        return nullptr;
        // return -1;
    }

    return window;
}

int main()
{
    // Initialize window
    GLFWwindow *window = initialize();

    if (!window)
    {
        return -1;
    }

    // Register mouse callback
    glfwSetCursorPosCallback(window, mouse_callback);
    // Optionally, hide and capture the cursor
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    const char *glsl_version = "#version 330";

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Imgui variables
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Load level file
    LvlLoader lvlLoader("../Level_01.json");
    std::cerr << "Loaded Level: " << lvlLoader.getName() << std::endl;

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)Width / (float)Height, 0.1f, 100.0f);

    // View matrix
    glm::mat4 view = camera.getLookAt();

    // Set light properties as uniforms
    // glm::vec3 light_position = glm::vec3(0.0f, 3.0f, 2.0f);
    // glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);

    // Set light intensity
    // glm::float32 intensity = 1.0f;

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear the buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            // Get current window size
            ImVec2 windowSize = ImGui::GetWindowSize();
            ImGui::Text("Window size: %.1f x %.1f", windowSize.x, windowSize.y);

            // Get window position
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImGui::Text("Window pos: %.1f, %.1f", windowPos.x, windowPos.y);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Process input
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera.moveForward();
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.moveBackward();
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.moveLeft();
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.moveRight();
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            camera.moveUp();
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            camera.moveDown();
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            camera.rotateLeft();
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            camera.rotateRight();
        }

        // Rendering
        ImGui::Render();

        /* if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            light_position.x -= camera.getSpeed();
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            light_position.x += camera.getSpeed();
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            light_position.z -= camera.getSpeed();
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            light_position.z += camera.getSpeed();
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
            light_position.y -= camera.getSpeed();
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
            light_position.y += camera.getSpeed();
        }

        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
            intensity -= camera.getSpeed();
        }
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
            intensity += camera.getSpeed();
        } */

        // Update view matrix
        view = camera.getLookAt();

        // for each object in the level
        for (Object object : lvlLoader.getObjects())
        {
            // Use the shader program
            glUseProgram(object.programID);

            // Set the view and projection matrices as uniforms in your shader program
            GLint viewLoc = glGetUniformLocation(object.programID, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

            GLint projectionLoc = glGetUniformLocation(object.programID, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

            int lightCount = 0;
            int numLights = lvlLoader.getLights().size();
            glUniform1i(glGetUniformLocation(object.programID, "numLights"), numLights);
            glUniform3fv(glGetUniformLocation(object.programID, "objectColor"), 1, glm::value_ptr(object.color));

            for (Light light : lvlLoader.getLights())
            {
                // Set light properties as uniforms
                std::string base = "lights[" + std::to_string(lightCount) + "]";
                glUniform3fv(glGetUniformLocation(object.programID, (base + ".position").c_str()), 1, glm::value_ptr(light.position));
                glUniform3fv(glGetUniformLocation(object.programID, (base + ".color").c_str()), 1, glm::value_ptr(light.color));

                // Set object color as uniform
                glUniform1f(glGetUniformLocation(object.programID, (base + ".intensity").c_str()), light.intensity);
                lightCount++;
            }

            // Bind the VAO
            glBindVertexArray(object.vao);

            glm::vec3 myRotationAxis(0.0f, 1.0f, 0.0f);
            glm::mat4 rotationMatrix = glm::rotate(object.angle, myRotationAxis);
            glm::vec3 translation(object.locationX, object.locationY, object.locationZ);
            glm::mat4 translationMatrix = glm::translate(translation);
            glm::vec3 scale(1.0f, 1.0f, 1.0f);
            glm::mat4 scaleMatrix = glm::scale(scale);

            // Set the transformation matrix
            GLint modelLoc = glGetUniformLocation(object.programID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(translationMatrix * rotationMatrix * scaleMatrix));

            ObjLoader objLoader = object.objLoader;

            // Draw the object
            glDrawElements(GL_TRIANGLES, objLoader.getFaces().size() * 3, GL_UNSIGNED_INT, 0);
        }

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        // glfwPollEvents();
    }

    // Clean up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    lvlLoader.destroyObjects();

    // Close GLFW
    glfwTerminate();

    return 0;
}
