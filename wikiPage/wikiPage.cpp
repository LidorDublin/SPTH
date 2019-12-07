//
// Created by lidor on 11/4/19.
//

#include "wikiPage.h"
#include "../thread_utils/thread_utils.h"  // Has to be included in the source file rather than in the header file due to circular dependency

uint16_t wikiPage::MAX_DEPTH = 0;
uint16_t wikiPage::MAX_NUM_OF_PATHS = 0;

uint32_t wikiPage::_totalNumOfLinks = 0;
uint32_t wikiPage::_totalNumOfProcessedLinks = 0;

wikiPage::wikiPage(int depth, std::string  page, std::vector<sharedWikiPage> links, wikiPage* parent) :
_depth(depth), _parent(parent), _page(std::move(page)), _links(std::move(links))
{
    wikiPage::_totalNumOfLinks += links.size();
}

wikiPage::wikiPage(int depth, std::string page, wikiPage* parent) :
wikiPage(depth, std::move(page), std::vector<sharedWikiPage>(), parent)
{
}

wikiPage::wikiPage(std::string&& page) : wikiPage(1, std::move(page), nullptr)
{
}

wikiPage::wikiPage() : _depth(1), _parent(nullptr), _page("")
{
    *this = wikiPage(web_utils::getRandomPage());
}

/*
 * Create a new link object and add to vector
 */
void wikiPage::addLink(const std::string& link)
{
    this->_links.push_back(std::make_shared<wikiPage>(wikiPage(this->_depth + 1, link, this)));
}

/*
 * Mostly used to split the work between all the threads.
 */
void wikiPage::getWikiPageLinksRecursively(pathsQueue& paths)
{
    if(!this->getWikiPageLinks())
        return;

    std::thread(&thread_utils::print).detach();

    // If there is only 1 thread:
    if (thread_utils::NUM_OF_THREADS <= 1)
        this->operateOnRange(this->begin(), this->end(), paths);

    else
    {
        const auto& ranges = this->splitLinks();
        for (const auto& [first, second] : ranges)
        {
            if(second - first)
            {
                thread_utils::numOfThreads++;
                std::thread(&wikiPage::operateOnRange, this, first, second, std::ref(paths)).detach();
            }
        }
        thread_utils::waitTillFinished.wait(thread_utils::m_waitTillFinished_unqLk);
    }
}

/*
 * The actual function that finds Hitler. Iterates over all links recursively from begin to end.
 * Args:
 *      const std::vector<sharedWikiPage>::const_iterator&& begin: The iterator to the beginning of the vector of links.
 *      const std::vector<sharedWikiPage>::const_iterator&& end: The iterator to the end of the vector of links.
 *      pathsQueue& paths: The priority_queue used to store the paths that were found
 */
void wikiPage::operateOnRange(const sharedWikiPageConstIterator&& begin, const sharedWikiPageConstIterator&& end, pathsQueue& paths)
{
    // Limit the depth
    if (this->_depth == wikiPage::MAX_DEPTH)
        goto exit_function;

    for (auto it = begin; it != end && !(wikiPage::checkFinish(paths)); ++it)
    {
        // Dereference check
        if (!*it || !(*it).get())
            goto exit_function;

        wikiPage::_totalNumOfProcessedLinks++;

        auto& link = **it;
        if(wikiPage::bingo(link._page))
        {
            // Store the current path in a dequeue of strings
            std::deque<std::string> path;

            // Insert Hitler to the end of the path and populate the deque with all the parents pages
            // (Hitler will be displayed in RED)
            std::stringstream hitler;
            hitler << ANSII_RED << wikiPage::HITLER.data() << ANSII_RESET;
            path.emplace_back(hitler.str());
            this->getAllParentsPages(path);

            // Push the deque to the output queue so the printing thread will process it
            thread_utils::output << path;

            thread_utils::mtx_pathsVector.lock();
            paths.push(path);
            thread_utils::mtx_pathsVector.unlock();
        }
        else if (link._depth < wikiPage::MAX_DEPTH)
        {
            // Check if there is a need to operate on this page (if the page was not visited in a higher depth)
            if(cache_utils::isPageVisited(link._page, link._depth) || wikiPage::checkFinish(paths))
                goto exit_function;

            if (link._links.empty() && link.getWikiPageLinks())
                    link.operateOnRange(link.begin(), link.end(), paths);
        }
    }

    exit_function:
    if(this->_depth == 1)
        thread_utils::numOfThreads--;

    if(!thread_utils::numOfThreads)
        thread_utils::waitTillFinished.notify_all();
}

