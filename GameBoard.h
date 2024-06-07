#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>


class GameBoard
{
private:
	bool gameOver = false;
	bool gameWon = false;
	int width;
	int height;
	int mineCount;
	int remainingCells;
	int dir[5]{ 0,1,0,-1,0 };
	int highLightCell[2]{ -1,-1 };
	std::vector<std::vector<int>> board;
	std::vector<std::vector<bool>> isOpen;
	std::vector<std::vector<bool>> hasFlag;

	/// GraphicsItems
	sf::Texture closedCellTexture;
	sf::Texture mineTexture;
	sf::Texture flagTexture;

	sf::Sprite closedCell;
	sf::Sprite mine;
	sf::Sprite flag;

	sf::Font font;
	sf::Text textItem;
	sf::Text endGameText;

	///Sounds
	sf::Music music;
	sf::SoundBuffer successSoundBuffer;
	sf::Sound successSound;
	sf::SoundBuffer buzzerBuffer;
	sf::Sound buzzerSound;

	void fillMines();
	void revealMines();


	void emptyCellClicked(int x, int y);
public:

	inline const static int cellSize = 30;

	GameBoard(int width =20, int height=20, int count=20);

	void draw(sf::RenderWindow* w);

	inline sf::Vector2u getWindowSize(){return { (unsigned)width * cellSize,(unsigned)height * cellSize };}

	bool openCell(int y, int x);
	void setFlag(int y, int x);
	void highLight(int y, int x);


	void gameWin() { gameOver = true; gameWon = true; }
	void print() {
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				std::cout << board[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
	~GameBoard() {
		music.stop();
	}
};

