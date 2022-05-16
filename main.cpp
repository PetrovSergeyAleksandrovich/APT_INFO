#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "headers/APT_control.h"

void responce_window(sf::RenderWindow &inWindow, sf::Event &inEvent)
{
    while (inWindow.pollEvent(inEvent))
    {
        if (inEvent.type == sf::Event::Closed)
            inWindow.close();
    }
}

void print_test(std::string inText)
{
    std::cout << "print_test: " << inText << std::endl;
}

void Z_level_warning()
{
    Sounds speaker;
    speaker.triggered_z();
}

class Commands
{
    std::string task = "show preview data";
    std::vector<std::string> available_tasks;
public:
    Commands()
    {
        ;
    }

    void setTask()
    {
        std::string input;
        std::cout << "type command: ";
        std::getline(std::cin, input);
        task = input;
    }

    std::string getCurrentTask()
    {
        return task;
    }

};

struct STATION
{
    std::string ip = "";
    std::string station_name = "";
};

int main()
{
    ///VARIABLES
    std::vector<STATION> ip_addresses;
    std::vector<APATIT*> stations;
    int amount_of_stations = 0;
    std::string ip, station_name;
    Sounds speaker;
    Commands command;

    ///READ AVAILABLE STATIONS
    std::ifstream my_file;
    my_file.open("C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\list.txt"); /* set your path on your machine*/
    while(!my_file.eof())
    {
        amount_of_stations++;
        STATION station;
        my_file >> ip >> station_name;
        station.ip = ip;
        station.station_name = station_name;
        ip_addresses.push_back(station);
    }
    my_file.close();

    ///GENERATE VECTOR OF STATIONS TO GET INFO
    for(int i = 0; i < amount_of_stations; i++)
    {
        APATIT* apt = new APATIT(i,ip_addresses[i].station_name, ip_addresses[i].ip);
        stations.push_back(apt);
    }



    ///RUN
    sf::RenderWindow window(sf::VideoMode(640, 640), "APT Monitor", sf::Style::Default);
    window.setFramerateLimit(120);
    sf::Event event;

    sf::CircleShape circle(50.0f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(window.getSize().x/2 - circle.getRadius(), window.getSize().y/2 - circle.getRadius());

    sf::CircleShape circleMax(1.0f);
    circleMax.setFillColor(sf::Color::Blue);
    circleMax.setPosition(window.getSize().x/2 - circleMax.getRadius(), window.getSize().y/2 - circleMax.getRadius());

    FONTS fonts;
    sf::Font font;
    font.loadFromFile(fonts.list[1]);

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(25);

    while (window.isOpen())
    {

        responce_window(window, event);

        std::cout << "Current Task: " << command.getCurrentTask() << std::endl;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            command.setTask();
        }

        ///Status Info Section
        if(command.getCurrentTask() == "show status")
        {
            for(int i = 0; i < stations.size(); i++)
            {
                responce_window(window, event);

                stations[i]->getRealtimeStatusAndParcing();
                speaker.request();

                if(stations[i]->station_realtime_status.responce_status_code == 200)
                {
                    stations[i]->printStatusInfo();
                    speaker.online();
                }
                else if(stations[i]->station_realtime_status.responce_status_code == 0)
                {
                    speaker.offline();
                }

                if(!stations[i]->isOnline)
                {
                    circle.setFillColor(sf::Color::Red);
                }
                else if(stations[i]->isOnline && circle.getFillColor()!=sf::Color::Green)
                {
                    circle.setFillColor(sf::Color::Green);
                }


                ///SLEEP for a while
                std::cout << "/";
                //Sleep(1000);
                std::cout << std::endl;
            }


        }

        ///Preview Info Section
        if(command.getCurrentTask() == "show preview data")
        {
            for(int i = 0; i < stations.size(); i++)
            {

                responce_window(window, event);

                //speaker.request();
                stations[i]->getRealtimePreviewAndParcing();

                if(!stations[i]->isOnline)
                {
                    circle.setFillColor(sf::Color::Red);
                }
                else
                {
                    circle.setFillColor(sf::Color::Green);
                }

                if(stations[i]->isOnline)
                {
                    int tmp = 0;
                    //circleMax.setRadius(1.0f);

                    for(int j = 0; j < stations[i]->EHZ.size(); j++)
                    {

                        responce_window(window, event);

                        if(stations[i]->EHZ[j] < 0)
                        {
                            tmp = (-1*stations[i]->EHZ[j]);
                            circle.setFillColor(sf::Color::Magenta);
                        }
                        else if(stations[i]->EHZ[j] >= 0)
                        {
                            tmp = stations[i]->EHZ[j];
                            circle.setFillColor(sf::Color::Green);
                        }

                        circle.setRadius((100 * tmp) / 8388608.0f);
                        if(circle.getRadius() < 1) circle.setRadius(1.0f);

                        if(circleMax.getRadius() < circle.getRadius())
                        {
                            circleMax.setRadius(circle.getRadius());
                        }

                        circle.setPosition(window.getSize().x/2 - circle.getRadius(),window.getSize().y/2 - circle.getRadius());
                        circleMax.setPosition(window.getSize().x/2 - circleMax.getRadius(),window.getSize().y/2 - circleMax.getRadius());

                        std::string print_text;
                        print_text = stations[i]->station_name + " / TIME: \n"
                                + std::to_string(std::localtime(&stations[i]->current_utc_time)->tm_hour) + " hs  "
                                + std::to_string(std::localtime(&stations[i]->current_utc_time)->tm_min) + " mins "
                                + std::to_string(std::localtime(&stations[i]->current_utc_time)->tm_sec) + " secs";
                        text.setString(print_text);

                        window.clear(sf::Color::Black);
                        window.draw(circleMax);
                        window.draw(circle);
                        window.draw(text);
                        window.display();
                    }

                    std::cout << std::put_time(std::gmtime(&stations[i]->current_utc_time), "%Y %B %d %H %M %S") << '\n';

                }
            }
        }

        std::cout << std::endl;

    }
    ///END OF RUN

    return 0;
}