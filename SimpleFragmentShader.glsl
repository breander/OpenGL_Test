#version 330 core

out vec4 FragColor;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

in vec3 FragPos;
in vec3 Normal;

#define NUM_LIGHTS 4
uniform Light lights[NUM_LIGHTS];
uniform int numLights;
uniform vec3 objectColor;

void main() {
    vec3 norm = normalize(Normal);
    vec3 result = vec3(0.0);

    for (int i = 0; i < numLights; ++i) {
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color;
        vec3 ambient = lights[i].intensity * objectColor;
        result += (ambient + diffuse) * objectColor;
    }

    FragColor = vec4(result, 1.0);
}
