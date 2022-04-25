#pragma once
#include <SFML/Audio.hpp>

class Sounds
{
    int add_delay = 500;
public:
///REQUEST SOUND
    void request()
    {
        sf::SoundBuffer sound_buffer_request;
        if(!sound_buffer_request.loadFromFile("C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\request.flac"))
            return;
        sf::Sound sound_request;
        sound_request.setBuffer(sound_buffer_request);
        sound_request.setVolume(25);
        sound_request.play();
        Sleep(int(sound_request.getBuffer()->getDuration().asMilliseconds()) + add_delay);
        return;
    }

///ONLINE SOUND
    void online()
    {
        sf::SoundBuffer sound_buffer_online;
        if (!sound_buffer_online.loadFromFile("C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\online.flac"))
            return;
        sf::Sound sound_online;
        sound_online.setBuffer(sound_buffer_online);
        sound_online.setVolume(25);
        sound_online.play();
        Sleep(int(sound_online.getBuffer()->getDuration().asMilliseconds()) + add_delay);
        return;
    }

///OFFLINE SOUND
    void offline()
    {
        sf::SoundBuffer sound_buffer_offline;
        if (!sound_buffer_offline.loadFromFile("C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\offline.flac"))
            return;
        sf::Sound sound_offline;
        sound_offline.setBuffer(sound_buffer_offline);
        sound_offline.setVolume(25);
        sound_offline.play();
        Sleep(int(sound_offline.getBuffer()->getDuration().asMilliseconds()) + add_delay);
        return;
    }

///Low current SOUND
    void low_current()
    {
        sf::SoundBuffer sound_buffer;
        if (!sound_buffer.loadFromFile("C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\low_current.flac"))
            return;
        sf::Sound sound;
        sound.setBuffer(sound_buffer);
        sound.setVolume(25);
        sound.play();
        Sleep(int(sound.getBuffer()->getDuration().asMilliseconds()) + add_delay);
        return;
    }

///Triggered_z SOUND
    void triggered_z()
    {
        sf::SoundBuffer sound_buffer;
        if (!sound_buffer.loadFromFile("C:\\Users\\Poizone\\CLionProjects\\APT_INFO\\triggered_z.flac"))
            return;
        sf::Sound sound;
        sound.setBuffer(sound_buffer);
        sound.setVolume(25);
        sound.play();
        Sleep(int(sound.getBuffer()->getDuration().asMilliseconds()) + add_delay);
        return;
    }

};

