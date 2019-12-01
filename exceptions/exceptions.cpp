//
// Created by lidor on 11/27/19.
//

#include "exceptions.h"

namespace exceptions
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
}
