#pragma once

#include<aria2/aria2.h>
#include<json.h>

class DownloadFile
{
private:
    json jsonEntry;                 // Initialize json object
    aria2::Session* session;        // aria2 session object
    aria2::KeyVals options;         // aria2 options
    aria2::SessionConfig config;    // configuration for the session
    aria2::GlobalStat globalStat;   // Global statistics of current aria2 session
    std::vector<aria2::A2Gid> gids; // unique gid per download

public:
    // constructor
    DownloadFile();

    void DownloadFile::setupSession();

    /*=============================================[ ACTION FUNCTION ]=============================================*/

    /*
     Add new download url
     Arguments:
     1. vector containing urls
     */
    int DownloadFile::addUrl(std::vector<std::string> uri);

    int StartDownload();


    /*----------------------------------[ PAUSE ,RESUME AND CANCEL ]----------------------------------*/

    /*
     Pause Download of given GID
     Return 0 if sucess else negative error code
     */
    int pause(int gidIndex);

    // Cancle download
    int Canceldownload(int gidIndex);

    // Resume paused download
    int ResumeDownload(int gidIndex);


    /*============================================[ GET FUNCTIONS ]============================================*/
    
    // Return number of active downloads
    int getSessionActiveDownloads();

    // Return file name
    std::string getDownloadFilename(int gidIndex);

    /*----------------------------------------[ SPEED ]----------------------------------------*/

    // Return overall download speed
    int getSessionDownloadSpeed();

    // Return download speed for a particular gid
    int getDownloadspeed(int gidIndex);

    /*------------------------------------[ DOWNLOAD SIZE ]------------------------------------*/

    // Return total size of file to download
    int getFileSize(int gidIndex);

    // Returns the completed length of this download in bytes.
    int getDownloadedSize(int gidIndex);

    /*------------------------------------[ DOWNLOAD DIRECTORY ]-------------------------------*/

    // Return download directory of file to download
    std::string getDownloadDir(int gidIndex);

    int closeSession();

};
