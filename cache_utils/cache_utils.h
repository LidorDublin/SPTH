//
// Created by lidor on 11/3/19.
//

#pragma once

#include <string>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <vector>

#include <algorithm>

#include "../tree/tree.h"

#include "nlohmann/json.hpp"

using nlohmann::json;

namespace cache_utils
{
    bool _cacheDirExists();
    bool _fileExists(const std::string& page);
    std::string _returnFormattedName(const std::string& page);

    bool isCached(const std::string& page);

    void readFromCache(const std::string& page, tree* links);
    bool cacheFile(const std::string& page, const json& content);
    bool cacheFile(const std::string& page, const std::vector<std::string>& content);
    bool cacheFile(const std::string& page, const std::vector<tree*>& links);

    void getLinksFromJson(const json& content, tree*);
}
