#pragma once

#include<aria2/aria2.h>

class DownloadFile
{
private:
    aria2::Session* session;        // aria2 session object
    aria2::KeyVals options;         // aria2 options
    aria2::SessionConfig config;    // configuration for the session
    aria2::GlobalStat globalStat;   // Global statistics of current aria2 session
    std::vector<aria2::A2Gid> gids; // unique gid per download 

public:
    // constructor
    DownloadFile();

    /*
     * Add new download url
     * arguments:
     * 1. New url to download file
     * 2. Download index according to ui
     */
    int DownloadFile::addUrl(std::vector<std::string> uri);

    int StartDownload();

    void DownloadFile::setupSession(aria2::KeyVals options, aria2::SessionConfig config);

    // Return number of active downloads
    int getSessionActiveDownloads();

    /*****************************************[ SPEED ]*****************************************/

    // Return overall download speed
    int getSessionDownloadSpeed();

    // Return download speed for a particular gid
    int getDownloadspeed(int gidIndex);

    /*************************************[ DOWNLOAD SIZE ]*************************************/

    // Return total size of file to download
    int getFileSize(int gidIndex);

    // Returns the completed length of this download in bytes.
    int getDownloadedSize(int gidIndex);

    /*************************************[ DOWNLOAD DIRECTORY ]********************************/

    // Return download directory of file to download
    std::string getDownloadDir(int gidIndex);

    int closeSession();

};