bool wikiPage::bingo(const std::string& str)
{
    // Check if the lengths are equal
    if(str.length() != wikiPage::HITLER.length())
        return false;

    // Iterate over both strings at once, comparing each character to the matching one on the other string
    // (Case-insensitive match)
    return std::equal(str.begin(), str.end(), wikiPage::HITLER.begin(), wikiPage::HITLER.end(), [](char chr1, char chr2){
        return std::tolower(chr1) == std::tolower(chr2);
    });
}

/*
 * Populate a deque with all the parents of the current link
 */
void wikiPage::getAllParents(std::deque<wikiPage*>& parents) const
{
    if(!this->_parent)
        return;

    parents.push_back(this->_parent);
    this->getAllParents(parents);
}

/*
 * Populate a deque with all the pages of the parents of the current link
 */
void wikiPage::getAllParentsPages(std::deque<std::string>& parents) const
{
    parents.push_front(this->_page);

    if(!this->_parent)
        return;

    this->_parent->getAllParentsPages(parents);
}

std::vector <std::pair<sharedWikiPageConstIterator, sharedWikiPageConstIterator>> wikiPage::splitLinks() const
{
    // Credit: https://codereview.stackexchange.com/a/106837

    std::vector<std::pair<sharedWikiPageConstIterator, sharedWikiPageConstIterator>> ranges{};
    ranges.reserve(thread_utils::NUM_OF_THREADS);

    auto begin = this->_links.begin();
    auto end = this->_links.end();

    auto dist = std::distance(begin, end);
    auto chunk = dist / thread_utils::NUM_OF_THREADS;
    auto remainder = dist % thread_utils::NUM_OF_THREADS;

    for (uint16_t i = 0; i < thread_utils::NUM_OF_THREADS - 1; ++i)
    {
        auto next_end = std::next(begin, chunk + (remainder ? 1 : 0));
        ranges.emplace_back(begin, next_end);

        begin = next_end;
        if (remainder) remainder--;
    }

    // last chunk
    ranges.emplace_back(begin, end);

    return ranges;
}

sharedWikiPageConstIterator wikiPage::begin() const
{
    return this->_links.begin();
}

sharedWikiPageConstIterator wikiPage::end() const
{
    return this->_links.end();
}

bool wikiPage::checkFinish(const pathsQueue& paths) const
{
    if(thread_utils::finished)
        return true;

    // Limit the number of paths to find
    std::lock_guard<std::mutex> lkGrd(thread_utils::mtx_pathsVector);
    return paths.size() >= wikiPage::MAX_NUM_OF_PATHS;
}

// Getters and Setters
const std::string &wikiPage::getPage() const
{
    return _page;
}

const std::vector<sharedWikiPage> &wikiPage::getLinks() const
{
    return _links;
}

unsigned long wikiPage::numOfLinks() const
{
    return this->_links.size();
}

uint32_t wikiPage::totalNumOfLinks()
{
    return wikiPage::_totalNumOfLinks;
}

uint32_t wikiPage::totalNumOfProcessedLinks()
{
    return wikiPage::_totalNumOfProcessedLinks;
}

/*
 * Populate this->_links vector with links in the wiki page.
 * Returns true if the vector contains any links after the request to WikiPedia, False otherwise
 */
bool wikiPage::getWikiPageLinks()
{
    try
    {
        web_utils::getPageLinks(this);
        return !this->_links.empty();
    }
    catch(exception_utils::NetworkError& error)
    {
        std::cerr << "Failed retrieving page '" << this->_page << "'. Failed with error:\n";
        std::cerr << error.what();
    }
    catch (...)
    {

    }

    return false;
}

