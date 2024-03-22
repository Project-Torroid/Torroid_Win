#include "pch.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include "logging.h"

std::string LogFilePath = "C:\\test\\torroid.log";

std::string Logging::current_datetime() {
    std::time_t now = std::time(nullptr);
    std::tm now_tm;

    // Use localtime_s to get the local time
    if (localtime_s(&now_tm, &now) != 0) {
        // Handle error (e.g., print an error message)
        return "Error getting local time";
    }

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void Logging::Error(std::string message)
{
    std::ofstream loggingfile(LogFilePath,  std::ios::app);
    loggingfile << "[-] " << current_datetime() << " " << message << std::endl;
    loggingfile.close();
}

void Logging::Info(std::string message)
{
    std::ofstream loggingfile(LogFilePath, std::ios::app);
    loggingfile << "[i] " << current_datetime() << " " << message << std::endl;
    loggingfile.close();
}

void Logging::warning( std::string message)
{
    std::ofstream loggingfile(LogFilePath, std::ios::app);
    loggingfile << "[W] " << current_datetime() << " " << message << std::endl;
    loggingfile.close();
}






//int main()
//{
//    class Logging l1;
//    l1.Error("error_log.txt", "This is error(fuck you buddy.....)");
//    l1.Info("error_log.txt","This is info");
//    l1.warning("error_log.txt","This is warning");
//    l1.ForBuddy("error_log.txt","hi buddy");
//    return EXIT_SUCCESS;
//}
