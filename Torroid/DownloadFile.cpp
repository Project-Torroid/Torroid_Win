#include "pch.h"
#include "DownloadFile.h"

#include <aria2/aria2.h>
#include <vector>
#include <sstream>
#include <winrt/Windows.Storage.h>
#include <future>
#include "logging.h"
#include "json.h"

int downloadEventCallback(aria2::Session* session, aria2::DownloadEvent event, const aria2::A2Gid& gid, void* userData)
{
    switch (event)
    {
    case aria2::EVENT_ON_DOWNLOAD_START:
        /*std::cout << "DOWNLOAD STARTED" << std::endl;*/
        break;
    case aria2::EVENT_ON_DOWNLOAD_COMPLETE:
        //std::cerr << "COMPLETE" << std::endl;
        break;
    case aria2::EVENT_ON_DOWNLOAD_ERROR:
        //std::cerr << "ERROR" << std::endl;
        break;
    }
    return 0;
}

DownloadFile::DownloadFile() {
    
    // Initialize aria2 dependencies
    int iResult = aria2::libraryInit();
    if (iResult)
    {
        Logging::Error("iResult: aria2 initionalisation error");
    }
    
    // Set callback function which will be invoked when download event occurred
    config.downloadEventCallback = (aria2::DownloadEventCallback)downloadEventCallback;

    // Downloads Folder Path
    auto dFolder = winrt::Windows::Storage::KnownFolders::GetFolderForUserAsync(nullptr, winrt::Windows::Storage::KnownFolderId::DownloadsFolder).get();

    // setting options to session
    options.push_back(std::pair<std::string, std::string>("file-allocation", "none"));
    options.push_back(std::pair<std::string, std::string>("max-connection-per-server", "10"));
    options.push_back(std::pair<std::string, std::string>("min-split-size", "5M"));
    options.push_back(std::pair<std::string, std::string>("split", "8"));
    options.push_back(std::pair<std::string, std::string>("max-concurrent-downloads", "8"));

    if (dFolder)
    {
        std::wstring folderPath = dFolder.Path().c_str();
        std::string downloadPath(folderPath.begin(), folderPath.end());
        std::string directory = "dir";
        //Logging::Info("Download path : " + downloadPath);
        std::pair<std::string, std::string> downloadOption(directory, downloadPath);
        options.push_back(downloadOption);
    }
}

void DownloadFile::setupSession() {
    // Create session object
    session = aria2::sessionNew(options, config);

    // Global statistics of current aria2 session
    globalStat = aria2::getGlobalStat(session);
}

int DownloadFile::addUrl(std::vector<std::string> uri)
{
    aria2::A2Gid gid;

    // Add new URI to session to download file
    int iResult = aria2::addUri(session, &gid, uri, options);
    if (iResult < 0)
    {
        /*std::cerr << "Failed to add download" << uri[0] << std::endl;*/
        return EXIT_FAILURE;
    }

    // Add ne gid to gid vector
    gids.insert(gids.begin(),gid);

    jsonEntry.json::addDownloadToJson("filenme","67mb",uri[0]);
    DownloadFile::ResumeDownload(0);

    return EXIT_SUCCESS;
}

int DownloadFile::StartDownload()
{
    int size = 0, size1 = 0;
    while (true)
    {
        int iResult = aria2::run(session, aria2::RUN_ONCE);
        size = getDownloadedSize(0);
        // size1 = getDownloadedSize(1);
        /*std::cout << "d1 -> " << size << " d0 -> " << size1 << std::endl;*/
        if (iResult != 1)
        {
            break;
        }
    }
    return EXIT_SUCCESS;
}

int DownloadFile::pause(int gidIndex)
{
    int iResult = aria2::pauseDownload(session, gids[gidIndex]);
    return iResult;
}

int DownloadFile::Canceldownload(int gidIndex)
{
    return aria2::removeDownload(session, gids[gidIndex]);
}

int DownloadFile::ResumeDownload(int gidIndex)
{
    Canceldownload(gidIndex);
    std::vector<std::string> uri;
    uri.push_back(jsonEntry.vDownloadEntries[gidIndex]["url"]);
    addUrl(uri);
    return EXIT_SUCCESS;
}

int DownloadFile::getSessionActiveDownloads()
{
    //return globalStat.numActive;
    std::async(std::launch::async, [=] { return globalStat.numActive;}); // async
}

std::string DownloadFile::getDownloadFilename(int gidIndex)
{   
    std::async(std::launch::async, [=] {  
        std::string substring;
        std::vector<std::string> substrings; // string vector

        aria2::DownloadHandle* handle = aria2::getDownloadHandle(session, gids[gidIndex]); // Get download handle of a particular download
        aria2::FileData filedata = handle->getFile(1); // local Path of file going to download
        aria2::deleteDownloadHandle(handle); // delete handle

        std::istringstream iss(filedata.path);

        // Seprate file name from full path
        while (std::getline(iss, substring, '/'))
        {
            substrings.push_back(substring);
        }

        // Return file name if available
        if (!substrings.empty())
        {
            return substrings.back();
        }
        else
        {
            return "No elements found in the path.";
        }
    });
}

int DownloadFile::getSessionDownloadSpeed()
{
    return globalStat.downloadSpeed;
}

int DownloadFile::getDownloadspeed(int gidIndex)
{
    aria2::DownloadHandle* handle = aria2::getDownloadHandle(session, gids[gidIndex]);
    int speed = handle->getDownloadSpeed();
    aria2::deleteDownloadHandle(handle);
    return speed;
}

int DownloadFile::getFileSize(int gidIndex)
{
    aria2::DownloadHandle* handle = aria2::getDownloadHandle(session, gids[gidIndex]);
    int TotalLength = handle->getTotalLength();
    aria2::deleteDownloadHandle(handle);
    return TotalLength;
}

int DownloadFile::getDownloadedSize(int gidIndex)
{
    aria2::DownloadHandle* handle = aria2::getDownloadHandle(session, gids[gidIndex]);
    int length = handle->getCompletedLength();
    aria2::deleteDownloadHandle(handle);
    return length;
}

/*************************************[ DOWNLOAD DIRECTORY ]*************************************/

// Return download directory of file to download
std::string DownloadFile::getDownloadDir(int gidIndex)
{
    aria2::DownloadHandle* handle = aria2::getDownloadHandle(session, gids[gidIndex]);
    std::string dir = handle->getDir();
    aria2::deleteDownloadHandle(handle);
    return dir;
}

int DownloadFile::closeSession()
{

    int exitStatus = aria2::sessionFinal(session);
    int iResult = aria2::libraryDeinit(); // Releases the global data
    return exitStatus;
}

//int main()
//{
//    std::vector<std::string> url = { "https://www.nhc.noaa.gov/pdf/NWS-NHC-1977-2.pdf" };
//     std::vector<std::string> url1 = {"https://www-2.dc.uba.ar/staff/becher/dragon.pdf"};
//    downloadFile dfile;
//     dfile.addUrl(url1);
//    dfile.addUrl(url);
//
//    std::cout << "downloading" << std::endl;
//
//    dfile.StartDownload();
//
//    dfile.closeSession();
//    std::cout << "done" << std::endl;
//    return EXIT_SUCCESS;
//}
