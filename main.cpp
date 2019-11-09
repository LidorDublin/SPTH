#include <iostream>
#include <iomanip>

#include <csignal>

#include "web_utils/web_utils.h"
#include "wikiPage/wikiPage.h"

#define DEFAULT_TEST_PAGE "google"

using std::cout;

void handler(sig_atomic_t s);

int main(int argc, char** argv)
{
    signal(SIGINT, handler);
    std::cout << std::boolalpha;

    wikiPage links(argc > 1 ?  argv[1] : DEFAULT_TEST_PAGE);
//    cout << links.getPage() << '\n';

//    links.getWikiPageLinks();
    auto _ = links.getWikiPageLinksRecursively();
    std::cout << (!_ ? "Nullptr" : _->getPage()) << '\n';

//    for(auto& link : links.getLinks())
//        std::cout << link->getPage() << '\n';

//    cout << wikiPage::totalNumOfLinks() << '\n';
//    cout << links.numOfLinks() << '\n';

    std::cout << "\nTotal number of links fetched: " << wikiPage::totalNumOfLinks() << '\n';
    return 0;
}

void handler(sig_atomic_t s)
{
    std::cout << "\nTotal number of links fetched: " << wikiPage::totalNumOfLinks() << '\n';
    exit(1);
}
