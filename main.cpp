#include <iostream>
#include <iomanip>

#include "web_utils/web_utils.h"
#include "wikiPage/wikiPage.h"

#define DEFAULT_TEST_PAGE "google"

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
    wikiPage links(argc > 1 ?  argv[1] : DEFAULT_TEST_PAGE);
//    cout << links.getPage() << endl;

    links.getWikiPageLinks();

    for(auto& link : links.getLinks())
        std::cout << link->getPage() << std::endl;

//    cout << wikiPage::totalNumOfLinks() << endl;
//    cout << links.numOfLinks() << endl;

    return 0;
}
