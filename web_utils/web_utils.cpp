//
// Created by lidor on 11/3/19.
//

#include "web_utils.h"

namespace web_utils
{
    json getPageLinks(const std::string& page)
    {
        if(cache_utils::isCached(page))
        {
            std::cout << "Cached\n";
            return json::parse(cache_utils::readFromCache(page));
        }

        json content = json::parse(web_utils::_requestPage(page));
        cache_utils::cacheFile(page, content);

        return content;
    }

    std::string _requestPage(const std::string& page)
    {
        std::string _page = page;
        std::replace(_page.begin(), _page.end(), ' ', '_');
        try
        {
            curlpp::Cleanup cleaner;
            curlpp::Easy request;

            std::stringstream content;

            request.setOpt(new curlpp::options::Url(web_utils::_WIKI_URL + _page));
            request.setOpt(cURLpp::Options::WriteStream(&content));

            request.perform();

            return content.str();
        }
        catch ( curlpp::LogicError & e )
        {
            std::cout << e.what() << std::endl;
        }
        catch ( curlpp::RuntimeError & e )
        {
            std::cout << e.what() << std::endl;
        }
        return "Error";
    }
}
