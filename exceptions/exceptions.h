//
// Created by lidor on 11/27/19.
//

#pragma once

#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>

namespace exception_utils
{
    class NetworkError : public std::runtime_error
    {
    public:
        explicit NetworkError(const char* msg);

        [[nodiscard]] const char* what() const noexcept override;

        friend std::ostream& operator<<(std::ostream& stream, const NetworkError& e);

    private:
        std::string m_msg;
    };

    class BadJson : public std::runtime_error
    {
    public:
        explicit BadJson(const char* msg);

        [[nodiscard]] const char* what() const noexcept override;

        friend std::ostream& operator<<(std::ostream& stream, const BadJson& e);

    private:
        std::string m_msg;
    };

    class FileError : public std::runtime_error
    {
    public:
        explicit FileError(const char* msg);

        [[nodiscard]] const char* what() const noexcept override;

        friend std::ostream& operator<<(std::ostream& stream, const BadJson& e);

    private:
        std::string m_msg;
    };
}
