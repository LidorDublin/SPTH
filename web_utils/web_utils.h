//
// Created by lidor on 11/3/19.
//

#pragma once

#include <string>
#include <sstream>
#include <algorithm>
#include <queue>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

#include "../wikiPage/wikiPage.h"
#include "../cache_utils/cache_utils.h"
#include "../exceptions/exceptions.h"

#include "nlohmann/json.hpp"

using nlohmann::json;

namespace web_utils
{
//public:
    void getPageLinks(wikiPage* links);
    std::string getRandomPage();

//private:
    const std::string m_WIKI_GET_PAGE = "https://en.wikipedia.org/w/api.php?action=parse&prop=links&format=json&page=";
    constexpr std::string_view m_WIKI_RANDOM_PAGE = "https://en.wikipedia.org/w/api.php?action=query&generator=random&grnnamespace=0&format=json";

    constexpr std::string_view m_ILLEGAL_CHARACTERS = "șəʿқаз&+/";
    constexpr uint8_t m_RETRY_COUNT = 3;

    std::string m_executeRequest(const std::string& page);
    std::string m_getPage(std::string page);
    bool m_isValidPage(const std::string& page);
}

static_assert(web_utils::m_RETRY_COUNT > 0, "Retry count cannot be equal or smaller than 0");
