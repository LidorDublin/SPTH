#include <iostream>
#include <iomanip>

#include "web_utils/web_utils.h"

#define DEFAULT_TEST_PAGE "google"

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
    std::vector<std::string> links = web_utils::getPageLinks(argc > 1 ?  argv[1] : DEFAULT_TEST_PAGE);

    for(auto& link : links)
        std::cout << link << std::endl;

//    cout << std::setw(4) << web_utils::getPageLinks("google");
    return 0;
}
