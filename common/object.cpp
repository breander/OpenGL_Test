#include <string>
#include <GL/glew.h>

#include "objloader.hpp"

#include "object.hpp"

Object::Object(const std::string& filePath) : objLoader(filePath) {
    
}