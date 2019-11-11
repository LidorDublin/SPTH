#include <iostream>
#include <iomanip>

#include <csignal>

#include "web_utils/web_utils.h"
#include "wikiPage/wikiPage.h"

#define DEFAULT_TEST_PAGE "google"

using std::cout;

void handler(sig_atomic_t s);
void printSummary();

int main(int argc, char** argv)
{
    signal(SIGINT, handler);
    std::cout << std::boolalpha;

    wikiPage links(argc > 1 ?  argv[1] : DEFAULT_TEST_PAGE);
//    cout << links.getPage() << '\n';

//    links.getWikiPageLinks();
    std::vector<std::deque<std::string>> paths;
    links.getWikiPageLinksRecursively(paths);

    for (const auto& path : paths)
    {
        for (const auto &page : path)
            std::cout << page << " -> ";
        std::cout << "Adolf Hitler\n";
    }

//    for(auto& link : links.getLinks())
//        std::cout << link->getPage() << '\n';

//    cout << wikiPage::totalNumOfLinks() << '\n';
//    cout << links.numOfLinks() << '\n';

    printSummary();
    return 0;
}

void handler(sig_atomic_t)
{
    printSummary();
    exit(1);
}

void printSummary()
{
    std::cout << "\n--------------------------------------------\n";
    std::cout << "Summary:\n\n";
    std::cout << "Total number of links collected: " << wikiPage::totalNumOfLinks() << '\n';
    std::cout << "Number of visited links: " << cache_utils::_visitedPages.size() << '\n';
    std::cout << "Number of revisited links: " << cache_utils::timesRevisited << '\n';
}
