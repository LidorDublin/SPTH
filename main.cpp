#include <iostream>
#include <iomanip>

#include "web_utils/web_utils.h"

using std::cout;
using std::endl;

int main()
{
    cout << std::setw(4) << web_utils::getPageLinks("google");
    return 0;
}
