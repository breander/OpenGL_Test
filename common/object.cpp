#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "objloader.hpp"

#include "object.hpp"

Object::Object(const std::string& filePath) : objLoader(filePath) {
    
}