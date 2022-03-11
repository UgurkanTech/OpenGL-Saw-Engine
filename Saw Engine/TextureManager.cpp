#include "TextureManager.h"
#include "filesystem.h"
#include <stb_image.h>

Textures* TextureManager::getTexture(string name) {
    if (name.empty()) return nullTexture;
    for (size_t i = 0; i < textures.size(); i++)
        if (textures.at(i)->name == name)
            return textures.at(i);
    return nullTexture;
}

void TextureManager::loadNullTextures() {
    Textures* tex = new Textures;

    unsigned int n = getNullTex();
    tex->name = "NULL";
    tex->albedoMap = n;
    tex->normalMap = NULL;
    tex->metallicMap = NULL;
    tex->roughnessMap = NULL;
    tex->AOMap = NULL;
    tex->loaded = true;
    nullTexture = tex;
    cout << "Texture null loaded." << endl;

}


Textures* TextureManager::addTextures(string name, string path) {
    Textures* tex = new Textures;
    tex->name = name;
    if (path.substr(path.length() - 4) == ".png") {
        tex->albedoMap = loadTexture(FileSystem::getPath(path).c_str());
    }
    else {
        tex->albedoMap = loadTexture(FileSystem::getPath(path + "/albedo.png").c_str());
        tex->normalMap = loadTexture(FileSystem::getPath(path + "/normal.png").c_str());
        tex->metallicMap = loadTexture(FileSystem::getPath(path + "/metallic.png").c_str());
        tex->roughnessMap = loadTexture(FileSystem::getPath(path + "/roughness.png").c_str());
        tex->AOMap = loadTexture(FileSystem::getPath(path + "/ao.png").c_str());
    }
    tex->loaded = true;
    textures.push_back(tex);
    cout << "Texture " << name << " loaded." << endl;
    return tex;
};

unsigned int TextureManager::getNullTex() {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int nrComponents = 3;
    unsigned int size = 32;
    glm::vec3 color(0, 0, 0);
    unsigned char* data = new unsigned char[3 * size * size * sizeof(unsigned char)];
    for (unsigned int i = 0; i < size * size; i++)
    {
        data[i * 3] = (unsigned char)(color.x * 255.0f);
        data[i * 3 + 1] = (unsigned char)(color.y * 255.0f);
        data[i * 3 + 2] = (unsigned char)(color.z * 255.0f);
    }
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, size, size, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load null Texture" << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int TextureManager::loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}