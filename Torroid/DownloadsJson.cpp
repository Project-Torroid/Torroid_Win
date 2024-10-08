#include "pch.h"
#include "DownloadsJson.h"
#include "logging.h"
#include "Utils.h"
#include <fstream>
#include <string>
#include <regex>
#include <filesystem>
#include <sstream>

using namespace winrt::Torroid;

DownloadsJson* DownloadsJson::jsonInstance_ = nullptr;

DownloadsJson::DownloadsJson()
{   
    std::string appDataPath = "C:\\Torroid\\";

    // Check if the directory already exists
    if (!std::filesystem::exists(appDataPath)) {
        // Create the Torroid directory
        std::filesystem::create_directory(appDataPath);
    }

    jsonFilePath = appDataPath + jsonfilename;
    std::ifstream infile(jsonFilePath);
    if (!infile.good())
    {
        std::ofstream newfile(jsonFilePath);
        newfile.close();
    }
    GetJsonToVector();
}

DownloadsJson& DownloadsJson::jsonInstance() {
    if (jsonInstance_ == nullptr) {
        jsonInstance_ = new DownloadsJson();
    }
    return *jsonInstance_;
}

size_t DownloadsJson::size()
{
    return vDownloadEntries.size(); // Get size of vector
}

size_t DownloadsJson::getIndex(std::string filename)
{
    for (size_t index = 0; index < vDownloadEntries.size(); index++)
    {
        std::string f_name = Utils::FilePath_to_FileName(DownloadsJson::filename(index));
        if(f_name == filename)
        {
			return index;
        }
    }
}

std::map<std::string, std::string> DownloadsJson::getKeyValuePairs(std::string &json)
{
    std::map<std::string, std::string> dataKeyValuePair;
    std::regex keyValueRegex("\"([^\"]+)\"\\s*:\\s*\"([^\"]+)\"");
    std::smatch match;

    while (std::regex_search(json, match, keyValueRegex))
    {
        dataKeyValuePair[match[1]] = match[2];
        json = match.suffix().str();
    }
    return dataKeyValuePair;
}

int DownloadsJson::GetJsonToVector()
{
    // Open the file to read
    std::ifstream inputFile(jsonFilePath);

    // Read the content of the file into a string
    std::string fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    // Close the file
    inputFile.close();

    // Process the JSON content
    size_t pos = 0;
    while ((pos = fileContent.find("{", pos)) != std::string::npos)
    {
        // Find the beginning of a JSON object
        size_t endPos = fileContent.find("}", pos);
        if (endPos == std::string::npos)
        {
            // std::cerr << "Error: Invalid JSON format." << std::endl;
            return EXIT_FAILURE;
        }

        // Extract the JSON object
        std::string jsonObject = fileContent.substr(pos, endPos - pos + 1);

        // Extract key-value pairs
        std::map<std::string, std::string> data = getKeyValuePairs(jsonObject);

        vDownloadEntries.push_back(data);

        // Move to the next position for searching
        pos = endPos + 1;
    }
    return EXIT_SUCCESS;
}

