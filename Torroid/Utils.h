#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace winrt::Torroid::Utils {
    std::string bytesToSize(const std::string& bytesStr);

    std::string FilePath_to_FileName(std::string filePath);

    std::string FilePath_to_FolderPath(std::string filePath);

    size_t FileName_to_Index(hstring const& fileName);

}