#include "pch.h"
#include "Utils.h"

using namespace winrt::Torroid;

std::string Utils::bytesToSize(const std::string& bytesStr) {
    try {
        long long bytes = std::stoll(bytesStr);
        const int kilobyte = 1024;
        const int megabyte = 1024 * 1024;
        const int gigabyte = megabyte * 1024;

        if (bytes < kilobyte)
        {
            return std::to_string(bytes) + " Bytes";
        }
        if (bytes < megabyte) {
            return std::to_string(bytes / kilobyte) + " KB";
        }
        else if (bytes < gigabyte) {
            return std::to_string(bytes / megabyte) + " MB";
        }
        else {
            return std::to_string(bytes / gigabyte) + " GB";
        }
    }
    catch (...) {
        return "Invalid input";
    }
}

std::string Utils::FilePath_to_FileName(std::string filePath)
{
    std::string substring;
    std::vector<std::string> substrings;
    std::istringstream iss(filePath);

    //Seprate file name from full path
    while (std::getline(iss, substring, '/'))
    {
        substrings.push_back(substring);
    }

    // Return file name 
    return substrings.back();
}