#pragma once

#include <string>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

#include "../wikiPage/wikiPage.h"

#include "nlohmann/json.hpp"

using nlohmann::json;
using sharedWikiPage = std::shared_ptr<wikiPage*>;

namespace cache_utils
{
//public:
    extern unsigned long timesRevisited;

    bool isCached(const std::string& page);

    // Read/Write file
    void readFromCache(const std::string& page, wikiPage* links);
    bool cacheFile(const std::string& page, const json& content);
    bool cacheFile(const std::string& page, const std::vector<std::string>& content);
    bool cacheFile(const std::string& page, const std::vector<sharedWikiPage>& links);

    bool isPageVisited(const std::string& page, const uint8_t depth);
    void visitPage(const std::string &page, const uint8_t depth);

    void getLinksFromJson(const json& content, wikiPage* links);

//private:
    extern std::map<std::string, uint8_t> m_visitedPages;

    bool m_cacheDirExists();
    bool m_fileExists(const std::string& page);
    std::string m_returnFormattedName(const std::string& page);
}
