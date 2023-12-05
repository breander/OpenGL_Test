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

#include "common/shader.hpp"

const int Width = 640;
const int Height = 480;

struct Vertex {
    float x, y, z;
    float nX, nY, nZ;
};

struct Normal{
    float x, y, z;
};

struct Face {
    unsigned int v1, v2, v3;
};

class ObjLoader {
public:
    ObjLoader(const std::string& filePath) {
        loadObj(filePath);
    }

    const std::vector<Vertex>& getVertices() const {
        return vertices;
    }

    const std::vector<Normal>& getVertexNormals() const{
        return normals;
    }

    const std::vector<Face>& getFaces() const {
        return faces;
    }

private:
    std::vector<Vertex> vertices;
    std::vector<Normal> normals;
    std::vector<Face> faces;

    void loadObj(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filePath << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string type;
            char junk;
            iss >> type;

            if (type == "v") {
                Vertex vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            }
            else if (type == "vn"){
                Normal normal;
                iss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            }
            else if (type == "f") {
                Face face;
                int n1, n2, n3;

                iss >> face.v1 >> junk >> junk >> n1 >> face.v2 >> junk >> junk >> n2 >> face.v3 >> junk >> junk >> n3;
                
                // OBJ file indices start from 1, but OpenGL indices start from 0
                vertices[face.v1-1].nX = normals[n1-1].x;
                vertices[face.v1-1].nY = normals[n1-1].y;
                vertices[face.v1-1].nZ = normals[n1-1].z;

                vertices[face.v2-1].nX = normals[n2-1].x;
                vertices[face.v2-1].nY = normals[n2-1].y;
                vertices[face.v2-1].nZ = normals[n2-1].z;

                vertices[face.v3-1].nX = normals[n3-1].x;
                vertices[face.v3-1].nY = normals[n3-1].y;
                vertices[face.v3-1].nZ = normals[n3-1].z;

                face.v1--; face.v2--; face.v3--;
                faces.push_back(face);
            }
        }

        file.close();
    }
};

GLuint createVertexBuffer(const std::vector<Vertex>& vertices) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    return vbo;
}

GLuint createElementBuffer(const std::vector<Face>& faces) {
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(Face), faces.data(), GL_STATIC_DRAW);
    return ebo;
}

void errorCallback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Error initializing GLFW" << std::endl;
        return -1;
    }

    // Set GLFW to throw errors
    glfwSetErrorCallback(errorCallback);

    // Set GLFW to use OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(Width, Height, "OpenGL OBJ Loader", NULL, NULL);
    if (!window) {
        std::cerr << "Error creating GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error initializing GLEW" << std::endl;
        return -1;
    }

    // Load OBJ file
    ObjLoader objLoader("../teapot.obj");

    // Create Vertex Array Object (VAO)
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create Vertex Buffer Object (VBO)
    GLuint vbo = createVertexBuffer(objLoader.getVertices());

    // Create Element Buffer Object (EBO)
    GLuint ebo = createElementBuffer(objLoader.getFaces());

    // Specify the layout of the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Specify the layout of the normal data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO, VBO, and EBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set up rotation angle
    float angle = 0.0f;

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Camera parameters
    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 6.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    float cameraSpeed = 0.05f;

    glm::vec3 modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    // Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "../SimpleVertexShader.vertexshader", "../SimpleFragmentShader.fragmentshader" );

    // Projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // View matrix
    glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

    // Set light properties as uniforms
    glm::vec3 light_position = glm::vec3(0.0f, 3.0f, 2.0f);
    glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);

    // Set object color as uniform
    glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.0f);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader program
        glUseProgram(programID);

        // Bind the VAO
        glBindVertexArray(vao);

        // Rotate the object
        angle += 0.001f;
        if (angle > 360.0f) {
            angle -= 360.0f;
        }

        // Process input
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraPosition += cameraSpeed * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraPosition -= cameraSpeed * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            cameraPosition += cameraUp * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            cameraPosition -= cameraUp * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
            cameraFront -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
            cameraFront += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            light_position.x -= cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            light_position.x += cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            light_position.z -= cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            light_position.z += cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
            light_position.y -= cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
            light_position.y += cameraSpeed;
        }

        // Update view matrix
        view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

        glm::vec3 myRotationAxis( 0.0f, 1.0f, 0.0f);
        glm::mat4 rotationMatrix = glm::rotate( angle, myRotationAxis );

        // Set the transformation matrix
        GLint modelLoc = glGetUniformLocation(programID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotationMatrix));

         // Set the view and projection matrices as uniforms in your shader program
        GLint viewLoc = glGetUniformLocation(programID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        GLint projectionLoc = glGetUniformLocation(programID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Set light properties as uniforms
        glUniform3fv(glGetUniformLocation(programID, "light.position"), 1, glm::value_ptr(light_position));
        glUniform3fv(glGetUniformLocation(programID, "light.color"), 1, glm::value_ptr(light_color));

        // Set object color as uniform
        glUniform3fv(glGetUniformLocation(programID, "objectColor"), 1, glm::value_ptr(objectColor));

        // Draw the object
        glDrawElements(GL_TRIANGLES, objLoader.getFaces().size() * 3, GL_UNSIGNED_INT, 0);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    // Close GLFW
    glfwTerminate();

    return 0;
}
