#include <iostream>
#include <vector>
#include <iomanip>
#include <cpr/cpr.h>
#include <random>
#include <math.h>
#include <windows.h>
#include <sfml/graphics.hpp>
#include <sfml/window.hpp>

struct FONTS
{
    std::vector<std::string> list = {
            "C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\fonts\\Roboto-Regular-webfont.woff",
            "C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\fonts\\Roboto-Bold-webfont.woff",
            "C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\fonts\\Roboto-Light-webfont.woff",
            "C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\fonts\\undertale battle font_0.ttf",
            "C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\fonts\\SigmarOne-Regular.ttf",
            "C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\fonts\\Monoton-Regular.ttf",
            "C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\fonts\\RockSalt-Regular.ttf",
            "C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\fonts\\RubikGlitch-Regular.ttf",
            "C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\fonts\\RubikMicrobe-Regular.ttf",
    };
};

int main()
{

    sf::RenderWindow window(sf::VideoMode(1000, 500), "window", sf::Style::Default);
    window.setFramerateLimit(60);

    FONTS fonts;
    sf::Font font;
    font.loadFromFile(fonts.list[4]);

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(50);

    sf::Text text2;
    text2.setFont(font);
    text2.setCharacterSize(50);

    std::string s = "Hello World\n";
    std::string tmp_text;

    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed) window.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        tmp_text.clear();
        text.setPosition(std::rand() % window.getSize().x/2, std::rand() % window.getSize().y/2);
        text.setFillColor(sf::Color(std::rand()%255, std::rand()%255, std::rand()%255));
        int shift = 1;

        text.setCharacterSize(std::rand()%100 + 25);
        for(int i = 0; i < s.size(); i++)
        {
            font.loadFromFile(fonts.list[std::rand() % fonts.list.size()]);
            text.setString(tmp_text);
            Sleep(100);
            tmp_text += s[i];
            text.setPosition(text.getPosition().x - shift, text.getPosition().y);

            window.clear(sf::Color::Black);
            window.draw(text);
            window.draw(text2);
            window.display();

            if(i == 2) text2.setString("");
        }

        tmp_text.clear();
        text2.setPosition(std::rand() % window.getSize().x/2, std::rand() % window.getSize().y/2);
        text2.setFillColor(sf::Color(std::rand()%255, std::rand()%255, std::rand()%255));
        text2.setCharacterSize(std::rand()%100 + 25);
        for(int i = 0; i < s.size(); i++)
        {
            font.loadFromFile(fonts.list[std::rand() % fonts.list.size()]);
            text2.setString(tmp_text);
            Sleep(100);
            tmp_text += s[i];
            text.setPosition(text.getPosition().x - shift, text.getPosition().y);

            window.clear(sf::Color::Black);
            window.draw(text);
            window.draw(text2);
            window.display();

            if(i == 2) text.setString("");
        }

    }

    return 0;
}