void DownloadsJson::addDownloadToJson(
    std::string filename,      // File name
    std::string totalFileSize, // Total file size in MB
    std::string url,           // URL of file to download
    std::string gid            // Download file gid
)
{
    std::string json = "[\n\t{\n"
        "\t\t\"fileDownloadPercentage\": \"0"
        "\",\n\t\t\"fileStatus\": \"Pause\",\n\t\t\"fileSizeCurrent\": \"0"
        "\",\n\t\t\"filename\": \"" +
        filename +
        "\",\n\t\t\"gid\": \"" + gid +
        "\",\n\t\t\"fileSizeTotal\": \"" + totalFileSize +
        "\",\n\t\t\"url\": \"" + url + "\"\n\t}\n]";


    std::ifstream infile(jsonFilePath);

    // Check if the file exist or is empty
    infile.seekg(0, std::ios::end);
    if (infile.tellg() == 0)
    {
        // If file is empty (new file) make structure of json with new entry
        infile.close();
        std::ofstream outfile(jsonFilePath);
        outfile << json;
        outfile.close();
    }
    else
    {

        std::vector<std::string> vLines; // Vector to contains lines from file
        std::string line;

        // Move pointer to start
        infile.seekg(0, std::ios::beg);

        // Read all lines from the file and put in vector
        while (std::getline(infile, line))
        {
            vLines.push_back(line);
        }

        // Add new Download entry to vector
        vLines.insert(vLines.begin() + 1, "\t{");
        vLines.insert(vLines.begin() + 2, "\t\t\"fileDownloadPercentage\": \"0\",");
        vLines.insert(vLines.begin() + 3, "\t\t\"fileStatus\": \"Pause\",");
        vLines.insert(vLines.begin() + 4, "\t\t\"fileSizeCurrent\": \"0\",");
        vLines.insert(vLines.begin() + 5, "\t\t\"filename\": \"" + filename + "\",");
        vLines.insert(vLines.begin() + 6, "\t\t\"gid\": \"" + gid + "\",");
        vLines.insert(vLines.begin() + 7, "\t\t\"fileSizeTotal\": \"" + totalFileSize + "\",");
        vLines.insert(vLines.begin() + 8, "\t\t\"url\": \"" + url + "\"");
        vLines.insert(vLines.begin() + 9, "\t},");

        // Close file
        infile.close();

        // Write all lines back to the file
        std::ofstream outFile(jsonFilePath);
        for (const auto &updatedLine : vLines)
        {
            outFile << updatedLine << '\n';
        }
        outFile.close();
    }
    vDownloadEntries.insert(vDownloadEntries.begin(), getKeyValuePairs(json));
    fileStatus(0, "Downloading");
    
}

void DownloadsJson::updateJsonOnPause(size_t index, std::string newDownloadedSize, std::string newdownloadPercent)
{
    size_t lineToUpdate = (2 + (index * 9)); // line index to update
    std::ifstream inFile(jsonFilePath);
    std::vector<std::string> vLines;
    std::string line;

    // Read all lines from the file and put in vector
    while (std::getline(inFile, line))
    {
        vLines.push_back(line);
    }

    // Update download state on pause
    vDownloadEntries[index]["fileDownloadPercentage"] = newdownloadPercent;
    vLines[lineToUpdate] = "\t\t\"fileDownloadPercentage\": \"" + newdownloadPercent + "\",";

    vLines[lineToUpdate + 1] = "\t\t\"fileStatus\": \"Pause\",";
    vDownloadEntries[index]["fileStatus"] = "Pause";
    
    vDownloadEntries[index]["fileSizeCurrent"] = newDownloadedSize;
    vLines[lineToUpdate + 2] = "\t\t\"fileSizeCurrent\": \"" + newDownloadedSize + "\",";

    // Write all lines back to the file
    std::ofstream outFile(jsonFilePath);
    for (const auto &updatedLine : vLines)
    {
        outFile << updatedLine << '\n';
    }
    outFile.close();
}

void DownloadsJson::updateJsonOnDownloadComplete(size_t index, std::string totalDownloadedSize)
{
    size_t lineToUpdate = (2 + (index * 9)); // line index to update

    std::ifstream inFile(jsonFilePath);
    std::vector<std::string> lines;
    std::string line;

    // Read all lines from the file and put in vector
    while (std::getline(inFile, line))
    {
        lines.push_back(line);
    }

    vDownloadEntries[index]["fileDownloadPercentage"] = "100";
    lines[lineToUpdate] = "\t\t\"fileDownloadPercentage\": \"100\",";

    vDownloadEntries[index]["fileStatus"] = "Complete";
    lines[lineToUpdate + 1] = "\t\t\"fileStatus\": \"Complete\",";

    vDownloadEntries[index]["fileSizeCurrent"] = totalDownloadedSize;
    lines[lineToUpdate + 2] = "\t\t\"fileSizeCurrent\": \"" + totalDownloadedSize + "\",";

    // Write all lines back to the file
    std::ofstream outFile(jsonFilePath);
    for (const auto &updatedLine : lines)
    {
        outFile << updatedLine << '\n';
    }

    // Close file
    outFile.close();
}

