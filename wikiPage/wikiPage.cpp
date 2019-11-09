//
// Created by lidor on 11/4/19.
//

#include <iostream>
#include "wikiPage.h"

unsigned long wikiPage::_totalNumOfLinks = 0;

wikiPage::wikiPage(int depth, std::string page, std::vector<wikiPage*> value)
{
    wikiPage::_totalNumOfLinks += value.size();
    wikiPage(depth, std::move(page));
    this->_links = std::move(value);
}

wikiPage::wikiPage(int depth, std::string page) : _depth(depth), _page(std::move(page)), _links(std::vector<wikiPage*>())
{
}

wikiPage::wikiPage(std::string page) : wikiPage(1, page)
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
    this->_links.push_back(new wikiPage(this->_depth + 1, std::move(link)));
    wikiPage::_totalNumOfLinks++;
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

void wikiPage::getWikiPageLinks()
{
    web_utils::getPageLinks(this);
}

wikiPage* wikiPage::getWikiPageLinksRecursively()
{
//    std::cout << this->_page << ' ' << (this->_page == "Adolf Hitler") << ' ' << this->_depth << '\n';

    if (this->_depth == wikiPage::MAX_DEPTH)
        return nullptr;

    this->getWikiPageLinks();

    auto begin = this->_links.begin();
    auto end = this->_links.end();

    auto linkIter = std::find_if(begin, end, [](wikiPage *link) {
        return wikiPage::bingo(link->_page);
    });
    if (linkIter != end)
        return *linkIter;

    for(auto& link : this->_links)
        if(auto page = link->getWikiPageLinksRecursively())
            return page;

    return nullptr;
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
