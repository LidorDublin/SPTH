//
// Created by lidor on 11/3/19.
//

#include "web_utils.h"
#include "../thread_utils/thread_utils.h"

/*
 * Populate the wikiPage* links' inner vector with pages
 */
void web_utils::getPageLinks(wikiPage* links)
{
    // Validity check
    if(!web_utils::m_isValidPage(links->getPage()))
        return;

    // If file is cached, read from cache
    if(cache_utils::isCached(links->getPage()))
        cache_utils::readFromCache(links->getPage(), links);

    // Integrate with WikiAPI to get the pages
    else
        cache_utils::getLinksFromJson(json::parse(web_utils::m_getPage(links->getPage())), links);
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
std::string web_utils::m_executeRequest(const std::string& page)
{
    curlpp::Easy request;
    std::stringstream content;

    request.setOpt(new curlpp::options::Url(web_utils::m_WIKI_URL + page));
    request.setOpt(cURLpp::Options::WriteStream(&content));

    request.perform();

    return content.str();
}

/*
 * Check if given page is valid (i.e not containing illegal character)
 */
bool web_utils::m_isValidPage(const std::string& page)
{
    return page.end() == std::find_if(page.begin(), page.end(), [](char ch){
        return web_utils::m_ILLEGAL_CHARACTERS.find(ch) != std::string::npos;
    });
}

std::string web_utils::m_getPage(std::string page)
{
    std::replace(page.begin(), page.end(), ' ', '_');

    for (int i = 0; i < web_utils::m_RETRY_COUNT; ++i)
    {
        try
        {
            return m_executeRequest(page);
        }
        catch (curlpp::LogicError& e)
        {
            thread_utils::coutThreadSafe << "LoginError: '" << e.what() << "' when running '" << __FUNCTION__ << "' with page='" << page << "'\n";
        }
        catch (curlpp::RuntimeError& e)
        {
            thread_utils::coutThreadSafe << "RuntimeError: '" << e.what() << "' when running '" << __FUNCTION__ << "' with page='" << page << "'\n";
        }
    }
    throw exceptions::NetworkError("Failed to retrieve page links from WikiPedia");
}
