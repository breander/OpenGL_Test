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


const int Width = 640;
const int Height = 480;

void errorCallback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

GLFWwindow* initialize(){
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Error initializing GLFW" << std::endl;
        return nullptr;
        //return -1;
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
    GLFWwindow* window = glfwCreateWindow(Width, Height, "OpenGL_Test", NULL, NULL);
    if (!window) {
        std::cerr << "Error creating GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
        //return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error initializing GLEW" << std::endl;
        return nullptr;
        //return -1;
    }

    return window;
}

int main() {
    // Initialize window
    GLFWwindow* window = initialize();

    if(!window){
        return -1;
    }

    // Load level file
    LvlLoader lvlLoader("../Level_01.json");
    std::cerr << "Loaded Level: " << lvlLoader.getName() << std::endl;

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Camera parameters
    Camera camera = Camera();

    // Projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)Width / (float)Height, 0.1f, 100.0f);

    // View matrix
    glm::mat4 view = camera.getLookAt();

    // Set light properties as uniforms
    //glm::vec3 light_position = glm::vec3(0.0f, 3.0f, 2.0f);
    //glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);

    // Set light intensity
    //glm::float32 intensity = 1.0f;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Process input
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.moveForward();
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.moveBackward();
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.moveLeft();
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.moveRight();
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            camera.moveUp();
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            camera.moveDown();
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
            camera.rotateLeft();
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
            camera.rotateRight();
        }

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
        for (Object object : lvlLoader.getObjects()) {
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

            for (Light light : lvlLoader.getLights()){
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

            glm::vec3 myRotationAxis( 0.0f, 1.0f, 0.0f);
            glm::mat4 rotationMatrix = glm::rotate( object.angle, myRotationAxis );
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

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up
    lvlLoader.destroyObjects();
 
    // Close GLFW
    glfwTerminate();

    return 0;
}
