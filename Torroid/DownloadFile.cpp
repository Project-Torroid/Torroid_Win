#include "pch.h"
#include "DownloadFile.h"
#include <aria2/aria2.h>
#include <vector>
#include <sstream>
#include <winrt/Windows.Storage.h>
#include "logging.h"
#include "json.h"
#include <filesystem>

int downloadEventCallback(aria2::Session* session, aria2::DownloadEvent event, const aria2::A2Gid gid, void* userData)
{   
    int index = 0;
    bool get = false;
    switch (event)
    {
    case aria2::EVENT_ON_DOWNLOAD_START:
        /*std::cout << "DOWNLOAD STARTED" << std::endl;*/
        break;
    case aria2::EVENT_ON_DOWNLOAD_COMPLETE:
        //std::cerr << "COMPLETE" << std::endl;
        
        for (auto map : DownloadFile::DownloadInstance().jsonEntry.vDownloadEntries)
        {
            if (std::stoull(map["gid"]) == gid)
            {   
                get = true;
                break;
            }
            index++;
        }
        if ( get)
        {
            std::string totalSize = DownloadFile::DownloadInstance().jsonEntry.vDownloadEntries[index]["totalFileSize"];
            DownloadFile::DownloadInstance().jsonEntry.updateJsonOnDownloadComplete(index, totalSize);
        }

        break;
    case aria2::EVENT_ON_DOWNLOAD_ERROR:
        //std::cerr << "ERROR" << std::endl;
        break;
    }
    return 0;
}

DownloadFile* DownloadFile::instance_ = nullptr;

