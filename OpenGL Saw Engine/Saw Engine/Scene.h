#ifndef SCENE
#define SCENE

#include <vector>
#include "Transform.h"
#include "LightSource.h"
#include "Asset.h"

class Scene {

public:
	int sceneID;
	float* HDRBackgroundData;
	int HDRBackgroundWidth, HDRBackgroundHeight, HDRBackgroundNrComponents;
	const char* HDRBackgroundPath;

	vector<Transform*> transforms;
	vector<LightSource*> lightSources;
	vector<Asset> assetNames;
	virtual void loadResources() {}
	virtual void initScene() {}
	void viewLoadScreen(int percentage, string text = "Loading - ");
	virtual void initLoop(){}
	virtual void fixedGameLoop(){}
	virtual void gameLoop(){}
	void loadTextureNames(bool viewLoadingScreen = true);
	
};
#endif