//
// Created by lidor on 11/3/19.
//

#pragma once

#include <string>
#include <sys/stat.h>
#include <fstream>
#include <sstream>

#include "nlohmann/json.hpp"

using nlohmann::json;

namespace cache_utils
{
    bool _cacheDirExists(void);
    bool _fileExists(const std::string& page);
    std::string _returnFormattedName(const std::string& page);

    bool isCached(const std::string& page);

    std::string readFromCache(const std::string& page);
    bool cacheFile(const std::string& page, const json& content);
}
