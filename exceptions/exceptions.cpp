//
// Created by lidor on 11/27/19.
//

#include "exceptions.h"

namespace exceptions
{
    NetworkError::NetworkError(const char *msg) : std::runtime_error(msg) {}

    const char* NetworkError::what() const noexcept
    {
        std::stringstream s;
        s << "Network error: " << this->m_msg;

        return s.str().c_str();
    }
}