DownloadFile::DownloadFile() {
    
    // Initialize aria2 dependencies
    int iResult = aria2::libraryInit();
    if (iResult)
    {
        Logging::Error("iResult: aria2 initionalisation error");
    }
    
    // Set callback function which will be invoked when download event occurred
    config.downloadEventCallback = (aria2::DownloadEventCallback)downloadEventCallback;
    config.keepRunning = true;

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

DownloadFile& DownloadFile::DownloadInstance() {
    if (instance_ == nullptr) {
        instance_ = new DownloadFile();
    }
    return *instance_;
}

IAsyncAction DownloadFile::setupSession() {
    // Create session object
    session = aria2::sessionNew(options, config);

    // Global statistics of current aria2 session
    globalStat = aria2::getGlobalStat(session);
    return 0;
}

void DownloadFile::StartDownload()
{
    while (true)
    {
        int iResult = aria2::run(session, aria2::RUN_ONCE);
    }
}

int DownloadFile::addUrl(std::vector<std::string> uri)
{   
    aria2::A2Gid gid;

    // Check if file already present
    for (auto map : jsonEntry.vDownloadEntries) {
        if (map["url"] == uri[0]) 
        {
            if (std::filesystem::exists(map["filename"]))
            {
                if (map["downloaded"] == "True")
                {
                    Logging::Info(" already present");

                    // return if file already downloaded and present in last download directory
                    return 3;
                }

                // Add new URI to session to download file
                int iResult = aria2::addUri(session, &gid, uri, options);
                if (iResult < 0)
                {
                    /*std::cerr << "Failed to add download" << uri[0] << std::endl;*/
                }

                // return if file is present but not completely downloaded yet (paused)
                return 2;
            }

            // Add new URI to session to download file
            int iResult = aria2::addUri(session, &gid, uri, options);
            if (iResult < 0)
            {
                /*std::cerr << "Failed to add download" << uri[0] << std::endl;*/
            }
            updateJsonAndUI(gid, uri[0]);
            // return if file downloaded last time but not present in last download directory
            return 1;
        }
    }
    // Add new URI to session to download file
    int iResult = aria2::addUri(session, &gid, uri, options);
    if (iResult < 0)
    {
        /*std::cerr << "Failed to add download" << uri[0] << std::endl;*/
    }
    updateJsonAndUI(gid, uri[0]);

    // return if file is downloading for first time
    return EXIT_SUCCESS;
}

int DownloadFile::pause(int Index)
{
    aria2::A2Gid sGid = std::stoull(jsonEntry.vDownloadEntries[Index]["gid"]);
    return aria2::pauseDownload(session, sGid);
}

int DownloadFile::Canceldownload(int Index)
{
    aria2::A2Gid sGid = std::stoull(jsonEntry.vDownloadEntries[Index]["gid"]);
    aria2::DownloadHandle* handle = aria2::getDownloadHandle(session, sGid);
    int downloadedSize = handle->getCompletedLength();
    int percentage = handle->getTotalLength() > 0 ? (100 * downloadedSize / handle->getTotalLength()) : 0;
    aria2::deleteDownloadHandle(handle);
    jsonEntry.updateJsonOnPause(Index, std::to_string(downloadedSize), std::to_string(percentage));
    return aria2::removeDownload(session, sGid);
}

int DownloadFile::ResumeDownload(int Index)
{
    Canceldownload(Index);
    std::vector<std::string> uri;
    uri.push_back(jsonEntry.vDownloadEntries[Index]["url"]);
    addUrl(uri);
    return EXIT_SUCCESS;
}

void DownloadFile::onDownloadComplete(aria2::A2Gid gid)
{
    int Index = 0;
    // Check if file already present
    for (auto map : jsonEntry.vDownloadEntries) {
        if (std::stoull(map["gid"]) == gid )
        {
            break;
        }
        Index++;
    }
    jsonEntry.updateJsonOnDownloadComplete(Index,jsonEntry.vDownloadEntries[Index]["totalFileSize"]);
}

int DownloadFile::getSessionActiveDownloads()
{
    //return globalStat.numActive;
    return globalStat.numActive;
}

IAsyncAction DownloadFile::updateJsonAndUI(aria2::A2Gid gid, std::string url)
{   
    Sleep(1000);
    std::string substring;
    std::vector<std::string> substrings; // string vector
    bool getinfo = false;
    aria2::DownloadHandle* handle; 
    std::string size;

    while (!getinfo){
        handle = aria2::getDownloadHandle(session, gid); // Get download handle of a particular download
        if (handle->getTotalLength() != 0) {
            getinfo = true;
        }
        aria2::deleteDownloadHandle(handle); // delete handle

    }
    handle = aria2::getDownloadHandle(session, gid); // Get download handle of a particular download

    aria2::FileData filedata = handle->getFile(1); // local Path of file going to download
    int iSize = handle->getTotalLength(); // file total size
    size = std::to_string(iSize);
    aria2::deleteDownloadHandle(handle); // delete handle

    std::string sGid = std::to_string(gid);
    jsonEntry.json::addDownloadToJson(filedata.path, size, url, sGid);

    return EXIT_SUCCESS;
}

std::string DownloadFile::getDownloadFilename(int Index)
{   
    std::string substring;
    std::vector<std::string> substrings; // string vector

    std::istringstream iss(jsonEntry.vDownloadEntries[Index]["filename"]);

     //Seprate file name from full path
    while (std::getline(iss, substring, '/'))
    {
        substrings.push_back(substring);
    }

    // Return file name 
    return substrings.back();
}

int DownloadFile::getSessionDownloadSpeed()
{
    return globalStat.downloadSpeed;
}

int DownloadFile::getDownloadspeed(int Index)
{
    aria2::A2Gid sGid = std::stoull(jsonEntry.vDownloadEntries[Index]["gid"]);
    aria2::DownloadHandle* handle = aria2::getDownloadHandle(session, sGid);
    int speed = handle->getDownloadSpeed();
    aria2::deleteDownloadHandle(handle);
    return speed;
}

std::string DownloadFile::getFileSize(int Index)
{
    return jsonEntry.vDownloadEntries[Index]["totalFileSize"];
    
}

int DownloadFile::getDownloadedSize(int Index)
{
    aria2::A2Gid sGid = std::stoull(jsonEntry.vDownloadEntries[Index]["gid"]);
    aria2::DownloadHandle* handle = aria2::getDownloadHandle(session, sGid);
    int length = handle->getCompletedLength();
    aria2::deleteDownloadHandle(handle);
    return length;
}

// Return download directory of file to download
std::string DownloadFile::getDownloadDir(int Index)
{
    aria2::A2Gid sGid = std::stoull(jsonEntry.vDownloadEntries[Index]["gid"]);
    aria2::DownloadHandle* handle = aria2::getDownloadHandle(session, sGid);
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
//     std::vector<std::string> url1 = {"https://www.nhc.noaa.gov/pdf/1851_2007_hurr_poster.pdf"};
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
