#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"

#include "vertex.hpp"
#include "normal.hpp"
#include "face.hpp"
#include "object.hpp"
#include "light.hpp"
#include "shader.hpp"
#include "lvlloader.hpp"

LvlLoader::LvlLoader(const std::string& filePath) {
    loadLevel(filePath);
}

const std::string& LvlLoader::getName() const {
    return _name;
}

std::vector<Object>& LvlLoader::getObjects() {
    return _objects;
}

std::vector<Light>& LvlLoader::getLights() {
    return _lights;
}

GLuint LvlLoader::createVertexBuffer(const std::vector<Vertex>& vertices) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    return vbo;
}

GLuint LvlLoader::createElementBuffer(const std::vector<Face>& faces) {
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(Face), faces.data(), GL_STATIC_DRAW);
    return ebo;
}

void LvlLoader::loadLevel(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    // Read the entire file into a string 
    std::string json((std::istreambuf_iterator<char>(file)), 
                std::istreambuf_iterator<char>()); 
  
    // Create a Document object  
      // to hold the JSON data 
    rapidjson::Document doc; 
  
    // Parse the JSON data 
    doc.Parse(json.c_str()); 
  
    // Check for parse errors 
    if (doc.HasParseError()) { 
        std::cerr << "Error parsing JSON: "
             << doc.GetParseError() << std::endl; 
        return;
    }

    rapidjson::Value& jName = doc["Name"];
    rapidjson::Value& jModels = doc["Models"];
    rapidjson::Value& lights = doc["Lights"];

    for (rapidjson::SizeType i = 0; i < lights.Size(); i++) {
        rapidjson::Value& light = lights[i];
        glm::vec3 position = glm::vec3(
            light["Position"][0].GetFloat(),
            light["Position"][1].GetFloat(),
            light["Position"][2].GetFloat()
        );
        glm::vec3 color = glm::vec3(
            light["Color"][0].GetFloat(),
            light["Color"][1].GetFloat(),
            light["Color"][2].GetFloat()
        );
        float intensity = light["Intensity"].GetFloat();
        Light theLight = Light();
        theLight.position = position;
        theLight.color = color;
        theLight.intensity = intensity;
        _lights.push_back(theLight);
    }
    
    _name = jName.GetString();
    rapidjson::Value& models = jModels.GetArray();

    for (rapidjson::SizeType i = 0; i < models.Size(); i++) {
        rapidjson::Value& model = models[i];
        std::string fileName = model["FileName"].GetString();
        std::string fragmentShader = model["FragmentShader"].GetString();
        std::string vertexShader = model["VertexShader"].GetString();
        float x = model["LocationX"].GetFloat();
        float y = model["LocationY"].GetFloat();
        float z = model["LocationZ"].GetFloat();
        float angle = model["Angle"].GetFloat();
        bool load = model["LoadObject"].GetBool();
        glm::vec3 color = glm::vec3(
            model["Color"][0].GetFloat(),
            model["Color"][1].GetFloat(),
            model["Color"][2].GetFloat()
        );

        if (!load) {
            std::cerr << "Skipped Model: " << fileName << " x: " << x << " y: " << y << std::endl;
            continue;
        }

        std::cerr << "Loaded Model: " << fileName << " x: " << x << " y: " << y << std::endl;

        // Create Vertex Array Object (VAO)
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // new Object
        Object object(fileName);
        object.vao = vao;
        object.locationX = x;
        object.locationY = y;
        object.locationZ = z;
        object.angle = angle;

        // Create Vertex Buffer Object (VBO)
        GLuint vbo = createVertexBuffer(object.objLoader.getVertices());
        object.vbo = vbo;

        // Create Element Buffer Object (EBO)
        GLuint ebo = createElementBuffer(object.objLoader.getFaces());
        object.ebo = ebo;

        // Specify the layout of the vertex data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        // Specify the layout of the normal data
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Unbind VAO, VBO, and EBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // load the shaders
        object.fragmentShader = fragmentShader;
        object.vertexShader = vertexShader;

        // Create and compile our GLSL program from the shaders
	    object.programID = LoadShaders(vertexShader.c_str(), fragmentShader.c_str());
        if (object.programID == 0) {
            std::cerr << "Error loading shaders for model: " << fileName << std::endl;
            continue;
        }

        std::cerr << "Loaded shader programId: " << object.programID 
                  << " for model: " << fileName << std::endl;

        // Set the color
        object.color = color;

        // Store the VAO in the objects vector
        _objects.push_back(object);
    }
}

void LvlLoader::destroyObjects() {
    for (Object& object : _objects) {
        glDeleteVertexArrays(1, &object.vao);
        glDeleteBuffers(1, &object.vbo);
        glDeleteBuffers(1, &object.ebo);
    }
}
