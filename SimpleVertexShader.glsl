#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 aNormal;

out vec3 FragPos;   // Pass fragment position to fragment shader
out vec3 Normal;    // Pass normal vector to fragment shader

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main(){

    // Transform vertex position to world coordinates
    FragPos = mat3(transpose(inverse(model))) * vertexPosition_modelspace;

    // Transform normal to world coordinates (no translation)
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    gl_Position = projection * view * model * vec4(vertexPosition_modelspace, 1.0);
}
