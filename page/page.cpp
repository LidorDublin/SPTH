//
// Created by lidor on 11/4/19.
//

#include <iostream>
#include "page.h"

unsigned long page::_totalNumOfLinks = 0;

page::page(int depth, std::string page, std::vector<page*> value)
{
    page::_totalNumOfLinks += value.size();
    page(depth, std::move(page));
    this->_links = std::move(value);
}

page::page(int depth, std::string page) : _depth(depth), _page(std::move(page)), _links(std::vector<page*>())
{
}

page::~page()
{
    for(auto&& link : this->_links)
        delete link;
}

void page::addLink(page* link)
{
    this->_links.push_back(link);
    page::_totalNumOfLinks++;
}

void page::addLink(std::string link)
{
    this->_links.push_back(new page(this->_depth + 1, std::move(link)));
    page::_totalNumOfLinks++;
}

const std::string &page::getPage() const
{
    return _page;
}

const std::vector<page *> &page::getLinks() const
{
    return _links;
}

int page::numOfLinks() const
{
    return this->_links.size();
}

unsigned long page::totalNumOfLinks()
{
    return page::_totalNumOfLinks;
}

