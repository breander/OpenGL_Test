#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "vertex.hpp"
#include "normal.hpp"
#include "face.hpp"

#include "objloader.hpp"

ObjLoader::ObjLoader(const std::string& filePath) {
    loadObj(filePath);
}

const std::vector<Vertex>& ObjLoader::getVertices() const {
    return vertices;
}

const std::vector<Normal>& ObjLoader::getVertexNormals() const{
    return normals;
}

const std::vector<Face>& ObjLoader::getFaces() const {
    return faces;
}

void ObjLoader::loadObj(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        char junk;
        iss >> type;

        if (type == "v") {
            Vertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (type == "vn"){
            Normal normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (type == "f") {
            Face face;
            int n1, n2, n3;

            iss >> face.v1 >> junk >> junk >> n1 >> face.v2 >> junk >> junk >> n2 >> face.v3 >> junk >> junk >> n3;
            
            // OBJ file indices start from 1, but OpenGL indices start from 0
            vertices[face.v1-1].nX = normals[n1-1].x;
            vertices[face.v1-1].nY = normals[n1-1].y;
            vertices[face.v1-1].nZ = normals[n1-1].z;

            vertices[face.v2-1].nX = normals[n2-1].x;
            vertices[face.v2-1].nY = normals[n2-1].y;
            vertices[face.v2-1].nZ = normals[n2-1].z;

            vertices[face.v3-1].nX = normals[n3-1].x;
            vertices[face.v3-1].nY = normals[n3-1].y;
            vertices[face.v3-1].nZ = normals[n3-1].z;

            face.v1--; face.v2--; face.v3--;
            faces.push_back(face);
        }
    }

    file.close();
}