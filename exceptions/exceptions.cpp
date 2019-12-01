//
// Created by lidor on 11/27/19.
//

#include "exceptions.h"

namespace exception_utils
{
    NetworkError::NetworkError(const char* msg) : std::runtime_error(msg), m_msg(msg)
    {
        this->m_msg = "NetworkError: " + this->m_msg;
    }

    const char* NetworkError::what() const noexcept
    {
        return this->m_msg.c_str();
    }

    std::ostream& operator<<(std::ostream& stream, const NetworkError& e)
    {
        stream << e.what();
        return stream;
    }


    BadJson::BadJson(const char* msg) : std::runtime_error(msg), m_msg(msg)
    {
        this->m_msg = "Bad JSON: " + this->m_msg;
    }

    const char* BadJson::what() const noexcept
    {
        return this->m_msg.c_str();
    }

    std::ostream& operator<<(std::ostream& stream, const BadJson& e)
    {
        stream << e.what();
        return stream;
    }

    FileError::FileError(const char* msg) : std::runtime_error(msg), m_msg(msg)
    {
        this->m_msg = "FileError: " + this->m_msg;
    }

    const char* FileError::what() const noexcept
    {
        return this->m_msg.c_str();
    }

    std::ostream& operator<<(std::ostream& stream, const FileError& e)
    {
        stream << e.what();
        return stream;
    }
}
