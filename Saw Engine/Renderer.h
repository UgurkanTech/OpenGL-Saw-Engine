#pragma once
#include <glm/glm.hpp>
#include <map>
#include <string>
using namespace std;
// renders (and builds at first invocation) a sphere
// -------------------------------------------------
class Shader;
typedef char GLchar;
enum class Shape { Cube, Quad, Sphere, Mesh, BillBoard };
class Renderer {
private:
    unsigned int sphereVAO = 0;
    unsigned int indexCount;

    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;

    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    unsigned int indexCountQuad;

public:
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };
    unsigned int VAO, VBO;
    std::map<GLchar, Character> Characters;
    int characterHeight;

    void renderSprite(Shader* shader, std::string texture, float x, float y, float sizeX, float sizeY, glm::vec4 color);
    void renderSphere();
    void renderCube();
    void renderQuad();
    void renderText(Shader* shader, std::string text, float x, float y, float scale, glm::vec3 color);
};

