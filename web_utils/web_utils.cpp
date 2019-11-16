//
// Created by lidor on 11/3/19.
//

#include "web_utils.h"

/*
 * Populate the wikiPage* links' inner vector with pages
 */
void web_utils::getPageLinks(wikiPage* links)
{
    // Validy check
    if(!web_utils::_isValidPage(links->getPage()))
        return;

    // If file is cached, read from cache
    if(cache_utils::isCached(links->getPage()))
        cache_utils::readFromCache(links->getPage(), links);

    // Integrate with WikiAPI to get the pages
    else
        cache_utils::getLinksFromJson(json::parse(web_utils::_requestPage(links->getPage())), links);
}

/*
 * Inner function
 * Integrates with WikiAPI to get all the pages from the given page
 * Args:
 *      const std::string& page: The page to get all the pages from
 *
 * Returns:
 *      The result from WikiAPI, as-is
 */
std::string web_utils::_requestPage(const std::string& page)
{
    std::string _page = page;
    std::replace(_page.begin(), _page.end(), ' ', '_');
    try
    {
        curlpp::Cleanup cleaner;
        curlpp::Easy request;

        std::stringstream content;

        request.setOpt(new curlpp::options::Url(web_utils::m_WIKI_URL + _page));
        request.setOpt(cURLpp::Options::WriteStream(&content));

        request.perform();

        return content.str();
    }
    catch (curlpp::LogicError& e)
    {
        std::cout << e.what() << '\n';
    }
    catch (curlpp::RuntimeError& e)
    {
        std::cout << e.what() << '\n';
    }
    return "Error";
}

/*
 * Check if given page is valid (i.e not containing illegal character)
 */
bool web_utils::_isValidPage(const std::string& page)
{
    return page.end() == std::find_if(page.begin(), page.end(), [](char ch){
        return web_utils::m_ILLEGAL_CHARACTERS.find(ch) != std::string::npos;
    });
}
