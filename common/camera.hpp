#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera {
public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front, float speed);

    glm::vec3 getPosition();
    glm::vec3 getUp();
    glm::vec3 getFront();

    glm::mat4 getLookAt();

    float getSpeed();
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void rotateLeft();
    void rotateRight();
private:
    glm::vec3 _position;
    glm::vec3 _up;
    glm::vec3 _front;
    float _speed;
};

#endif