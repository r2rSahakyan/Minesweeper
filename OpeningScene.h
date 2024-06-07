#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
class OpeningScene
{
private:
	sf::Font font;
	sf::Text textItem;
	int currSelectedOpt = -1;
	int highLightPos[2]{ -1,-1 };
	std::vector<std::string> options;
public:
	OpeningScene();
	void draw(sf::RenderWindow* w);
	void highlight(int y, int x);
	std::array<int, 3>  getSelectedOpt();
};

