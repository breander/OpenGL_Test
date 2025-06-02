#include "objloader.hpp"
#include <glm/glm.hpp>

#ifndef OBJECT_HPP
#define OBJECT_HPP

class Object {
public:
    Object(const std::string& filePath);
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint programID;
    std::string fragmentShader;
    std::string vertexShader;
    float locationX;
    float locationY;
    float locationZ;
    float angle;
    glm::vec3 color;
    ObjLoader objLoader;
private:
};

#endif