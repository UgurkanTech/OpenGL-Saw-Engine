#include "Scene.h"
#include "GLEngine.h"
class Scene_01 : public Scene {
public:
	
	void loadResources() {
		assetNames.push_back(Asset(TEXTURE, "gold", "resources/textures/pbr/gold"));
		//assetNames.push_back(Asset(TEXTURE, "cobble", "resources/textures/pbr/cobble"));
		//assetNames.push_back(Asset(TEXTURE, "iron", "resources/textures/pbr/rusted_iron"));
		//assetNames.push_back(Asset(TEXTURE, "grass", "resources/textures/pbr/grass"));
		assetNames.push_back(Asset(TEXTURE, "plastic", "resources/textures/pbr/plastic"));
		assetNames.push_back(Asset(TEXTURE, "wall", "resources/textures/pbr/wall"));
		//textureNames.push_back(Asset(TEXTURE, "bricks", "resources/textures/pbr/bricks"));

		//textureNames.push_back(Asset(TEXTURE, "diamond", "resources/textures/pbr/diamond"));
		//textureNames.push_back(Asset(TEXTURE, "oak", "resources/textures/pbr/oak"));
		assetNames.push_back(Asset(TEXTURE, "button", "resources/textures/button.png"));

		HDRBackgroundPath = "resources/textures/hdr/lilienstein_2k.hdr";

		loadTextureNames(true);
		

		cout << GLEngine::tm->textures.size() << " textures loaded before start." << endl;

		initScene();
	}

	void initScene() {
		for (size_t i = 0; i < 20; i++)
		{
			for (size_t j = 0; j < 20; j++)
			{
				//this->transforms.push_back(new Transform(GLEngine::shaders.pbrShader, assetNames.at(rand() % 3).assetName, Shape::Cube, glm::vec3(static_cast<double>(i) * -2, static_cast<double>(j) * -2, -10)));
				return;
			}
		}

		for (size_t i = 0; i < 6; i++)
		{
			this->lightSources.push_back(new LightSource(glm::vec3(static_cast<double>(i) * 5, 0, 5), glm::vec3(300, 300, 300)));
		}
	}

	void fixedGameLoop() {
			//for (size_t i = 0; i < this->transforms.size(); i++)
				//this->transforms.at(i)->translateModel(glm::vec3(0, 0, glm::sin(glfwGetTime() + i / 0.5) / 30));
	}
};