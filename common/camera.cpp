#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"

Camera::Camera(){
    _position = glm::vec3(0.0f, 0.0f, 6.0f);
    _up = glm::vec3(0.0f, 1.0f, 0.0f);
    _front = glm::vec3(0.0f, 0.0f, -1.0f);
    _speed = 0.05f;
}

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front, float speed){
    _position = position;
    _up = up;
    _front = front;
    _speed = speed;
}

glm::vec3 Camera::getPosition(){
    return _position;
}

glm::vec3 Camera::getUp(){
    return _up;
}

glm::vec3 Camera::getFront(){
    return _front;
}

float Camera::getSpeed(){
    return _speed;
}

glm::mat4 Camera::getLookAt(){
    return glm::lookAt(_position, _position + _front, _up);
}

void Camera::moveForward(){
    _position += _speed * _front;
}

void Camera::moveBackward(){
    _position -= _speed * _front;
}

void Camera::moveLeft(){
    _position -= glm::normalize(glm::cross(_front, _up)) * _speed;
}

void Camera::moveRight(){
    _position += glm::normalize(glm::cross(_front, _up)) * _speed;
}

void Camera::moveUp(){
    _position += _up * _speed;
}

void Camera::moveDown(){
    _position -= _up * _speed;
}

void Camera::rotateLeft(){
    _front -= glm::normalize(glm::cross(_front, _up)) * _speed;
}

void Camera::rotateRight(){
    _front += glm::normalize(glm::cross(_front, _up)) * _speed;
}