#include "wikiPage/wikiPage.h"
#include "thread_utils/thread_utils.h"

#include <iostream>
#include <csignal>

#define DEFAULT_TEST_PAGE "google"

#define ANSII "\033["
#define ANSII_RESET ANSII"0m"
#define ANSII_BOLD ANSII"1m"
#define ANSII_UNDERLINE ANSII"4m"

using std::cout;

void cleanup();
void handler(sig_atomic_t s);
void printSummary();
void printPaths(pathsQueue& paths);

int main(int argc, char** argv)
{
    signal(SIGINT, handler); // Catch the ^C and call handler function

//    std::ios_base::sync_with_stdio(false); // Reference: https://stackoverflow.com/a/31165481/7924484
    std::cout << std::boolalpha; // Allows printing boolean values rather than 0 or 1

    wikiPage links(argc > 1 ?  argv[1] : DEFAULT_TEST_PAGE);

    pathsQueue paths;
    links.getWikiPageLinksRecursively(paths);

    printPaths(paths);

    cleanup();
    printSummary();
    return 0;
}

/*
 * Clean up, release mutexes, wake blocking CVs...
 */
void cleanup()
{
    thread_utils::finished = true;
    thread_utils::waitTillFinished.notify_all();
    thread_utils::m_outputReady.notify_all();
}

/*
 * A function to catch ^C (SIGINT) instead of crash immediately
 */
void handler(sig_atomic_t)
{
    cleanup();
    printSummary();
    exit(1);
}

/*
 * Prints the summary of the run, including some statistics and such
 */
void printSummary()
{
    std::cout << "\n\n--------------------------------------------\n";
    std::cout << "Summary:\n\n";
    std::cout << "Total number of links " ANSII_UNDERLINE "collected" ANSII_RESET ": " << wikiPage::totalNumOfLinks() << '\n';
    std::cout << "Total number of links " ANSII_UNDERLINE "processed" ANSII_RESET ": " << wikiPage::totalNumOfProcessedLinks() << '\n';
    std::cout << "Number of uniquely visited links: " << cache_utils::m_visitedPages.size() << '\n';
    std::cout << "Number of revisited links: " << cache_utils::timesRevisited << '\n';
}

void printPaths(pathsQueue& paths)
{
    std::cout << "\n--------------------------------------------\n";
    std::cout << ANSII_UNDERLINE "Paths found" ANSII_RESET ": " << '\n';

    while(!paths.empty())
    {
        for (const auto &page : paths.top())
            std::cout << page << " -> ";
        std::cout << "Adolf Hitler\n";

        paths.pop();
    }
}
