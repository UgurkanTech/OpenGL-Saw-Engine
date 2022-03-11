#pragma once
#include "Textures.h"
#include "shader.h"
#include <vector>
#include <string>

using namespace std;
class TextureManager {
public:
    vector<Textures*> textures;

    Textures* nullTexture;
    Shader* nullShader;
    Textures* getTexture(string name);

    void loadNullTextures();

    Textures* addTextures(string name, string path);

    unsigned int getNullTex();

    unsigned int loadTexture(char const* path);
};
