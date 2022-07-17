#pragma once
class GUI {
public:
	void drawGUI();
	void clickEvent(double posX, double posY, bool rightClick = false);

	static bool FindPoint(int x1, int y1, int x2, int y2, int x, int y);

	bool checkCollision(double posX, double posY);
	void positionEvent(double posX, double posY);

};