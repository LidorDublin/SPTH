//
// Created by lidor on 11/3/19.
//

#include "cache_utils.h"

namespace cache_utils
{
    bool _cacheDirExists()
    {
        // Check if can be removed
        struct stat info;
        return !stat(".cache", &info);
    }

    bool _fileExists(const std::string& page)
    {
        // Check if can be removed
        struct stat info;
        return !stat(cache_utils::_returnFormattedName(page).c_str(), &info);
    }

    bool isCached(const std::string& page)
    {
        if(!cache_utils::_cacheDirExists()) // Check if caching directory exists
            if(!mkdir(".cache", 0644))  // In case caching directory doesn't exist, create
//            throw std::exception("Can't create caching directory");
                throw std::exception();

        return _fileExists(page);
    }

    std::string readFromCache(const std::string& page)
    {
        if(!cache_utils::isCached(page))
//        throw std::exception("File not cached");
            throw std::exception();

        std::ifstream file(cache_utils::_returnFormattedName(page));
        std::stringstream buf;

        buf << file.rdbuf();

        return buf.str();
    }

    bool cacheFile(const std::string& page, const json& content)
    {
        std::ofstream file(cache_utils::_returnFormattedName(page));
        if(!file.is_open())
        {
            // Handle error
            return false;
        }

        file << content.dump();
        return true;
    }

    std::string _returnFormattedName(const std::string &page)
    {
        return (std::string(".cache/") + page + ".json");
    }
}
