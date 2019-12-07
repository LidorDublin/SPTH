//
// Created by lidor on 12/2/19.
//

#include "config_parser.h"

void config_parser::initToDefault()
{
    for (auto& [key, val] : settings)
    {
        *(uint16_t*)val = 5;
    }
}

void config_parser::readConfig(const std::string& fileName)
{
    std::ifstream f(fileName);
    readConfig(f);
}

void config_parser::readConfig(std::ifstream& f)
{
    initToDefault();

    std::string key;
    std::string val;

    // Read line by line
    for(std::string line; std::getline(f, line);)
    {
        // Delete all whitespaces
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        std::istringstream s(line);
        std::getline(s, key, '=');
        std::getline(s, val);

        if(key.empty() || val.empty())
            continue;

        if(config_parser::settings.count(key))
            *(uint16_t*)config_parser::settings.at(key) = std::stoi(val);
    }
}
