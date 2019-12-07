//
// Created by lidor on 11/3/19.
//

#include "cache_utils.h"

unsigned long cache_utils::timesRevisited = 0;
std::map<std::string, uint8_t > cache_utils::m_visitedPages;

/*
 * Returns a boolean value indicating whether the caching directory exists
 */
bool cache_utils::m_cacheDirExists()
{
    struct stat info{};
    return !stat(".cache", &info);
}

/*
 * Returns a boolean value indicating whether the file exists in the caching directory
 */
bool cache_utils::m_fileExists(const std::string& page)
{
    struct stat info{};
    return !stat(cache_utils::m_returnFormattedName(page).c_str(), &info);
}

/*
 * Formats and returns the name of the file (as of for now, prepending '.cache/' to it's name)
 */
std::string cache_utils::m_returnFormattedName(const std::string &page)
{
    return std::string(".cache/") + page;
}

/*
 * Returns whether the file is cached
 */
bool cache_utils::isCached(const std::string& page)
{
    if(!cache_utils::m_cacheDirExists()) // Check if caching directory exists
        if(!mkdir(".cache", 0644))  // In case caching directory doesn't exist, create
        {
            throw exception_utils::FileError("Can't create caching directory");
        }

    return m_fileExists(page);
}

/*
 * Read and parse file from cache
 * *NOTE:* This functions assumes the file is in correct format
 */
void cache_utils::readFromCache(const std::string& page, wikiPage* links)
{
    if(!cache_utils::isCached(page))
    {
        std::stringstream msg;
        msg << "File not cached" + page << '"';

        throw exception_utils::FileError(msg.str().c_str());
    }

    std::ifstream file(cache_utils::m_returnFormattedName(page));
    if(!file.is_open())
    {
        std::stringstream msg;
        msg << "Could not open file '" + page << '"';

        throw exception_utils::FileError(msg.str().c_str());
    }

    std::string line;
    while(std::getline(file, line))
    {
        if(line.length())
            links->addLink(line);
    }
}

/*
 * Cache file, store it in caching directory
 * Returns:
 *      A boolean, indicating whether the caching was done successfully
 */
bool cache_utils::cacheFile(const std::string& page, const json& content)
{
    std::ofstream file(cache_utils::m_returnFormattedName(page));
    if(!file.is_open())
        return false;

    file << content.dump();
    return true;
}

/*
 * Cache file, store it in caching directory
 * Returns:
 *      A boolean, indicating whether the caching was done successfully
 */
bool cache_utils::cacheFile(const std::string& page, const std::vector<std::string>& content)
{
    std::ofstream file(cache_utils::m_returnFormattedName(page));
    if(!file.is_open())
        // Handle error
        return false;

    for(auto& link : content)
        file << link << '\n';

    return true;
}

/*
 * Cache file, store it in caching directory
 * Returns:
 *      A boolean, indicating whether the caching was done successfully
 */
bool cache_utils::cacheFile(const std::string& page, const std::vector<sharedWikiPage>& links)
{
    std::ofstream file(cache_utils::m_returnFormattedName(page));
    if(!file.is_open())
        // Handle error
        return false;

    for(auto& link : links)
        file << (*link).getPage() << '\n';

    return true;
}

/*
 * Parses a JSON object and stores all the links in the second parameter (wikiPage* links)
 */
void cache_utils::getLinksFromJson(const json& content, wikiPage* links)
{
     if(!content.contains("parse"))
         throw exception_utils::BadJson("Bad JSON received, no \"parse\" Attribute");

    const json& temp = content.at("parse");
    if(!temp.contains("links"))
        throw exception_utils::BadJson("Bad JSON received, no \"links\" Attribute");


    // Iterate over the JSON object
    auto&& items = temp.at("links").items();
    for(auto&& [idx, link] : items)
    {
        // if the entry contains "exists" key, it means the page exists. If not, the page doesn't exist
        // "ns" key means namespace. There are many namespaces in the WikiAPI but only namespace 0 is interesting here.
        // Further reading:
        // https://stackoverflow.com/questions/40579942/what-do-the-parameters-in-wikipedia-api-response-mean
        // https://en.wikipedia.org/wiki/Wikipedia:Namespace
        if(!link.contains("exists") || link.at("ns").get<int>())
            continue;

        // Iterate over the results, store only the page itself
        for(auto&& [key, value] : link.items())
        {
            if(key == "*" && value.is_string())
                links->addLink(value.get<std::string>());
        }
    }

    cache_utils::cacheFile(links->getPage(), links->getLinks());
}

/*
 * Returns whether the page was visited in a higher depth
 */
bool cache_utils::isPageVisited(const std::string& page, const uint8_t depth)
{
    auto it = cache_utils::m_visitedPages.find(page);
    if(it != cache_utils::m_visitedPages.end() && it->second <= depth)
    {
        timesRevisited++;
        return true;
    }

    cache_utils::visitPage(page, depth);
    return false;
}

/*
 * Stores the page and the depth in a key-value map object
 */
void cache_utils::visitPage(const std::string &page, const uint8_t depth)
{
    cache_utils::m_visitedPages[page] = depth;
}
