#include "pch.h"
#include "json.h"
#include "logging.h"
#include <fstream>
#include <string>
#include <regex>

json::json()
{   
    //TODO: app data path
    std::string appDataPath = "C:\\test\\";
    jsonFilePath = appDataPath + jsonfilename;
    std::ifstream infile(jsonFilePath);
    if (!infile.good())
    {
        std::ofstream newfile(jsonFilePath);
        newfile.close();
    }
    GetJsonToVector();
}

std::map<std::string, std::string> json::getKeyValuePairs(std::string &json)
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

int json::GetJsonToVector()
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

void json::addDownloadToJson(
    std::string filename,      // File name
    std::string totalFileSize, // Total file size in MB
    std::string url,           // URL of file to download
    std::string gid            // Download file gid
)
{
    std::string json = "[\n\t{\n"
        "\t\t\"downloadPercent\": \"0"
        "\",\n\t\t\"downloaded\": \"False\",\n\t\t\"downloadedSize\": \"0"
        "\",\n\t\t\"filename\": \"" +
        filename +
        "\",\n\t\t\"gid\": \"" + gid +
        "\",\n\t\t\"totalFileSize\": \"" + totalFileSize +
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
        vLines.insert(vLines.begin() + 2, "\t\t\"downloadPercent\": \"0\",");
        vLines.insert(vLines.begin() + 3, "\t\t\"downloaded\": \"False\",");
        vLines.insert(vLines.begin() + 4, "\t\t\"downloadedSize\": \"0\",");
        vLines.insert(vLines.begin() + 5, "\t\t\"filename\": \"" + filename + "\",");
        vLines.insert(vLines.begin() + 6, "\t\t\"gid\": \"" + gid + "\",");
        vLines.insert(vLines.begin() + 7, "\t\t\"totalFileSize\": \"" + totalFileSize + "\",");
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
}

void json::updateJsonOnPause(size_t index, std::string newDownloadedSize, std::string newdownloadPercent)
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

    Logging::Info("updating");
    // Update download state on pause
    vDownloadEntries[index]["downloadedSize"] = newDownloadedSize;
    vLines[lineToUpdate + 2] = "\t\t\"downloadedSize\": \"" + newDownloadedSize + "\",";
    vDownloadEntries[index]["downloadPercent"] = newdownloadPercent;
    vLines[lineToUpdate] = "\t\t\"downloadPercent\": \"" + newdownloadPercent + "\",";

    // Write all lines back to the file
    std::ofstream outFile(jsonFilePath);
    for (const auto &updatedLine : vLines)
    {
        outFile << updatedLine << '\n';
    }
    Logging::Info("updated");
    outFile.close();
}

void json::updateJsonOnDownloadComplete(size_t index, std::string totalDownloadedSize)
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

    vDownloadEntries[index]["downloadPercent"] = "100";
    lines[lineToUpdate] = "\t\t\"downloadPercent\": \"100\",";

    vDownloadEntries[index]["downloaded"] = "True";
    lines[lineToUpdate + 1] = "\t\t\"downloaded\": \"True\",";

    vDownloadEntries[index]["downloadedSize"] = totalDownloadedSize;
    lines[lineToUpdate + 2] = "\t\t\"downloadedSize\": \"" + totalDownloadedSize + "\",";

    // Write all lines back to the file
    std::ofstream outFile(jsonFilePath);
    for (const auto &updatedLine : lines)
    {
        outFile << updatedLine << '\n';
    }

    // Close file
    outFile.close();
}

void json::deleteDownload(size_t index)
{
    std::ifstream inFile(jsonFilePath);
    std::vector<std::string> lines;
    std::string line;

    // Read all lines from the file and put in vector
    while (std::getline(inFile, line))
    {
        lines.push_back(line);
    }

    if (lines.size() > (10 + (index * 8)))
    {
        // Delete download element from json file and json object entry vector
        lines.erase(lines.begin() + 1 + (index * 7), lines.begin() + 9 + (index * 7));
        vDownloadEntries.erase(vDownloadEntries.begin() + index);

        // Write all lines back to the file
        std::ofstream outFile(jsonFilePath);
        for (const auto &updatedLine : lines)
        {
            outFile << updatedLine << '\n';
        }
        outFile.close();
    }
}

void json::writeToJsonOnexit()
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
