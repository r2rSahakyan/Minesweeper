#include "GameBoard.h"
#include <numeric>


GameBoard::GameBoard(int width, int height, int count) :width(width),
height(height),
mineCount(count),
remainingCells(width* height - count),
board(height, std::vector<int>(width, 0)),
isOpen(height, std::vector<bool>(width, false)),
hasFlag(height, std::vector<bool>(width, false))
{
	fillMines();
	closedCellTexture.loadFromFile("closedCellTexture.png");
	mineTexture.loadFromFile("mine.png");
	flagTexture.loadFromFile("flag.png");

	closedCell.setTexture(closedCellTexture);
	mine.setTexture(mineTexture);
	flag.setTexture(flagTexture);

	font.loadFromFile("arial.ttf");
	textItem.setFont(font);
	textItem.setCharacterSize(20);
	textItem.setStyle(sf::Text::Bold);

	endGameText.setFont(font);
	endGameText.setString("Game Over");
	endGameText.setCharacterSize(50);	
	endGameText.setStyle(sf::Text::Bold);
	endGameText.setFillColor(sf::Color::Green);

	music.openFromFile("music.ogg");
	music.setLoop(true);
	music.setVolume(50.0f);
	music.play();

	successSoundBuffer.loadFromFile("success.mp3");
	successSound.setBuffer(successSoundBuffer);
	
	buzzerBuffer.loadFromFile("buzzer.mp3");
	buzzerSound.setBuffer(buzzerBuffer);
	buzzerSound.setPlayingOffset(sf::milliseconds(600));

}

void GameBoard::fillMines()
{
	std::vector<int> candidates(width * height);
	std::iota(candidates.begin(), candidates.end(), 0);
	auto begin = candidates.begin();
	int countToChoose = mineCount;
	int sz = candidates.size();

	srand(unsigned(time(NULL)));
	while (countToChoose--)
	{
		auto it = begin;
		std::advance(it, rand() % sz);
		std::swap(*it, *begin);
		begin++;
		sz--;
	}
	for (int i = 0; i < mineCount; ++i)
	{
		int x = candidates[i] / width;
		int y = candidates[i] % width;
		board[x][y] = -1;
		for (int nx = x - 1; nx <= x + 1; ++nx)
		{
			for (int ny = y - 1; ny <= y + 1; ++ny)
			{
				if (nx >= 0 && ny >= 0 && nx < height && ny < width && board[nx][ny] != -1)
					board[nx][ny]++;
			}
		}
	}
}

void GameBoard::revealMines()
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (board[i][j] == -1)
				isOpen[i][j] = true;
		}
	}
	gameOver = true;
}

void GameBoard::emptyCellClicked(int x, int y)
{
	if (isOpen[x][y])
		return;
	isOpen[x][y] = true;
	--remainingCells;
	if (remainingCells == 0)
	{
		gameWin();
		return;
	}
	if (board[x][y] != 0)
		return;
	for (int d = 0; d < 4; ++d)
	{
		int nx = x + dir[d];
		int ny = y + dir[d + 1];
		if (nx >= 0 && ny >= 0 && nx < height && ny < width && board[x][y] != -1)
			emptyCellClicked(nx, ny);
	}
}

void GameBoard::draw(sf::RenderWindow* w)
{

	if (gameWon)
	{
		std::string s = "YOU WON!";
		endGameText.setString(s);
		endGameText.setPosition(w->getSize().x / 2 - endGameText.getLocalBounds().width / 2, w->getSize().y / 2 - endGameText.getLocalBounds().height / 2);
		w->draw(endGameText);
		return;
	}
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (!isOpen[i][j])
			{
				closedCell.setPosition(j * cellSize, i * cellSize);
				closedCell.setColor(sf::Color::White);
				if (i == highLightCell[0] && j == highLightCell[1])
				{
					closedCell.setColor(sf::Color(255, 255, 255, 128));
				}
				w->draw(closedCell);
				if (hasFlag[i][j])
				{
					flag.setPosition(j * cellSize, i * cellSize);
					w->draw(flag);
				}
				
			}
			else
			{
				closedCell.setPosition(j * cellSize, i * cellSize);
				closedCell.setColor(sf::Color(255, 255, 255, 50));
				w->draw(closedCell);
				if (board[i][j] == -1)
				{
					mine.setPosition(j * cellSize, i * cellSize);
					if (gameOver)
						mine.setColor(sf::Color(255, 0, 0, 200));
					w->draw(mine);
				}
				if (board[i][j] > 0)
				{
					textItem.setString(std::string(1, '0' + board[i][j]));
					textItem.setPosition(j * cellSize + 12 - textItem.getLocalBounds().width / 2, i * cellSize + 12 - textItem.getLocalBounds().height / 2);
					sf::Color color;
					switch (board[i][j])
					{
					case 1:
						color = sf::Color::Blue;
						break;
					case 2:
						color = sf::Color::Green;
						break;
					case 3:
						color = sf::Color::Red;
						break;
					case 4:
						color = sf::Color::Magenta;
						break;
					default:
						color = sf::Color::Black;
					}
					textItem.setFillColor(color);
					w->draw(textItem);
				}
			}
			
		}
		if (gameOver)
		{
			endGameText.setPosition(w->getSize().x / 2 - endGameText.getLocalBounds().width / 2, w->getSize().y / 2 - endGameText.getLocalBounds().height / 2);
			endGameText.setFillColor(sf::Color::Red);
			w->draw(endGameText);
		}
	}
}

void GameBoard::setFlag(int y, int x)
{
	x /= cellSize;
	y /= cellSize;
	if (isOpen[x][y])
		return;
	hasFlag[x][y] = !hasFlag[x][y];
}

void GameBoard::highLight(int y, int x)
{
	x /= cellSize;
	y /= cellSize;
	highLightCell[0] = x;
	highLightCell[1] = y;
}

bool GameBoard::openCell(int y, int x)
{
	std::cout << remainingCells << std::endl;
	x /= cellSize;
	y /= cellSize;
	if (isOpen[x][y])
		return true;
	if (board[x][y] == 0)
	{
		emptyCellClicked(x, y);
		successSound.play();
		return true;
	}
	isOpen[x][y] = true;
	if (board[x][y] == -1)
	{
		buzzerSound.play();
		revealMines();
		gameOver = true;
		return false;
	}
	--remainingCells;
	if (remainingCells == 0)
	{
		gameWin();
	}
	successSound.play();
	return true;
}
