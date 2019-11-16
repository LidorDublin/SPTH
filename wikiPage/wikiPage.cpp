//
// Created by lidor on 11/4/19.
//

#include "wikiPage.h"

std::atomic<uint32_t> wikiPage::_totalNumOfLinks = 0;
std::atomic<uint32_t> wikiPage::_totalNumOfProcessedLinks = 0;
std::atomic<uint8_t> wikiPage::_numOfThreads = 0;
const std::thread::id wikiPage::MAIN_THREAD_ID = std::this_thread::get_id();
std::mutex wikiPage::_mtx;
std::atomic<bool> wikiPage::KILL = false;

wikiPage::wikiPage(int depth, std::string  page, std::vector<wikiPage*> links, wikiPage* parent) :
_depth(depth), _parent(parent), _page(std::move(page)), _links(std::move(links))
{
    wikiPage::_totalNumOfLinks += links.size();
}

wikiPage::wikiPage(int depth, std::string page, wikiPage* parent) :
wikiPage(depth, std::move(page), std::vector<wikiPage*>(), parent)
{
}

wikiPage::wikiPage(std::string&& page) : wikiPage(1, std::move(page), nullptr)
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

unsigned int wikiPage::numOfLinks() const
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

void wikiPage::getWikiPageLinksRecursively(pathsQueue& paths)
{
//    std::cout << std::this_thread::get_id() << ' ' << wikiPage::MAIN_THREAD_ID << ' ' << this->_page << '\n';

//    std::cout << this->_page << ' ' << wikiPage::bingo(this->_page) << ' ' << this->_depth << '\n';

    if(wikiPage::KILL)
        return;

    wikiPage::_mtx.lock();
    if(paths.size() == 50)
    {
        wikiPage::_mtx.unlock();
        return;
    }
    wikiPage::_mtx.unlock();

    if (this->_depth == wikiPage::MAX_DEPTH)
        return;

    if(cache_utils::isPageVisited(this->_page, this->_depth))
        return;

    wikiPage::_mtx.lock();
    wikiPage::_totalNumOfProcessedLinks++;
    wikiPage::_mtx.unlock();

    this->getWikiPageLinks();
    for (auto& link : this->_links)
    {
        if(wikiPage::KILL)
            break;

        if(wikiPage::bingo(link->_page))
        {
            std::deque<std::string> deq;
            deq.push_front(this->_page);
            this->getAllParentsPages(deq);

            wikiPage::_mtx.lock();
            std::cout << "Found " << paths.size() + 1 << " paths already!\t|  ";
            std::for_each(deq.begin(), deq.end(), [](const std::string& page){
                std::cout << page << " -> ";
            });
            std::cout << wikiPage::HITLER << '\n';
            paths.push(deq);

            if(paths.size() == 500)
                wikiPage::KILL = true;

            wikiPage::_mtx.unlock();
        }
        else
        {
            if(wikiPage::_numOfThreads > 5)
                link->getWikiPageLinksRecursively(paths);
            else
            {
//                wikiPage::_mtx.lock();
                wikiPage::_numOfThreads++;
                std::thread(&wikiPage::getWikiPageLinksRecursively, link, std::ref(paths)).detach();
//                wikiPage::_mtx.unlock();
            }
        }
    }

    if(wikiPage::MAIN_THREAD_ID != std::this_thread::get_id())
        wikiPage::_numOfThreads--;
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
    if(!this || !this->_parent)
        return;

    parents.push_front(this->_parent->_page);
    this->_parent->getAllParentsPages(parents);
}

uint8_t wikiPage::numOfThreads()
{
    return wikiPage::_numOfThreads;
}


