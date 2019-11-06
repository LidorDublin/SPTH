//
// Created by lidor on 11/3/19.
//

#include "web_utils.h"

namespace web_utils
{
    void getPageLinks(tree* links)
    {
        if(cache_utils::isCached(links->getPage()))
            cache_utils::readFromCache(links->getPage(), links);

        else
            cache_utils::getLinksFromJson(json::parse(web_utils::_requestPage(links->getPage())), links);
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
        catch (curlpp::LogicError& e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (curlpp::RuntimeError& e)
        {
            std::cout << e.what() << std::endl;
        }
        return "Error";
    }
}
