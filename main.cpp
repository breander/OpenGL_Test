#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex {
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

    const std::vector<Face>& getFaces() const {
        return faces;
    }

private:
    std::vector<Vertex> vertices;
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
            iss >> type;

            if (type == "v") {
                Vertex vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            } else if (type == "f") {
                Face face;
                iss >> face.v1 >> face.v2 >> face.v3;
                // OBJ file indices start from 1, but OpenGL indices start from 0
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL OBJ Loader", NULL, NULL);
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
    ObjLoader objLoader("../cube.obj");

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

    // Unbind VAO, VBO, and EBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set up rotation angle
    float angle = 0.0f;

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Camera parameters
    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    float cameraSpeed = 0.05f;

    // Projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // View matrix
    glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader program
        glUseProgram(0);

        // Bind the VAO
        glBindVertexArray(vao);

        // Rotate the object
        angle += 0.01f;
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

        // Update view matrix
        view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);


        // Set the transformation matrix
        GLint modelLoc = glGetUniformLocation(0, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, nullptr);

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
