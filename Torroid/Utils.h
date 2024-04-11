#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace winrt::Torroid::Utils {
    std::string bytesToSize(const std::string& bytesStr);

    std::string FilePath_to_FileName(std::string filePath);

}