#pragma once

#include "pch.h"
#include "aria2/aria2.h"
#include "DownloadsJson.h"

#include <vector>
#include <string>
#include <sstream>

class DownloadFile
{
private:
    aria2::Session* session{ NULL };        // aria2 session object
    aria2::KeyVals options;         // aria2 options
    aria2::SessionConfig config;    // configuration for the session
    aria2::GlobalStat globalStat{ NULL };   // Global statistics of current aria2 session
    std::vector<aria2::A2Gid> gids; // unique gid per download
    int iActiveDownload = 0;
    static DownloadFile* instance_;

    // constructor
    DownloadFile();

public:

    /*=============================================[ Setup FUNCTION ]=============================================*/

    // Return DownloadFile static object
    static DownloadFile& DownloadInstance();

    // Session setup
    int DownloadFile::setupSession();

    // Start Download
    void StartDownload();

    /*=============================================[ ACTION FUNCTION ]=============================================*/

    /*
        Add new download url
        Arguments:
        1. vector containing urls

        Return status:
        0 : if file is downloading for first time
        1 : if file downloaded last time but not present in last download directory
        2 : if file is present but not completely downloaded yet (paused)
        3 : if file already downloaded and present in last download directory

    */
    int addUrl(std::vector<std::string> uri);

    /*-------------------------------[ PAUSE ,RESUME AND CANCEL ]-------------------------------*/

    /*
        Pause Download
        Return 0 if sucess else negative error code
        */
    void pause(int Index);

    // Cancle download
    int Canceldownload(int Index);

    // Resume paused download
    int ResumeDownload(int Index);

    // Update json on download complete event
    void onDownloadComplete(aria2::A2Gid gid);


    /*============================================[ GET FUNCTIONS ]============================================*/

    // Return number of active downloads
    int getSessionActiveDownloads();

    int updateJson(aria2::A2Gid gid, std::string url);

    // Return file name
    std::string getDownloadFilename(aria2::A2Gid gid);

    /*----------------------------------------[ SPEED ]----------------------------------------*/

    // Return overall download speed
    int getSessionDownloadSpeed();

    // Return download speed for a particular Download
    int getDownloadspeed(int Index);

    /*------------------------------------[ DOWNLOAD SIZE ]------------------------------------*/

    // Return total size of file to download
    int64_t getTotalFileSize(int Index);

    // Returns the completed length of this download in bytes.
    int64_t getCurrentFileSize(int Index);

    /*------------------------------------[ DOWNLOAD DIRECTORY ]-------------------------------*/

    // Return download directory of file to download
    std::string getDownloadDir(int Index);

    // Destructor
    ~DownloadFile();

};