void DownloadsJson::RemoveDownloadEntry(int index)
{
    std::ifstream inFile(jsonFilePath);
    std::vector<std::string> lines;
    std::string line;

    // Read all lines from the file and put in vector
    while (std::getline(inFile, line))
    {
        lines.push_back(line);
    }

    if (lines.size() > (10 + (index * 9)))
    {
        // Delete download element from json file and json object entry vector
        lines.erase(lines.begin() + 1+ (index * 9), lines.begin() + 10 + (index * 9));  // delete from file
        vDownloadEntries.erase(vDownloadEntries.begin() + index); // delete from vector

        // Write all lines back to the file
        std::ofstream outFile(jsonFilePath);
        for (const auto &updatedLine : lines)
        {
            outFile << updatedLine << '\n';
        }
        outFile.close();
    }
}

void DownloadsJson::writeToJsonOnexit()
{
    // Construct the JSON string
    std::string jsonString = "[\n";
    for (const auto &entry : vDownloadEntries)
    {
        jsonString += "\t{\n";
        for (auto it = entry.begin(); it != entry.end(); ++it)
        {
            jsonString += "\t\t\"" + it->first + "\": \"" + it->second + "\",\n";
        }
        jsonString.pop_back();
        jsonString.pop_back();
        jsonString += "\n\t},\n";
    }
    jsonString.pop_back();
    jsonString.pop_back();
    jsonString += "\n]";

    // Write the JSON string to a file
    std::ofstream outputFile(jsonFilePath);
    outputFile << jsonString;
    outputFile.close();
}

std::string DownloadsJson::fileDownloadPercentage(int Index)
{
    return vDownloadEntries[Index]["fileDownloadPercentage"];
}

void DownloadsJson::fileDownloadPercentage(int Index, std::string fileDownloadPercentage)
{
    vDownloadEntries[Index]["fileDownloadPercentage"] = fileDownloadPercentage;
}

std::string DownloadsJson::fileStatus(int Index)
{
    return vDownloadEntries[Index]["fileStatus"];
}

void DownloadsJson::fileStatus(int Index, std::string fileStatus)
{
    vDownloadEntries[Index]["fileStatus"] = fileStatus;
}

std::string DownloadsJson::fileSizeCurrent(int Index)
{
    return vDownloadEntries[Index]["fileSizeCurrent"];
}

void DownloadsJson::fileSizeCurrent(int Index, std::string fileSizeCurrent)
{
    vDownloadEntries[Index]["fileSizeCurrent"] = fileSizeCurrent;
}

std::string DownloadsJson::filename(int Index)
{
    return vDownloadEntries[Index]["filename"];
}

void DownloadsJson::filename(int Index, std::string filename)
{
    vDownloadEntries[Index]["filename"] = filename;
}

std::string DownloadsJson::gid(int Index)
{
    return vDownloadEntries[Index]["gid"];
}

void DownloadsJson::gid(int Index, std::string gid)
{
    vDownloadEntries[Index]["gid"] = gid;
}

std::string DownloadsJson::fileSizeTotal(int Index)
{
    return vDownloadEntries[Index]["fileSizeTotal"];
}

void DownloadsJson::fileSizeTotal(int Index, std::string fileSizeTotal)
{
    vDownloadEntries[Index]["fileSizeTotal"] = fileSizeTotal;
}

std::string DownloadsJson::fileUrl(int Index)
{
    return vDownloadEntries[Index]["url"];
}

void DownloadsJson::fileUrl(int Index, std::string fileUrl)
{
    vDownloadEntries[Index]["url"] = fileUrl;
}
