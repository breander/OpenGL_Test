#version 330 core

out vec4 FragColor;

struct Light {
    vec3 position;
    vec3 color;
};

in vec3 FragPos;    // Fragment position in world coordinates
in vec3 Normal;     // Normal vector in world coordinates

uniform Light light;
uniform vec3 objectColor;
uniform float intensity;

void main() {
    // Lambertian reflection model
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    // Ambient light (you can adjust the ambient coefficient)
    vec3 ambient = intensity * objectColor;  // Adjust the ambient coefficient as needed

    // Final color = Ambient + Diffuse
    vec3 result = (ambient + diffuse) * objectColor;

    //FragColor = vec4(result, 1.0);
	FragColor = vec4(result, 1.0);
}
