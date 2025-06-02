#include <glm/glm.hpp>

#ifndef LIGHT_HPP
#define LIGHT_HPP

class Light {
public:
    glm::vec3 position; // Position of the light in world coordinates
    glm::vec3 color;    // Color of the light (RGB)
    float intensity;    // Intensity of the light
private:
};

#endif