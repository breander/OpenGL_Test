#ifndef LVLLOADER_HPP
#define LVLLOADER_HPP

class LvlLoader {
public:
    LvlLoader(const std::string& filePath);
    const std::string& getName() const;
    const std::vector<Object>& getObjects() const;          // const version
    std::vector<Object>& getObjects();                      // non-const version (add this)
    const std::vector<Light>& getLights() const;
    std::vector<Light>& getLights();                        // non-const version (add this)
    void destroyObjects();
private:
    std::vector<Object> _objects;
    std::vector<Light> _lights;
    std::string _name;

    GLuint createVertexBuffer(const std::vector<Vertex>& vertices);
    GLuint createElementBuffer(const std::vector<Face>& faces);
    void loadLevel(const std::string& filePath);
};

#endif