#include <vector>
#include "vertex.hpp"
#include "normal.hpp"
#include "face.hpp"

#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

class ObjLoader {
public:
    ObjLoader(const std::string& filePath);

    const std::vector<Vertex>& getVertices() const;

    const std::vector<Normal>& getVertexNormals() const;
    
    const std::vector<Face>& getFaces() const;

private:
    std::vector<Vertex> vertices;
    std::vector<Normal> normals;
    std::vector<Face> faces;

    void loadObj(const std::string& filePath);
};

#endif