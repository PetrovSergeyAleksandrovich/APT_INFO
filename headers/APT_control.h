#pragma once
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <windows.h>
#include <iomanip>
#include "sounds.h"

int getRadiusFromData(std::vector<int> inVector)
{
    float average = 0.0f;

    for (auto &i : inVector)
    {
        average += float(i);
    }
    average /= inVector.size();

    return int((inVector[0] * 100) / 1000000);
}

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

struct STATION_REALTIME_STATUS
{
    int responce_status_code = 0;
    std::string date = "";
    std::string device_name = "";
    std::string model = "";
    std::string production = "";
    std::string fw = "";
    int memory_total = 0;
    int memory_free = 0;
    std::string rec_status = "";
    std::string recmode = "";
    int samplerate = 0;
    std::string session = "none";
    std::string miniseed_00 = "";
    std::string miniseed_01 = "";
    std::string miniseed_02 = "";
    std::string gps_utc = "never";
    float input_voltage = 0.0f;
    float input_current = 0.0f;
};

class APATIT
{
    std::string apt_url;
public:
    cpr::Response resp;
    STATION_REALTIME_STATUS station_realtime_status;
    int station_number = 0;
    std::string station_name = "noname";
    std::string last_time_online = "never";
    std::vector<int> EHZ, EHN, EHE;
    time_t current_utc_time;
    double z_displacement = 0.0f;
    bool isOnline = true;

public:
    /// Constructor
    APATIT(int inStationNumber, std::string inStationName, std::string inURL)
    {
        this->apt_url = inURL;
        this->station_number = inStationNumber;
        this->station_name = inStationName;
    }

    ///Prints all status data
    void printStatusInfo()
    {
        std::cout << "Station ONLINE\nTIME UTC: " << station_realtime_status.gps_utc << std::endl;
        std::cout << "Session Time: " << station_realtime_status.session << std::endl;
        std::cout << "Input Voltage: " << station_realtime_status.input_voltage << std::endl;
        std::cout << "Input Current: " << station_realtime_status.input_current << std::endl;
    }

///Return recieved info realtime status
    void getRealtimeStatusAndParcing()
    {
        ///mnt/nand/factory_data.xml
        resp = cpr::Get(cpr::Url("http://" + apt_url + "/nand/status/realtime_status.xml"));
        ///resp = cpr::Get(cpr::Url("http://" + apt_url + "/nand/status/realtime_preview.xml"));

        ///std::cout << resp.text << std::endl;

        std::cout << "\nRequest sent to Station: " << station_name << std::endl;

        station_realtime_status.responce_status_code = resp.status_code;

        if(resp.status_code != 200)
        {
            isOnline = false;
            std::cout << "Station OFFLINE\nlast time online: " << last_time_online << std::endl;
            std::cout << "Error Message: " << resp.error.message << std::endl;
            std::cout << "NO CONNECTION. Retry to connect...";
            std::cout << std::endl;
            return;
        }
        else
        {
            isOnline = true;
        }
        parcingAptTime();
        parcingAptSession();
        parcingInputVoltage();
        parcingInputCurrent();
    }

///Return UTC date and time as string
    void parcingAptTime()
    {
        station_realtime_status.gps_utc = resp.text;

        if(station_realtime_status.gps_utc.find("gps utc="))
        {
            for(int i = station_realtime_status.gps_utc.find("gps utc="); i < station_realtime_status.gps_utc.size(); i++)
            {
                int counter = 0;
                int j = i + 9;
                std::string tmp;
                while(counter < 1)
                {
                    tmp += station_realtime_status.gps_utc[j++];
                    if(station_realtime_status.gps_utc[j] == '\"')counter++;
                }
                station_realtime_status.gps_utc = tmp;
            }

            for (int i = 0; i < station_realtime_status.gps_utc.size(); i++)
            {
                if(station_realtime_status.gps_utc[i] == '"') station_realtime_status.gps_utc[i] = ' ';
            }
        }
    }

///Return SESSION TIME as string
    void parcingAptSession()
    {
        station_realtime_status.session = resp.text;

        if(station_realtime_status.session.find("session="))
        {
            for(int i = station_realtime_status.session.find("session="); i < station_realtime_status.session.size(); i++)
            {
                int counter = 0;
                int j = i + 9;
                std::string tmp;
                while(counter < 1 )
                {
                    tmp += station_realtime_status.session[j++];
                    if(station_realtime_status.session[j] == '\"')counter++;
                }
                station_realtime_status.session = tmp;
            }

            for (int i = 0; i < station_realtime_status.session.size(); i++)
            {
                if(station_realtime_status.session[i] == '"') station_realtime_status.session[i] = ' ';
            }
        }
    }

    ///Return INPUT VOLTAGE value
    void parcingInputVoltage()
    {
        std::string info = resp.text;

        if(info.find("inputvoltage="))
        {
            for(int i = info.find("inputvoltage="); i < info.size(); i++)
            {
                int counter = 0;
                int j = i + 13;
                std::string tmp;
                while(counter < 1)
                {
                    tmp += info[j++];
                    if(info[j] == '\"')counter++;
                }
                info = tmp;
            }

            for (int i = 0; i < info.size(); i++)
            {
                if(info[i] == '"') info[i] = ' ';
            }
        }
        station_realtime_status.input_voltage =  std::stof(info);
    }


