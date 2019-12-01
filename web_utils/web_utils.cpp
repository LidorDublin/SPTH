//
// Created by lidor on 11/3/19.
//

#include <iomanip>
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
        cache_utils::getLinksFromJson(
                json::parse(web_utils::m_getPage(web_utils::m_WIKI_GET_PAGE + links->getPage())), links
        );
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
std::string web_utils::m_executeRequest(const std::string& url)
{
    curlpp::Easy request;
    std::stringstream content;

    request.setOpt(new curlpp::options::Url(url));
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
    throw exception_utils::NetworkError("Failed to retrieve page links from WikiPedia");
}

std::string web_utils::getRandomPage()
{
    std::string page;

    for (int i = 0; i < web_utils::m_RETRY_COUNT; ++i)
    {
        try
        {
            // A Loop to get a random WikiPedia page as a starting point
            // The loop is required in order to try again and again until a starting point found
            do
            {
                // Parse JSON returned from m_executeRequest
                json j(json::parse(web_utils::m_executeRequest(web_utils::m_WIKI_RANDOM_PAGE.data())));

                // Check if JSON is valid:
                if(!j.contains("query"))
                    continue;

                j = j.at("query");
                if(!j.contains("pages"))
                    continue;

                j = j.at("pages");
                if(j.empty())
                    continue;

                j = *j.begin();
                if(!j.contains("title"))
                    continue;

                // Finally, get the ACTUAL page returned from Wiki API
                page = j.at("title");

            } while(page.empty());

            std::cout << page << '\n';
            return page;
        }
        catch (curlpp::LogicError& e)
        {
            std::cerr << "LoginError: '" << e.what() << "' when running '" << __FUNCTION__ << "'\n";
        }
        catch (curlpp::RuntimeError& e)
        {
            std::cerr << "RuntimeError: '" << e.what() << "' when running '" << __FUNCTION__ << "'\n";
        }
        catch(...)
        {
            std::cerr << "Unknown error occurred in function: '" << __FUNCTION__ << "'\n";
        }
    }
    throw exception_utils::NetworkError("Failed to retrieve random page from WikiPedia");
}
