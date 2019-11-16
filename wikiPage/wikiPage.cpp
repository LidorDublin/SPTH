//
// Created by lidor on 11/4/19.
//

#include "wikiPage.h"

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

/*
 * Add link to vector, increments the counter of links
 */
void wikiPage::addLink(wikiPage* link)
{
    this->_links.push_back(std::make_shared<wikiPage*>(link));
    wikiPage::_totalNumOfLinks++;
}

/*
 * Create a new link object and add to vector
 */
void wikiPage::addLink(std::string link)
{
    this->addLink(new wikiPage(this->_depth + 1, std::move(link), this));
}

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

void wikiPage::getWikiPageLinks()
{
    web_utils::getPageLinks(this);
}

/*
 * Operate on all the links, recursively, to find the paths to Hitler
 */
void wikiPage::getWikiPageLinksRecursively(pathsQueue& paths)
{
    // Limit the number of paths since otherwise it will never end (or will it?)
    if(paths.size() == wikiPage::MAX_NUM_OF_PATHS)
        return;

    // Limit the depth
    if (this->_depth == wikiPage::MAX_DEPTH)
        return;

    // Check if there is a need to operate on this page (if the page was not visited in a higher depth)
    if(cache_utils::isPageVisited(this->_page, this->_depth))
        return;

    wikiPage::_totalNumOfProcessedLinks++;

    // Populate vector with links
    this->getWikiPageLinks();

    for (auto& link : this->_links)
    {
        if(wikiPage::bingo((*link)->_page))
        {
            std::cout << "Found " << paths.size() + 1 << " paths already!\t|  ";

            std::deque<std::string> deq;
            deq.push_front(this->_page);
            this->getAllParentsPages(deq);
            std::for_each(deq.begin(), deq.end(), [](const std::string& page){
                std::cout << page << " -> ";
            });
            std::cout << wikiPage::HITLER << '\n';
            paths.push(deq);
        }
        else
            (*link)->getWikiPageLinksRecursively(paths);
    }
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
    if(!this->_parent)
        return;

    parents.push_front(this->_parent->_page);
    this->_parent->getAllParentsPages(parents);
}

