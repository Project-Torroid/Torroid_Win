#include "pch.h"
#include "Utils.h"

#include <iomanip>
#include "DownloadsJson.h"

using namespace winrt::Torroid;

std::string Utils::bytesToSize(const std::string& bytesStr) {
    try {
        long long bytes = std::stoll(bytesStr);
        const int kilobyte = 1024;
        const int megabyte = 1024 * 1024;
        const int gigabyte = megabyte * 1024;

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);

        if (bytes < kilobyte) {
            ss << bytes << " Bytes";
        }
        else if (bytes < megabyte) {
            ss << (bytes / static_cast<double>(kilobyte)) << " KB";
        }
        else if (bytes < gigabyte) {
            ss << (bytes / static_cast<double>(megabyte)) << " MB";
        }
        else {
            ss << (bytes / static_cast<double>(gigabyte)) << " GB";
        }

        return ss.str();
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
    while (std::getline(iss, substring, '\\'))
    {
        substrings.push_back(substring);
    }

    // Return file name 
    return substrings.back();
}

std::string Utils::FilePath_to_FolderPath(std::string filePath)
{
    // Get the folder path.
    size_t lastSlashPos = filePath.find_last_of('\\');
    if (lastSlashPos != std::string::npos)
    {
        filePath = filePath.substr(0, lastSlashPos);
    }
    return filePath;
}

size_t Utils::FileName_to_Index(hstring const& fileName)
{
    return DownloadsJson::jsonInstance().getIndex(winrt::to_string(fileName));
}