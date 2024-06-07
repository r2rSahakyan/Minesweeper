#define _WIN32_WINNT 0x0500
#include <SFML/Graphics.hpp>
#include <windows.h>
#include "GameBoard.h"
#include "OpeningScene.h"

int main()
{
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    OpeningScene openingScene;
    sf::RenderWindow openingWindow(sf::VideoMode(400, 400), "Choose Level", sf::Style::Titlebar | sf::Style::Close);
    std::array<int, 3> dificulty{ 20,20,30 };
    while (openingWindow.isOpen())
    {
        sf::Event event;
        while (openingWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                openingWindow.close();
            if (event.type == sf::Event::MouseMoved)
                openingScene.highlight(event.mouseMove.x, event.mouseMove.y);
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                dificulty= openingScene.getSelectedOpt();
                openingWindow.close();
            }
                    
        }
        openingWindow.clear(sf::Color(255, 255, 255));
        ////START DRAWING 
        openingScene.draw(&openingWindow);
        ///END DRAWING        
        openingWindow.display();

    }
    

    bool gameOver = false;
    GameBoard g(dificulty[0],dificulty[1],dificulty[2]);
    g.print();
    sf::RenderWindow window(sf::VideoMode(g.getWindowSize().x, g.getWindowSize().y), "Minesweeper!", sf::Style::Titlebar | sf::Style::Close);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (!gameOver  && event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                    gameOver =!g.openCell(event.mouseButton.x, event.mouseButton.y);
                if (event.mouseButton.button == sf::Mouse::Right)
                    g.setFlag(event.mouseButton.x, event.mouseButton.y);
            }
            if (!gameOver && event.type == sf::Event::MouseMoved)
                g.highLight(event.mouseMove.x, event.mouseMove.y);
        }

        window.clear(sf::Color(255, 255, 255));
        ////START DRAWING 
        g.draw(&window);
        ///END DRAWING        
        window.display();
    }
    return 0;
}