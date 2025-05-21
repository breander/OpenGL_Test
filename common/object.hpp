#include "objloader.hpp"

#ifndef OBJECT_HPP
#define OBJECT_HPP

class Object {
public:
    Object(const std::string& filePath);
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    float locationX;
    float locationY;
    float locationZ;
    ObjLoader objLoader;
private:
};

#endif