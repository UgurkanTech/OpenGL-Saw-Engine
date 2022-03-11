#pragma once
#include <string>
using namespace std;
typedef enum { TEXTURE, SPRITE, HDRBACKGROUND} AssetType;
struct Asset {
	AssetType assetType;
	string assetName, assetPath;
	Asset(AssetType type, string name, string path)
	{
		assetType = type;
		assetName = name;
		assetPath = path;
	}
	void test() {
		//cout << "Area of Rectangle is: " << endl;
	}
};