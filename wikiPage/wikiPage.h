//
// Created by lidor on 11/4/19.
//

#pragma once

#include <string>
#include <deque>
#include <vector>
#include <iterator>

#include <iostream>

class wikiPage; // Forward declaration, fixes circular dependencies

#include "../web_utils/web_utils.h"

struct ComparatorForPathsLengths
{
    /*
     * Function predicate used for the ordering in the priority_queue
     * Args:
     *      const std::deque<std::string>& left: The first dequeue to compare
     *      const std::deque<std::string>& right: The second dequeue to compare
     *
     * Returns:
     *      a boolean value. True if the left deque is smaller, else otherwise
     */
    bool operator()(const std::deque<std::string>& left, const std::deque<std::string>& right)
    {
        return left.size() < right.size();
    }
};

// Used in order to not write the long line (the part after the '=' sign)
using pathsQueue = std::priority_queue<std::deque<std::string>, std::deque<std::deque<std::string>>, ComparatorForPathsLengths>;
using sharedWikiPage = std::shared_ptr<wikiPage*>;
using sharedWikiPageConstIterator = std::vector<sharedWikiPage>::const_iterator;

class wikiPage
{
public:
    // Constructors
    explicit wikiPage(int depth, std::string  page, std::vector<sharedWikiPage>  links, wikiPage* parent);
    explicit wikiPage(int depth, std::string page, wikiPage* parent);
    explicit wikiPage(std::string&& page);

    // Destructor: marked as default
    virtual ~wikiPage() = default;

    // Add link to vector
    void addLink(wikiPage* link);
    void addLink(const std::string& link);

    // Returns the page as string
    const std::string& getPage() const;

    // Returns a vector containing all the links in the current page
    const std::vector<sharedWikiPage>& getLinks() const;
    sharedWikiPageConstIterator begin() const;
    sharedWikiPageConstIterator end() const;

    // Used to populate the vector of links with the actual links in the current page
    void getWikiPageLinks();
    void getWikiPageLinksRecursively(pathsQueue& paths);

    // Returns the number of links aka the size of the links vector
    // Deprecated? unused...
    unsigned long numOfLinks() const;

    static bool bingo(const std::string& str);

    static uint32_t totalNumOfLinks();
    static uint32_t totalNumOfProcessedLinks();

    // Constants
    constexpr static std::string_view HITLER{"Adolf Hitler"};
    constexpr static uint8_t MAX_DEPTH = 5;
    constexpr static uint16_t MAX_NUM_OF_PATHS = 250;

protected:
    static uint32_t _totalNumOfLinks;
    static uint32_t _totalNumOfProcessedLinks;

    uint8_t _depth;

    wikiPage* _parent;

    std::string _page;
    std::vector<sharedWikiPage> _links{};

    void getAllParents(std::deque<wikiPage*>& parents) const;
    void getAllParentsPages(std::deque<std::string>& parents) const;

    void operateOnRange(const sharedWikiPageConstIterator&& begin, const sharedWikiPageConstIterator&& end, pathsQueue& paths);

    std::vector<std::pair<sharedWikiPageConstIterator, sharedWikiPageConstIterator>> splitLinks() const;

    bool checkFinish(const pathsQueue& paths) const;
};
