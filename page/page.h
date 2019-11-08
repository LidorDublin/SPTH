//
// Created by lidor on 11/4/19.
//

#pragma once

#include <vector>
#include <string>

#include <utility>

class page
{
public:
    explicit page(int depth, std::string page, std::vector<page*>  value);
    explicit page(int depth, std::string page);

    virtual ~page();

    void addLink(page* link);
    void addLink(std::string link);

    const std::string& getPage() const;

    const std::vector<page*>& getLinks() const;

    int numOfLinks() const;

    static unsigned long totalNumOfLinks();

    const static unsigned short MAX_DEPTH = 3;

protected:
    static unsigned long _totalNumOfLinks;

    int _depth;

    std::string _page;
    std::vector<page*> _links{};
};

