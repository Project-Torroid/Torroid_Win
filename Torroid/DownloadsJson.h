#pragma once

#include <regex>
#include <map>


class DownloadsJson
{
private:
    static DownloadsJson* jsonInstance_;

    // Constructor
    DownloadsJson();
public:
    std::string jsonfilename = "downloads.json";                      // JSON file name
    std::string jsonFilePath;                                         // JSON file path
    std::vector<std::map<std::string, std::string>> vDownloadEntries; // Json object key value pair vector+

	// Return the size of the JSON Vector
    size_t size();

    //args: filename
    //return: index
	//return the index of the filename in the JSON vector
	size_t getIndex(std::string filename);

    // Return DownloadFile static object
    static DownloadsJson& jsonInstance();

    /*=====================================================[ Get json object ]=====================================================*/

    // Return key value pair from json string
    std::map<std::string, std::string> getKeyValuePairs(std::string &json);

    // Get json keyvalue pair from file to vector
    int DownloadsJson::GetJsonToVector();

    /*===================================================[ Add download to json ]===================================================*/

    /*
     Add new download info to json
     Arguments :
     1. File name
     2. Total file size in MB
     3. URL of file to download
     */
    void DownloadsJson::addDownloadToJson(
        std::string filename,      // File name
        std::string totalFileSize, // Total file size in MB
        std::string url,           // URL of file to download
        std::string gid            // Download file gid
    );
    /*=========================================[ Json update on pause, complete or delete ]=========================================*/

    /* Update states of download in Json file on download pause
     Arguments :
     1. Index
     2. new Download size
     3. new download percentage
     */
    void DownloadsJson::updateJsonOnPause(size_t index, std::string newDownloadedSize, std::string newdownloadPercent);

    /* Update states of download in Json file on download complete
     Arguments :
     1. Index
     2. total downloaded size
     */
    void DownloadsJson::updateJsonOnDownloadComplete(size_t index, std::string totalDownloadedSize);

    /* Delete download entry in Json file
     Arguments :
     1. Index
     */
    void DownloadsJson::RemoveDownloadEntry(int index);

    // Write json info to json file on exit
    void DownloadsJson::writeToJsonOnexit();

    /*====================================================[ Getters and Setters ]====================================================*/

    /* setters
     Arguments :
     1. Index
     2. value to update
     */

    // fileDownloadPercentage
    std::string DownloadsJson::fileDownloadPercentage(int Index); // Getter
    void DownloadsJson::fileDownloadPercentage(int Index, std::string fileDownloadPercentage); // Getter

    // fileStatus
    std::string DownloadsJson::fileStatus(int Index); // Getter
    void DownloadsJson::fileStatus(int Index, std::string fileStatus); // Getter
    
    //fileSizeCurrent
    std::string DownloadsJson::fileSizeCurrent(int Index); // Getter
    void DownloadsJson::fileSizeCurrent(int Index, std::string fileSizeCurrent); // Getter

    // filename
    std::string DownloadsJson::filename(int Index); // Getter
    void DownloadsJson::filename(int Index, std::string filename); // Getter

    // gid
    std::string DownloadsJson::gid(int Index); // Getter
    void DownloadsJson::gid(int Index, std::string gid); // Getter
    
    // fileSizeTotal
    std::string DownloadsJson::fileSizeTotal(int Index); // Getter
    void DownloadsJson::fileSizeTotal(int Index, std::string fileSizeTotal); // Getter
    
    // fileUrl
    std::string DownloadsJson::fileUrl(int Index); // Getter
    void DownloadsJson::fileUrl(int Index, std::string fileUrl); // Getter
};
