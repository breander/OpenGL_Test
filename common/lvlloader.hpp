#ifndef LVLLOADER_HPP
#define LVLLOADER_HPP

class LvlLoader {
public:
    LvlLoader(const std::string& filePath);
    const std::string& getName() const;
private:
    std::vector<GLuint> objects;
    std::string _name;

    void loadLevel(const std::string& filePath);
};

#endif