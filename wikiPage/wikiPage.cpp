//
// Created by lidor on 11/4/19.
//

#include "wikiPage.h"

#include <utility>

unsigned long wikiPage::_totalNumOfLinks = 0;
unsigned long wikiPage::_totalNumOfProcessedLinks = 0;

wikiPage::wikiPage(int depth, std::string  page, std::vector<wikiPage*> links, wikiPage* parent) :
_depth(depth), _parent(parent), _page(std::move(page)), _links(std::move(links))
{
    wikiPage::_totalNumOfLinks += links.size();
}

wikiPage::wikiPage(int depth, std::string page, wikiPage* parent) :
wikiPage(depth, std::move(page), std::vector<wikiPage*>(), parent)
{
}

wikiPage::wikiPage(std::string page) : wikiPage(1, std::move(page), nullptr)
{
}

wikiPage::~wikiPage()
{
    for(auto&& link : this->_links)
        delete link;
}

void wikiPage::addLink(wikiPage* link)
{
    this->_links.push_back(link);
    wikiPage::_totalNumOfLinks++;
}

void wikiPage::addLink(std::string link)
{
    this->addLink(new wikiPage(this->_depth + 1, std::move(link), this));
}

const std::string &wikiPage::getPage() const
{
    return _page;
}

const std::vector<wikiPage *> &wikiPage::getLinks() const
{
    return _links;
}

int wikiPage::numOfLinks() const
{
    return this->_links.size();
}

unsigned long wikiPage::totalNumOfLinks()
{
    return wikiPage::_totalNumOfLinks;
}

unsigned long wikiPage::totalNumOfProcessedLinks()
{
    return wikiPage::_totalNumOfProcessedLinks;
}

void wikiPage::getWikiPageLinks()
{
    web_utils::getPageLinks(this);
}

void wikiPage::getWikiPageLinksRecursively(pathsQueue& paths)
{
    wikiPage::_totalNumOfProcessedLinks++;

//    std::cout << this->_page << ' ' << wikiPage::bingo(this->_page) << ' ' << this->_depth << '\n';

    if(paths.size() == 25)
        return;

    if (this->_depth == wikiPage::MAX_DEPTH)
        return;

    if(cache_utils::isPageVisited(this->_page))
        return;
//    Insert page to visitedPages caching set
    cache_utils::visitPage(this->_page);

    this->getWikiPageLinks();
    for (auto& link : this->_links)
    {
        if(wikiPage::bingo(link->_page))
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
            link->getWikiPageLinksRecursively(paths);
    }
}

bool wikiPage::bingo(const std::string& str)
{
    if(str.length() != wikiPage::HITLER.length())
        return false;

    auto it = std::search(str.begin(), str.end(), wikiPage::HITLER.begin(), wikiPage::HITLER.end(), [](char chr1, char chr2){
        return std::tolower(chr1) == std::tolower(chr2);
    });

    return it != str.end();
}

wikiPage *wikiPage::getParent() const
{
    return this->_parent;
}

void wikiPage::getAllParents(std::deque<wikiPage *>& parents) const
{
    if(!this->_parent)
        return;

    parents.push_back(this->_parent);
    this->getAllParents(parents);
}

void wikiPage::getAllParentsPages(std::deque<std::string>& parents) const
{
//    std::cout << "In recursion\n";
//    return;

    if(!this->_parent)
        return;

    parents.push_front(this->_parent->_page);
    this->_parent->getAllParentsPages(parents);
}


