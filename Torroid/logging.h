#pragma once

class Logging {

public:
    // it return current date and time:
   static std::string current_datetime();

    // log errors in log files:
   static void Error(std::string message);

    // log information in files:
   static  void Info(std::string message);

    // log warning in files:
   static void warning(std::string message);

    // for buddy 
   static void ForBuddy(std::string message);

};
