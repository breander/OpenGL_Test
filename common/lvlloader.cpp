#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include "lvlloader.hpp"

#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"

LvlLoader::LvlLoader(const std::string& filePath) {
    loadLevel(filePath);
}

const std::string& LvlLoader::getName() const {
    return name;
};

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

    rapidjson::Value& s = doc["Name"];
    
    name = s.GetString();
}