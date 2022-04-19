#include <iostream>
#include <vector>
#include "APT_control.h"
#include "apt_realtime_status_parcing.h"
#include <windows.h>
#include <SFML/Audio.hpp>

void print_test(std::string inText)
{
    std::cout << "print_test: " << inText << std::endl;
}

struct STATION
{
    std::string ip = "";
    std::string station_name = "";
};

int main()
{
    std::vector<STATION> ip_addresses;
    std::vector<APATIT*> stations;
    int amount_of_stations = 0;
    std::string ip, station_name;
    float time_to_sleep = 0.0f;

    ///REQUEST
    sf::SoundBuffer sound_buffer_request;
    if (!sound_buffer_request.loadFromFile("C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\request.flac"))
        return -1;
    sf::Sound sound_request;
    sound_request.setBuffer(sound_buffer_request);
    sound_request.setVolume(25);


    ///ONLINE
    sf::SoundBuffer sound_buffer_online;
    if (!sound_buffer_online.loadFromFile("C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\online.flac"))
        return -1;
    sf::Sound sound_online;
    sound_online.setBuffer(sound_buffer_online);
    sound_online.setVolume(25);

    ///OFFLINE
    sf::SoundBuffer sound_buffer_offline;
    if (!sound_buffer_offline.loadFromFile("C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\offline.flac"))
        return -1;
    sf::Sound sound_offline;
    sound_offline.setBuffer(sound_buffer_offline);
    sound_offline.setVolume(25);

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

    for(int i = 0; i < amount_of_stations; i++)
    {
        APATIT* apt = new APATIT(i,ip_addresses[i].station_name, ip_addresses[i].ip);
        stations.push_back(apt);
    }

    ///RUN
    while(true)
    {
        for(int i = 0; i < stations.size(); i++)
        {
            stations[i]->getRealtimeStatusAndParcing();
            sound_request.play();
            Sleep(3000);

            if(stations[i]->station_realtime_status.responce_status_code == 200)
            {
                stations[i]->printStatusInfo();
                sound_online.play();
            }
            else if(stations[i]->station_realtime_status.responce_status_code == 0)
            {
                sound_offline.play();
            }
        }
        std::cout << std::endl;

        ///SLEEP for a while
        for(int i = 0 ; i < 10; i++)
        {
            Sleep(500);
            std::cout << "/";
        }
        std::cout << std::endl;
    }
    ///END OF RUN

    return 0;
}