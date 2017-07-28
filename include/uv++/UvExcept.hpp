#ifndef UV_UVEXCEPT_HPP
#define UV_UVEXCEPT_HPP

#include <stdexcept>
#include <sstream>

#include <uv.h>

namespace uv
{
    #define uv_assert(error) \
        do { \
            if((error)) { \
                std::stringstream s; \
                s << "call uv function failed [ " << error.toString() <<"] "<< __FILE__ << ":" << __LINE__ ; \
                throw std::runtime_error(s.str()); \
            } \
        } while(0)
}

#endif
