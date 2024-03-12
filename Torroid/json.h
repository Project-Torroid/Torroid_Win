#pragma once

#include <regex>
#include <map>


class json
{
public:
    std::string jsonfilename = "downloads.json";                      // JSON file name
    std::string jsonFilePath;                                         // JSON file path
    std::vector<std::map<std::string, std::string>> vDownloadEntries; // Json object key value pair vector+

    // Constructor
    json();

    /*=====================================================[ Get json object ]=====================================================*/

    // Return key value pair from json string
    std::map<std::string, std::string> getKeyValuePairs(std::string &json);

    // Get json keyvalue pair from file to vector
    int json::GetJsonToVector();

    /*=====================================================[ Add download to json ]=====================================================*/

    /*
     Add new download info to json
     Arguments :
     1. File name
     2. Total file size in MB
     3. URL of file to download
     */
    void json::addDownloadToJson(
        std::string filename,        // File name
        std::string totalFileSize,   // Total file size in MB
        std::string url              // URL of file to download
    );
    /*=============================================[ Json update on pause, complete or delete ]=============================================*/

    /* Update states of download in Json file on download pause
     Arguments :
     1. Index
     2. new Download size
     3. new download percentage
     */
    void json::updateJsonOnPause(size_t index, std::string newDownloadedSize, std::string newdownloadPercent);

    /* Update states of download in Json file on download complete
     Arguments :
     1. Index
     2. total downloaded size
     */
    void json::updateJsonOnDownloadComplete(size_t index, std::string totalDownloadedSize);

    /* Delete download entry in Json file
     Arguments :
     1. Index
     */
    void json::deleteDownload(size_t index);

    // Write json info to json file on exit
    void json::writeToJsonOnexit();
};
