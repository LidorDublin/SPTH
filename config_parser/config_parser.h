//
// Created by lidor on 12/2/19.
//

#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>

#include "../thread_utils/thread_utils.h"
#include "../wikiPage/wikiPage.h"

class config_parser
{
public:
    static void readConfig(const std::string& fileName);
    static void readConfig(std::ifstream& f);

private:
    inline static std::map<std::string, void*> settings {
            {"num_of_threads", (void*)&thread_utils::NUM_OF_THREADS},
            {"max_num_of_paths", (void*)&wikiPage::MAX_NUM_OF_PATHS},
            {"max_depth", (void*)&wikiPage::MAX_DEPTH},
    };
};
