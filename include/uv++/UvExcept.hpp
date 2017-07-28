#ifndef UV_UVEXCEPT_HPP
#define UV_UVEXCEPT_HPP

#include <stdexcept>
#include <sstream>

#include <uv.h>

namespace uv
{
    #define uv_assert(c, status) \
        do { \
            if(!(c)) { \
                std::stringstream s; \
                s << "call uv function failed [ " << uv_strerror(status) <<"] "<< __FILE__ << ":" << __LINE__ ; \
                throw std::runtime_error(s.str()); \
            } \
        } while(0)
}

#endif
