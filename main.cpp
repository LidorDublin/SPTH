#include <iostream>
#include <iomanip>

#include <csignal>

#include "web_utils/web_utils.h"
#include "wikiPage/wikiPage.h"

#define DEFAULT_TEST_PAGE "google"

#define ANSII "\033["
#define ANSII_RESET ANSII"0m"
#define ANSII_BOLD ANSII"1m"
#define ANSII_UNDERLINE ANSII"4m"

using std::cout;

void handler(sig_atomic_t s);
void printSummary();

int main(int argc, char** argv)
{
    signal(SIGINT, handler);
    std::cout << std::boolalpha;

    wikiPage links(argc > 1 ?  argv[1] : DEFAULT_TEST_PAGE);

    std::vector<std::deque<std::string>> paths;
    links.getWikiPageLinksRecursively(paths);

    std::cout << "\n--------------------------------------------\n";
    std::cout << ANSII_UNDERLINE "Paths found" ANSII_RESET ": " << '\n';
    for (const auto& path : paths)
    {
        for (const auto &page : path)
            std::cout << page << " -> ";
        std::cout << "Adolf Hitler\n";
    }

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
    std::cout << "\n\n--------------------------------------------\n";
    std::cout << "Summary:\n\n";
    std::cout << "Total number of links " ANSII_UNDERLINE "collected" ANSII_RESET ": " << wikiPage::totalNumOfLinks() << '\n';
    std::cout << "Total number of links " ANSII_UNDERLINE "processed" ANSII_RESET ": " << wikiPage::totalNumOfProcessedLinks() << '\n';
    std::cout << "Number of uniquely visited links: " << cache_utils::_visitedPages.size() << '\n';
    std::cout << "Number of revisited links: " << cache_utils::timesRevisited << '\n';
}
