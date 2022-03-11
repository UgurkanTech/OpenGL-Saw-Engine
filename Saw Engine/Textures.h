#pragma once
#include <string>
using namespace std;
class Textures {
public:
	string name;
	unsigned int albedoMap;
	unsigned int normalMap;
	unsigned int metallicMap;
	unsigned int roughnessMap;
	unsigned int AOMap;
	bool loaded = false;
};