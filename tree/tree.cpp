//
// Created by lidor on 11/4/19.
//

#include <iostream>
#include "tree.h"

unsigned long tree::_totalNumOfLinks = 0;

tree::tree(int depth, std::string page, std::vector<tree*> value)
{
    tree::_totalNumOfLinks += value.size();
    tree(depth, std::move(page));
    this->_links = std::move(value);
}

tree::tree(int depth, std::string page) : _depth(depth), _page(std::move(page)), _links(std::vector<tree*>())
{
}

tree::~tree()
{
    for(auto&& link : this->_links)
        delete link;
}

void tree::addLink(tree* link)
{
    this->_links.push_back(link);
    tree::_totalNumOfLinks++;
}

void tree::addLink(std::string link)
{
    this->_links.push_back(new tree(this->_depth + 1, std::move(link)));
    tree::_totalNumOfLinks++;
}

const std::string &tree::getPage() const
{
    return _page;
}

const std::vector<tree *> &tree::getLinks() const
{
    return _links;
}

int tree::numOfLinks() const
{
    return this->_links.size();
}

unsigned long tree::totalNumOfLinks()
{
    return tree::_totalNumOfLinks;
}

