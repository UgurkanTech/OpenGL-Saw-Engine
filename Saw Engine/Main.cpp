#include "GLEngine.h"

#include <iostream>
#include "Scene_01.cpp"
using namespace std;
int main() {
	GLEngine gl;

	Scene* s = new Scene_01();

	gl.initEngine(s);
}