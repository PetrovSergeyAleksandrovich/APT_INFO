#pragma once
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <windows.h>

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
    std::string station_name = "";
    std::string last_time_online = "never";

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

///Return recieved info as string
    void getRealtimeStatusAndParcing()
    {
        resp = cpr::Get(cpr::Url("http://" + apt_url + "/nand/status/realtime_status.xml"));
        std::cout << "\nRequest sent to Station: " << station_name << std::endl;

        station_realtime_status.responce_status_code = resp.status_code;

        if(resp.status_code != 200)
        {
            std::cout << "Station OFFLINE\nlast time online: " << last_time_online << std::endl;
            std::cout << "Error Message: " << resp.error.message << std::endl;
            std::cout << "NO CONNECTION. Retry to connect...";
            std::cout << std::endl;
            return;
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
};