#include "OpeningScene.h"

OpeningScene::OpeningScene() :
	options{ "Easy 20x20 - 40","Medium 25x25 - 99","Hard 35x35 - 199" }
{
	font.loadFromFile("arial.ttf");
	textItem.setFont(font);
	textItem.setFillColor(sf::Color::Black);
	textItem.setCharacterSize(40);
}

void OpeningScene::draw(sf::RenderWindow* w)
{
	int distance = 50;
	for (int i = 0; i < options.size(); ++i)
	{
		textItem.setString(options[i]);
		textItem.setPosition(w->getSize().x / 2 - textItem.getLocalBounds().width / 2, distance + i * 100);		
		textItem.setFillColor(i == 0 ? sf::Color::Green : i == 1 ? sf::Color::Yellow : sf::Color::Red);
		textItem.setStyle(sf::Text::Regular);
		if (textItem.getGlobalBounds().contains(highLightPos[0], highLightPos[1]))
		{
			textItem.setStyle(sf::Text::Bold);
			currSelectedOpt = i;
		}
		w->draw(textItem);
	}
}

void OpeningScene::highlight(int x, int y)
{
	highLightPos[0] = x;
	highLightPos[1] = y;
}

std::array<int,3> OpeningScene::getSelectedOpt()
{
	if (currSelectedOpt == 0)
	{
		return { 20,20,40 };
	}
	else if (currSelectedOpt == 1)
	{
		return { 25,25,99 };
	}
	else
		return { 35,35,199 };
}




