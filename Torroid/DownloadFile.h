#pragma once

#include "pch.h"
#include<aria2/aria2.h>
#include<json.h>

#include <winrt/Windows.Foundation.Collections.h>

using namespace winrt;
using namespace Windows::Foundation;

class DownloadFile
{
private:
    json jsonEntry;                 // Initialize json object
    aria2::Session* session = NULL;        // aria2 session object
    aria2::KeyVals options;         // aria2 options
    aria2::SessionConfig config;    // configuration for the session
    aria2::GlobalStat globalStat = {NULL};   // Global statistics of current aria2 session
    std::vector<aria2::A2Gid> gids; // unique gid per download
    int iActiveDownload=0;
    static DownloadFile* instance_;
    // constructor
    DownloadFile();
    

public:
    
    /*=============================================[ Setup FUNCTION ]=============================================*/

    // Return DownloadFile static object
    static DownloadFile& DownloadInstance();

    // Session setup
    IAsyncAction DownloadFile::setupSession();

    // Start Download
    void StartDownload();

    /*=============================================[ ACTION FUNCTION ]=============================================*/

    /*
     Add new download url
     Arguments:
     1. vector containing urls
     */
    IAsyncAction DownloadFile::addUrl(std::vector<std::string> uri);

    /*-------------------------------[ PAUSE ,RESUME AND CANCEL ]-------------------------------*/

    /*
     Pause Download of given GID
     Return 0 if sucess else negative error code
     */
    int pause(int gidIndex);

    // Cancle download
    int Canceldownload(int gidIndex);

    // Resume paused download
    int ResumeDownload(int gidIndex);

    void DownloadFile::onDownloadComplete(aria2::A2Gid gid);


    /*============================================[ GET FUNCTIONS ]============================================*/
    
    // Return number of active downloads
    int getSessionActiveDownloads();

    IAsyncAction DownloadFile::updateJsonAndUI(aria2::A2Gid gid, std::string url);

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
