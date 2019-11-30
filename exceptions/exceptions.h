//
// Created by lidor on 11/27/19.
//

#pragma once

#include <stdexcept>
#include <string>
#include <sstream>

namespace exceptions
{
    class NetworkError : public std::runtime_error
    {
    public:
        explicit NetworkError(const char* msg);

        [[nodiscard]] const char* what() const noexcept override;

    private:
        std::string m_msg;
    };
}
