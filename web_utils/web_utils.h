//
// Created by lidor on 11/3/19.
//

#pragma once

#include <string>
#include <sstream>
#include <algorithm>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

#include "nlohmann/json.hpp"

using nlohmann::json;

namespace web_utils
{
//public:
    json getPageLinks(const std::string& page);

//private:
    const std::string _WIKI_URL = "https://en.wikipedia.org/w/api.php?action=parse&prop=links&format=json&page=";
    std::string _requestPage(const std::string& page);
}
