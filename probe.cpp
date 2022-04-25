#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Enemy
{
public:
    unsigned int index = 0;
    sf::Vector2f size = {20, 20};
    sf::RectangleShape shape;
    sf::Color color = sf::Color::Green;
    float health = 100;
    bool alive = false;
public:
    Enemy(int inIndex)
    {
        index = inIndex;
        shape.setSize(size);
        shape.setFillColor(color);
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    int counter_dead_enemies = 0;
    std::vector<Enemy> enemies;

    sf::Vector2f size(20, 20);
    sf::RectangleShape shape(size);
    shape.setPosition(window.getSize().x/2, window.getSize().y - size.y);
    shape.setFillColor(sf::Color::White);

    sf::CircleShape bullet(5.0f);
    bullet.setFillColor(sf::Color::White);
    bool bullet_lifetime = false;

    sf::Vector2f enemy_size(20, 20);
    sf::RectangleShape enemy(enemy_size);
    enemy.setFillColor(sf::Color::Yellow);
    bool enemy_lifetime = false;

    for(int i = 0; i < 5; i++)
    {
        enemies.push_back(Enemy(i));
    }

    while (window.isOpen())
    {
        window.clear(sf::Color::Black);

        float speed = 0.05;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            if(shape.getPosition().x < window.getSize().x - size.x)
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                {
                    speed *= 2;
                }
                shape.setPosition(shape.getPosition().x+speed , shape.getPosition().y);
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            if(shape.getPosition().x > 0)
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                {
                    speed *= 2;
                }
                shape.setPosition(shape.getPosition().x - speed, shape.getPosition().y);
            }
        }
 /*       if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            if(shape.getPosition().y > 0)
            {
                shape.setPosition(shape.getPosition().x, shape.getPosition().y-speed );
            }

        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            if(shape.getPosition().y < window.getSize().y - size.y)
            {
                shape.setPosition(shape.getPosition().x, shape.getPosition().y+speed );
            }
        }*/

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) and !bullet_lifetime)
        {
            bullet_lifetime = true;
            bullet.setPosition(shape.getPosition().x + shape.getSize().x - (std::rand() % int(size.x)) , shape.getPosition().y);
        }
        if(bullet.getPosition().y < 0)
        {
            bullet_lifetime = false;
        }
        bullet.setPosition(bullet.getPosition().x, bullet.getPosition().y - 0.5);
        if(bullet_lifetime)
        {
            window.draw(bullet);
        }


        for(int i = 0; i < enemies.size(); i++)
        {
            if(!enemies[i].alive)
            {
                enemies[i].alive = true;
                enemies[i].health = 100.0f;
                enemies[i].shape.setFillColor(sf::Color::Green);
                enemies[i].shape.setPosition(std::rand() % window.getSize().x - enemies[i].shape.getSize().x*2, -enemies[i].shape.getSize().y);

            }
            if(enemies[i].shape.getPosition().y < window.getSize().y)
            {
                enemies[i].shape.setPosition(enemies[i].shape.getPosition().x, enemies[i].shape.getPosition().y + 0.01);
            }
            else{
                enemies[i].shape.setPosition(std::rand() % window.getSize().x - enemies[i].shape.getSize().x*2, -enemies[i].shape.getSize().y);
            }

            if(bullet.getPosition().x > enemies[i].shape.getPosition().x &&
               bullet.getPosition().x < enemies[i].shape.getPosition().x + enemies[i].shape.getSize().x &&
               bullet.getPosition().y < enemies[i].shape.getPosition().y + enemies[i].shape.getSize().y)
            {
                enemies[i].health -= float(std::rand() % 50);

                if(enemies[i].health < 100.0f && enemies[i].health > 50.0f)
                {
                    enemies[i].shape.setFillColor(sf::Color::Cyan);
                }
                else if(enemies[i].health < 50.0f && enemies[i].health > 25.0f)
                {
                    enemies[i].shape.setFillColor(sf::Color::Yellow);
                }
                else if(enemies[i].health < 25.0f && enemies[i].health > 0.0f)
                {
                    enemies[i].shape.setFillColor(sf::Color::Red);
                }

                if(enemies[i].health <= 0)
                {
                    enemies[i].alive = false;
                    counter_dead_enemies++;
                    std::cout << "killed enemies " << counter_dead_enemies << std::endl;
                }
                bullet_lifetime = false;

            }
            else{
                window.draw(enemies[i].shape);
            }

        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.draw(shape);
        window.display();


    }
    return 0;
}