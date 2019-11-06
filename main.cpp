#include <iostream>
#include <iomanip>

#include "web_utils/web_utils.h"
#include "tree/tree.h"

#define DEFAULT_TEST_PAGE "google"

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
    tree links(0, argc > 1 ?  argv[1] : DEFAULT_TEST_PAGE);
    cout << links.getPage() << endl;

    web_utils::getPageLinks(&links);

    for(auto& link : links.getLinks())
        std::cout << link->getPage() << std::endl;

    cout << tree::totalNumOfLinks() << endl;
    cout << links.numOfLinks() << endl;

    return 0;
}
