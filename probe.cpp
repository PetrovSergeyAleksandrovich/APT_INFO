#include <iostream>
#include <string>
#include <fstream>
#include <cpr/cpr.h>
#include <windows.h>

int main()
{
    cpr::Response resp = cpr::Get(cpr::Url("http://195.112.99.102:8888/nand/status/realtime_preview.xml"));
    std::cout << resp.text << std::endl;

    std::ofstream my_file;
    my_file.open("APT_factory_data.txt", std::ios::trunc);
    my_file << resp.text;

    return 0;
}