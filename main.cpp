#include <iostream>
#include <vector>
#include <cmath>
#include "apt_realtime_status_parcing.h"
#include "APT_control.h"
#include <windows.h>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "sounds.h"


void print_test(std::string inText)
{
    std::cout << "print_test: " << inText << std::endl;
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
    while(true)
    {
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

                /// Test Input Current
                if(stations[i]->station_realtime_status.input_current < 0.25f)
                {
                    speaker.low_current();
                }

                ///SLEEP for a while
                std::cout << "/";
                Sleep(1000);
                std::cout << std::endl;
            }
        }

        ///Preview Info Section
        if(command.getCurrentTask() == "show preview data")
        {
            for(int i = 0; i < stations.size(); i++)
            {
                speaker.request();
                stations[i]->getRealtimePreviewAndParcing();
            }

            ///SLEEP for a while
            std::cout << "/";
            Sleep(1000);
            std::cout << std::endl;
        }

        std::cout << std::endl;


    }
    ///END OF RUN

    return 0;
}