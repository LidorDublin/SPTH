//
// Created by lidor on 11/4/19.
//

#pragma once

#include <vector>
#include <string>

#include <utility>

class wikiPage;

#include "../web_utils/web_utils.h"

class wikiPage
{
public:
    explicit wikiPage(int depth, std::string page, std::vector<wikiPage*>  value);
    explicit wikiPage(int depth, std::string page);
    explicit wikiPage(std::string page);

    virtual ~wikiPage();

    void addLink(wikiPage* link);
    void addLink(std::string link);

    const std::string& getPage() const;

    const std::vector<wikiPage*>& getLinks() const;

    void getWikiPageLinks();

    int numOfLinks() const;

    static unsigned long totalNumOfLinks();

    const static unsigned short MAX_DEPTH = 3;

protected:
    static unsigned long _totalNumOfLinks;

    int _depth;

    std::string _page;
    std::vector<wikiPage*> _links{};
};
