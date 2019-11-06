//
// Created by lidor on 11/4/19.
//

#pragma once

#include <vector>
#include <string>

#include <utility>

class tree
{
public:
    explicit tree(int depth, std::string page, std::vector<tree*>  value);
    explicit tree(int depth, std::string page);

    virtual ~tree();

    void addLink(tree* link);
    void addLink(std::string link);

    const std::string& getPage() const;

    const std::vector<tree*>& getLinks() const;

    int numOfLinks() const;

    static unsigned long totalNumOfLinks();

    const static unsigned short MAX_DEPTH = 3;

protected:
    static unsigned long _totalNumOfLinks;

    int _depth;

    std::string _page;
    std::vector<tree*> _links{};
};

