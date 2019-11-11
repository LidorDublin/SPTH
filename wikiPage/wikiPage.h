//
// Created by lidor on 11/4/19.
//

#pragma once

#include <vector>
#include <string>

#include <utility>
#include <iostream>

#include <deque>
#include <vector>

class wikiPage;

#include "../web_utils/web_utils.h"

class wikiPage
{
public:
    explicit wikiPage(int depth, std::string  page, std::vector<wikiPage*>  links, wikiPage* parent);
    explicit wikiPage(int depth, std::string page, wikiPage* parent);
    explicit wikiPage(std::string page);

    virtual ~wikiPage();

    void addLink(wikiPage* link);
    void addLink(std::string link);

    const std::string& getPage() const;

    const std::vector<wikiPage*>& getLinks() const;

    void getWikiPageLinks();

    wikiPage* getWikiPageLinksRecursively(std::vector<std::deque<std::string>>& paths);

    int numOfLinks() const;

    static bool bingo(const std::string& str);

    inline const static std::string HITLER = "Adolf Hitler";
    static unsigned long totalNumOfLinks();
    const static unsigned short MAX_DEPTH = 5;

protected:
    static unsigned long _totalNumOfLinks;

    int _depth;

    wikiPage* _parent;

    std::string _page;
    std::vector<wikiPage*> _links{};
};

