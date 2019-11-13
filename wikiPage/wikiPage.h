//
// Created by lidor on 11/4/19.
//

#pragma once

#include <string>

#include <utility>
#include <iostream>

#include <deque>
#include <vector>

class wikiPage;

#include "../web_utils/web_utils.h"

struct ComparatorForPathsLengths
{
    bool operator()(const std::deque<std::string>& a, const std::deque<std::string>& b)
    {
        return a.size() < b.size();
    }
};

using pathsQueue = std::priority_queue<std::deque<std::string>, std::deque<std::deque<std::string>>, ComparatorForPathsLengths>;

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

    void getWikiPageLinksRecursively(pathsQueue& paths);

    int numOfLinks() const;

    static bool bingo(const std::string& str);

    inline const static std::string HITLER = "Adolf Hitler";
    const static unsigned short MAX_DEPTH = 5;
    static unsigned long totalNumOfLinks();
    static unsigned long totalNumOfProcessedLinks();

protected:
    static unsigned long _totalNumOfLinks;
    static unsigned long _totalNumOfProcessedLinks;

    int _depth;

    wikiPage* _parent;

    std::string _page;
    std::vector<wikiPage*> _links{};

    wikiPage* getParent() const;
    void getAllParents(std::deque<wikiPage*>& parents) const;
    void getAllParentsPages(std::deque<std::string>& parents) const;
};