    ///Return INPUT CURRENT value
    void parcingInputCurrent()
    {
        std::string info = resp.text;

        if(info.find("inputcurrent="))
        {
            for(int i = info.find("inputcurrent="); i < info.size(); i++)
            {
                int counter = 0;
                int j = i + 13;
                std::string tmp;
                while(counter < 1)
                {
                    tmp += info[j++];
                    if(info[j] == '\"')counter++;
                }
                info = tmp;
            }

            for (int i = 0; i < info.size(); i++)
            {
                if(info[i] == '"') info[i] = ' ';
            }
        }
        station_realtime_status.input_current =  std::stof(info);
    }

    ///Send GET request to Station
    void getRequest()
    {
        std::cout << "\nRequest sent to Station: " << station_name << std::endl;
        resp = cpr::Get(cpr::Url(apt_url));

        if(resp.status_code != 200)
        {
            std::cout << "Station OFFLINE\nlast time online: " << last_time_online << std::endl;
            std::cout << "Error Message: " << resp.error.message << std::endl;
            std::cout << "NO CONNECTION. Retry to connect...";
            std::cout << std::endl;
            return;
        }
        else
        {
            std::cout << "GET status_code responce: " << resp.status_code << std::endl;
        }
        return;
    }

///Return recieved info realtime preview
    void getRealtimePreviewAndParcing()
    {
        resp = cpr::Get(cpr::Url("http://" + apt_url + "/nand/status/realtime_preview.xml"));

        std::string info = resp.text;

        std::cout << "resp.status_code " << resp.status_code << std::endl;

        if(resp.status_code != 200)
        {
            isOnline = false;
            std::cout << "Station OFFLINE\nlast time online: " << last_time_online << std::endl;
            std::cout << "Error Message: " << resp.error.message << std::endl;
            std::cout << "NO CONNECTION. Retry to connect...";
            std::cout << std::endl;
            Sounds speaker;
            speaker.offline();
            return;
        }
        else
        {
            isOnline = true;
        }

        if(resp.status_code == 200)
        {
            ///Clear recieved data buffer; If ignore then data is collecting
            EHZ.clear();
            EHN.clear();
            EHE.clear();
            int start_point;
            start_point = info.find("block");

            ///Start parcing EHZ EHE EHN and time in C-format
            for(int i = start_point; i < info.size(); i++)
            {
                ///ch1 EHZ parsing
                if(info[i] == 'c' && info[i+1] == 'h'
                   && info[i+2] == '1' && info[i+3] == '=' && info[i+4] == '"')
                {
                    std::string tmp = "";
                    for(int j = i + 5; info[j] != '"'; j++)
                    {
                        tmp += info[j];
                        if(info[j] == '"')
                        {
                            i += j;
                        }
                    }
                    EHZ.emplace_back(std::stoi(tmp));
                }
                ///ch2 EHN parsing
                if(info[i] == 'c' && info[i+1] == 'h'
                   && info[i+2] == '2' && info[i+3] == '=' && info[i+4] == '"')
                {
                    std::string tmp = "";
                    for(int j = i + 5; info[j] != '"'; j++)
                    {
                        tmp += info[j];
                        if(info[j] == '"')
                        {
                            i += j;
                        }
                    }
                    EHN.emplace_back(std::stoi(tmp));
                }
                ///ch3 EHE parsing
                if(info[i] == 'c' && info[i+1] == 'h'
                   && info[i+2] == '3' && info[i+3] == '=' && info[i+4] == '"')
                {
                    std::string tmp = "";
                    for(int j = i + 5; info[j] != '"'; j++)
                    {
                        tmp += info[j];
                        if(info[j] == '"')
                        {
                            i += j;
                        }
                    }
                    EHE.emplace_back(std::stoi(tmp));
                }

                ///C-time parcing, getting seconds since 1970-01-01 00:00:00 UTC
                if(info[i] == 't' && info[i+1] == 'i' && info[i+2] == 'm'
                   && info[i+3] == 'e' && info[i+4] == '=' && info[i+5] == '"')
                {
                    std::string tmp = "";
                    for(int j = i + 6; info[j] != '"'; j++)
                    {
                        tmp += info[j];
                        if(info[j] == '"')
                        {
                            i += j;
                        }
                    }
                    current_utc_time = std::stoi(tmp);
                }
            }
        }

        ///Triggered Z channel
/*        for(int i = 0; i < EHZ.size(); i++)
        {
            if(EHZ[i] > 1)
            {
                std::cout << "Triggered value Z: " << EHZ[i] << std::endl;
                std::cout << "UTC:   " << std::put_time(std::gmtime(&current_utc_time), "%Y %B %d %H %M %S") << '\n';
                std::cout << "Local: " << std::put_time(std::localtime(&current_utc_time), "%Y %B %d %H %M %S") << '\n';
                Sounds speaker;
                speaker.triggered_z();
                break;
            }
        }*/




    }



};