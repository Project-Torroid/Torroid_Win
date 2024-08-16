#include "pch.h"
#include "DownloadFile.h"
#include "DownloadsJson.h"
#include "logging.h"

#include <filesystem>
#include <winrt/Windows.Storage.h>

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
        
        // Get index using gid
        for (std::map<std::string, std::string> map : DownloadsJson::jsonInstance().vDownloadEntries)
        {
            if (std::stoull(map["gid"]) == gid)
            {   
                get = true;
                break;
            }
            index++;
        }

        // Write to JSON
        if (get)
        {
            std::string totalSize = DownloadsJson::jsonInstance().fileSizeTotal(index);
            DownloadsJson::jsonInstance().updateJsonOnDownloadComplete(index, totalSize);
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

    // Initialize JSON
    DownloadsJson::jsonInstance();
    
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

int DownloadFile::setupSession() {
    // Create session object
    session = aria2::sessionNew(options, config);

    return 0;
}

void DownloadFile::StartDownload()
{
    while (true)
    {
        aria2::run(session, aria2::RUN_ONCE);
    }
}

int DownloadFile::addUrl(std::vector<std::string> uri)
{   
    aria2::A2Gid gid;
    int iResult;

    // Check if file already present
    for (std::map<std::string, std::string> map : DownloadsJson::jsonInstance().vDownloadEntries) {
        if (map["url"] == uri[0]) 
        {
            if (std::filesystem::exists(map["filename"]))
            {
                if (map["fileStatus"] == "Complete")
                {
                    Logging::Info(" already present");

                    // return if file already downloaded and present in last download directory
                    return 3;
                }

                // Add new URI to session, to download file
                iResult = aria2::addUri(session, &gid, uri, options);
                if (iResult < 0)
                {
                }

                DownloadsJson::jsonInstance().fileStatus(0, "Downloading");
                DownloadsJson::jsonInstance().gid(0, std::to_string(gid));

                // return if file is present but not completely downloaded yet (paused)
                return 2;
            }

            // Add new URI to session to download file
            iResult = aria2::addUri(session, &gid, uri, options);
            if (iResult < 0)
            {
            }
            updateJson(gid, uri[0]);
            // return 1 if file downloaded last time but not present in last download directory
            return 1;
        }
    }

    // Add new URI to session to download file
    iResult = aria2::addUri(session, &gid, uri, options);

    if (iResult < 0)
    {
    }
    updateJson(gid, uri[0]);

    // return if file is downloading for first time
    return EXIT_SUCCESS;
}

void DownloadFile::pause(int Index)
{
    aria2::A2Gid sGid = std::stoull(DownloadsJson::jsonInstance().gid(Index));
    int64_t size = getCurrentFileSize(Index);
    aria2::removeDownload(session, sGid);
    std::string f_totalSize = DownloadsJson::jsonInstance().fileSizeTotal(Index);
    double d_percentage = static_cast<double>(size)/ std::stod(f_totalSize) * 100;
    DownloadsJson::jsonInstance().updateJsonOnPause(Index,std::to_string(size ),std::to_string(static_cast<int>(d_percentage)));
}

int DownloadFile::removeDownload(int Index, bool deletefromdisk)
{
    std::string filePath = DownloadsJson::jsonInstance().filename(Index);
    std::string aria2FilePath = filePath + ".aria2";

    if (deletefromdisk)
    {
        if (std::filesystem::exists(aria2FilePath))
        {
            remove(aria2FilePath.c_str());
        }
        remove(filePath.c_str());
    }
    DownloadsJson::jsonInstance().RemoveDownloadEntry(Index); // delete entry fron json
    return EXIT_SUCCESS;
}

int DownloadFile::ResumeDownload(int Index)
{
    std::string url = DownloadsJson::jsonInstance().fileUrl(Index);
    std::vector<std::string> vUrl;
    vUrl.push_back(url);
    int iRresult = addUrl(vUrl);
    return iRresult;
}

// Update json on download complete event
void DownloadFile::onDownloadComplete(aria2::A2Gid gid)
{
    int Index = 0;

    // Check if file already present
    for (std::map<std::string, std::string> map : DownloadsJson::jsonInstance().vDownloadEntries) {
        if (std::stoull(map["gid"]) == gid )
        {
            break;
        }
        Index++;
    }
    DownloadsJson::jsonInstance().updateJsonOnDownloadComplete(Index, DownloadsJson::jsonInstance().fileSizeTotal(Index));
}

// Return number of active downloads
int DownloadFile::getSessionActiveDownloads()
{
    return aria2::getGlobalStat(session).numActive;
}

int DownloadFile::updateJson(aria2::A2Gid gid, std::string url)
{   
    Sleep(1000);
    std::string substring;
    std::vector<std::string> substrings; // string vector
    bool getinfo = false;
    aria2::DownloadHandle* handle; 
    std::string size;

    // Wait to get download file stats
    while (!getinfo){
        handle = aria2::getDownloadHandle(session, gid); // Get download handle of a particular download
        if (handle->getTotalLength() != 0) {
            getinfo = true;
        }
        aria2::deleteDownloadHandle(handle); // delete handle

    }
    handle = aria2::getDownloadHandle(session, gid); // Get download handle of a particular download

    aria2::FileData filedata = handle->getFile(1); // local Path of file going to download
    int64_t iSize = handle->getTotalLength(); // file total size
    size = std::to_string(iSize);
    aria2::deleteDownloadHandle(handle); // delete handle

    DownloadsJson::jsonInstance().addDownloadToJson(filedata.path, size, url, std::to_string(gid));

    return EXIT_SUCCESS;
}

// Return file name
std::string DownloadFile::getDownloadFilename(aria2::A2Gid gid)
{
    std::string substring;
    std::vector<std::string> substrings; // string vector

    aria2::DownloadHandle*  handle = aria2::getDownloadHandle(session, gid); // Get download handle of a particular download
    aria2::FileData filedata = handle->getFile(1); // local Path of file going to download
    aria2::deleteDownloadHandle(handle); // delete handle

    std::istringstream iss(filedata.path);

    //Seprate file name from full path
    while (std::getline(iss, substring, '/'))
    {
        substrings.push_back(substring);
    }

    // Return file name 
    return substrings.back();
}

// Return overall download speed
int DownloadFile::getSessionDownloadSpeed()
{
    return aria2::getGlobalStat(session).downloadSpeed;
}

// Return download speed for a particular Download
int DownloadFile::getDownloadspeed(int Index)
{
    aria2::A2Gid sGid = std::stoull(DownloadsJson::jsonInstance().gid(Index));
    aria2::DownloadHandle* handle = aria2::getDownloadHandle(session, sGid);
    int speed = handle->getDownloadSpeed();
    aria2::deleteDownloadHandle(handle);
    return speed;
}

// Return total size of file to download
int64_t DownloadFile::getTotalFileSize(int Index)
{
    aria2::A2Gid sGid = std::stoull(DownloadsJson::jsonInstance().gid(Index)); // Get gid from JSON
    aria2::DownloadHandle* handle = aria2::getDownloadHandle(session, sGid); // Get download handle of a particular download
    int64_t totalSize = handle->getTotalLength(); // Get file total size
    aria2::deleteDownloadHandle(handle); // delete handle
    return totalSize;
}

// Returns the completed length of this download in bytes.
int64_t DownloadFile::getCurrentFileSize(int Index)
{
    aria2::A2Gid sGid = std::stoull(DownloadsJson::jsonInstance().gid(Index)); // Get gid from JSON
    aria2::DownloadHandle* handle = aria2::getDownloadHandle(session, sGid); // Get download handle of a particular download
    int64_t length = handle->getCompletedLength(); // get completed length
    aria2::deleteDownloadHandle(handle);
    return length;
}

// Return download directory of file to download
std::string DownloadFile::getDownloadDir(int Index)
{
    aria2::A2Gid sGid = std::stoull(DownloadsJson::jsonInstance().gid(Index));
    aria2::DownloadHandle* handle = aria2::getDownloadHandle(session, sGid);
    std::string dir = handle->getDir();
    aria2::deleteDownloadHandle(handle);
    return dir;
}

// Destructor
DownloadFile::~DownloadFile()
{
    aria2::sessionFinal(session);
    aria2::libraryDeinit(); // Releases the global data
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
