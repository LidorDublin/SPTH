//
// Created by lidor on 11/3/19.
//

#include <iostream>
#include "cache_utils.h"

namespace cache_utils
{
    bool _cacheDirExists()
    {
        // Check if can be removed
        struct stat info{};
        return !stat(".cache", &info);
    }

    bool _fileExists(const std::string& page)
    {
        // Check if can be removed
        struct stat info{};
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

    void readFromCache(const std::string& page, wikiPage* links)
    {
        if(!cache_utils::isCached(page))
//        throw std::exception("File not cached");
            throw std::exception();

        std::ifstream file(cache_utils::_returnFormattedName(page));
        if(!file.is_open())
            // Handle error
            throw std::exception();

        std::string line;
        while(std::getline(file, line))
        {
            if(line.length())
                links->addLink(line);
        }
    }

    bool cacheFile(const std::string& page, const json& content)
    {
        std::ofstream file(cache_utils::_returnFormattedName(page));
        if(!file.is_open())
            // Handle error
            return false;

        file << content.dump();
        return true;
    }

    bool cacheFile(const std::string& page, std::vector<std::string>& content)
    {
        std::ofstream file(cache_utils::_returnFormattedName(page));
        if(!file.is_open())
            // Handle error
            return false;

        for(auto& link : content)
        {
            file << link << '\n';
        }
        return true;
    }

    bool cacheFile(const std::string& page, const std::vector<wikiPage*>& links)
    {
        std::ofstream file(cache_utils::_returnFormattedName(page));
        if(!file.is_open())
            // Handle error
            return false;

        for(wikiPage* link : links)
            file << link->getPage() << '\n';

        return true;
    }

    std::string _returnFormattedName(const std::string &page)
    {
        return std::string(".cache/") + page;
    }

    void getLinksFromJson(const json& content, wikiPage* links)
    {
         if(!content.contains("parse"))
//              Handle exception
             throw std::exception();
//            throw std::exception("JSON not in regular format");

        json temp = content.at("parse");
        if(!temp.contains("links"))
//              Handle exception
            throw std::exception();
//            throw std::exception("JSON not in regular format");


        for(auto&& [idx, link] : temp.at("links").items())
        {
            if(!link.contains("exists") || link.at("ns").get<int>())
                continue;

            for(auto&& [key, value] : link.items())
            {
                if(key == "*" && value.is_string())
                    links->addLink(value.get<std::string>());
            }
        }

        cache_utils::cacheFile(links->getPage(), links->getLinks());
    }
}
